#include "shadergraph/Evaluator.h"
#include "shadergraph/Variant.h"
#include "shadergraph/Block.h"
#include "shadergraph/ParserUtility.h"
#include "shadergraph/ValueImpl.h"

#include <dag/Graph.h>
#include <cpputil/StringHelper.h>

namespace shadergraph
{

Evaluator::Evaluator(const BlockPtr& block)
{
    std::vector<BlockPtr> blocks;
    GetAntecedentNodes(block, blocks);

    Sort(blocks);
    Resolve();

    Rename();
    Concatenate();
}

std::string Evaluator::GenShaderCode() const
{
    std::string ret;

    // header
    for (auto& b : m_blocks) {
        auto str = b->GetHeader();
        if (!str.empty()) {
            ret += str + "\n";
        }
    }

    // uniforms
    for (auto& b : m_blocks)
    {
        auto& unifs = b->GetUniforms();
        for (auto& u : unifs) {
            ret += "uniform " + TypeToString(u.type) + " " + u.name + ";\n";
        }
    }
    if (!ret.empty()) {
        ret += "\n";
    }

    // functions
    for (auto& b : m_blocks)
    {
        auto& funcs = b->GetFunctions();
        for (size_t i = 0, n = funcs.size(); i < n; ++i)
        {
            if (IsFuncNotExport(*b, i)) {
                continue;
            }

            std::string params;

            auto f = funcs[i];
            auto f_val = std::static_pointer_cast<FunctionVal>(f.val);
            for (int i = 0, n = f_val->inputs.size(); i < n; ++i)
            {
                auto var = f_val->inputs[i];
                params += TypeToString(var.type) + " " + var.name;
                if (i != n - 1) {
                    params += ", ";
                }
            }

            ret += cpputil::StringHelper::Format(R"(
%s %s(%s)
{
%s
}

)", TypeToString(f_val->output.type).c_str(), f.name.c_str(), params.c_str(), f_val->code.c_str());
        }
    }

    // main()

    std::string main;
    for (auto& b : m_blocks) {
        auto str = b->GetBody();
        if (!str.empty()) {
            main += str + "\n";
        }
    }
    ret += cpputil::StringHelper::Format(R"(
void main()
{
%s
}
	)", main.c_str());

    // version
    if (!ret.empty()) {
        ret = "#version 330 core\n" + ret;
    }

    // rename
    for (auto& b : m_blocks)
    {
        for (auto& i : b->GetImports())
        {
            const auto f = "#" + i.var.type.name + "#";
            std::string t = i.var.type.default_name.empty() ?
                i.var.type.name : i.var.type.default_name;
            auto itr = m_real_names.find(&i.var.type);
            if (itr != m_real_names.end()) {
                t = itr->second;
            }
            cpputil::StringHelper::ReplaceAll(ret, f, t);
        }
        for (auto& o : b->GetExports())
        {
            const auto f = "#" + o.var.type.name + "#";
            std::string t = o.var.type.default_name.empty() ?
                o.var.type.name : o.var.type.default_name;
            auto itr = m_real_names.find(&o.var.type);
            if (itr != m_real_names.end()) {
                t = itr->second;
            }
            cpputil::StringHelper::ReplaceAll(ret, f, t);
        }
    }

    return ret;
}

void Evaluator::Sort(const std::vector<BlockPtr>& blocks)
{
    std::vector<std::shared_ptr<dag::Node<Variant>>> _blocks(blocks.size());
    for (size_t i = 0, n = blocks.size(); i < n; ++i) {
        _blocks[i] = blocks[i];
    }
    auto orders = dag::Graph<Variant>::TopologicalSorting(_blocks);

    m_blocks.resize(orders.size());
    for (size_t i = 0, n = orders.size(); i < n; ++i) {
        m_blocks[i] = blocks[orders[i]];
    }
}

void Evaluator::Resolve()
{
    ResolveFunctions();
}

void Evaluator::Rename()
{
    for (auto& b : m_blocks)
    {
        for (auto& o : b->GetExports())
        {
            auto& v = o.var.type;

            auto itr = m_real_names.find(&v);
            if (itr != m_real_names.end())
            {
                auto itr2 = m_symbols.find(itr->second);
                if (itr2 == m_symbols.end()) {
                    m_symbols.insert(itr->second);
                    continue;
                }
            }

            std::string real_name = v.default_name.empty() ? v.name : v.default_name;
            auto itr2 = m_symbols.find(real_name);
            if (itr2 == m_symbols.end())
            {
                m_real_names.insert({ &v, real_name });
                m_symbols.insert(real_name);
            }
            else
            {
                int idx = 0;
                do {
                    auto _real_name = real_name + std::to_string(idx++);
                    auto itr = m_symbols.find(_real_name);
                    if (itr == m_symbols.end())
                    {
                        m_real_names.insert({ &v, _real_name });
                        m_symbols.insert(_real_name);
                        break;
                    }
                } while (true);
            }
        }
    }
}

void Evaluator::Concatenate()
{
    for (auto& b : m_blocks)
    {
        for (auto& i : b->GetImports())
        {
            auto& conns = i.conns;
            if (conns.empty()) {
                continue;
            }
            assert(conns.size() == 1);
            auto& conn = conns[0];
            auto from = conn.node.lock();
            assert(from);
            auto& f_var = from->GetExports()[conn.idx].var.type;
            auto& t_var = const_cast<Variant&>(i.var.type);
            auto f_itr = m_real_names.find(&f_var);
            if (f_itr == m_real_names.end()) {
                continue;
            }
            auto t_itr = m_real_names.find(&t_var);
            if (t_itr != m_real_names.end()) {
                t_itr->second = f_itr->second;
            } else {
                m_real_names.insert({ &t_var, f_itr->second });
            }
        }
    }
}

void Evaluator::ResolveFunctions()
{
    for (auto& b : m_blocks)
    {
        auto func_idx = b->GetCurrFuncIdx();
        if (func_idx < 0) {
            continue;
        }

        auto& funcs = b->GetFunctions();

        auto curr_func = funcs[func_idx];
        size_t input_idx = std::static_pointer_cast<FunctionVal>(curr_func.val)->inputs.size();

        for (size_t i = 0, n = funcs.size(); i < n; ++i)
        {
            if (IsFuncNotExport(*b, i)) {
                continue;
            }

            auto f = funcs[i];
            auto f_val = std::static_pointer_cast<FunctionVal>(f.val);
            int idx = input_idx;
            for (auto& d : f_val->desc)
            {
                if (d->GetType() != ParserProp::Type::Function) {
                    continue;
                }

                auto& inputs = b->GetImports();
                assert(idx >= 0 && idx < static_cast<int>(inputs.size()));
                auto& conns = inputs[idx].conns;
                if (conns.empty()) {
                    ++idx;
                    continue;
                }
                assert(conns.size() == 1);
                auto& conn = conns[0];

                auto prev_node = conn.node.lock();
                if (!prev_node) {
                    ++idx;
                    continue;
                }

                auto prop_f = std::static_pointer_cast<PropFunction>(d);
                std::string from = prop_f->name;

                std::string to = prev_node->GetExports()[conn.idx].var.type.name;
                cpputil::StringHelper::ReplaceAll(f_val->code, from, to);

                ++idx;
            }
        }
    }
}

void Evaluator::GetAntecedentNodes(const BlockPtr& src, std::vector<BlockPtr>& dst)
{
    std::queue<BlockPtr> buf;
    buf.push(src);

    std::set<BlockPtr> unique;
    while (!buf.empty())
    {
        auto n = buf.front(); buf.pop();
        if (unique.find(n) != unique.end()) {
            continue;
        }
        unique.insert(n);
        dst.push_back(n);
        for (auto& port : n->GetImports()) {
            for (auto& conn : port.conns) {
                buf.push(std::static_pointer_cast<Block>(conn.node.lock()));
            }
        }
    }
}

bool Evaluator::IsFuncNotExport(const Block& block, int func_idx)
{
    auto func = block.GetFunctions()[func_idx];
    auto f_val = std::static_pointer_cast<FunctionVal>(func.val);

    bool ex = false;
    for (auto& d : f_val->desc) {
        if (d->GetType() == ParserProp::Type::Export) {
            ex = true;
            break;
        }
    }

    return ex && block.GetCurrFuncIdx() != func_idx;
}

}