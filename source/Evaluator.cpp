#include "shadergraph/Evaluator.h"
#include "shadergraph/Variant.h"
#include "shadergraph/Block.h"
#include "shadergraph/ParserUtility.h"
#include "shadergraph/ValueImpl.h"

#include <dag/Graph.h>
#include <cpputil/StringHelper.h>

namespace shadergraph
{

Evaluator::Evaluator(const std::vector<BlockPtr>& blocks)
{
    Sort(blocks);
    Resolve();
}

std::string Evaluator::GenShaderCode() const
{
    std::string ret;

    // uniforms
    for (auto& b : m_blocks)
    {
        auto& unifs = b->GetUniforms();
        for (auto& u : unifs) {
            ret += "uniform " + TypeToString(u.type) + " " + u.name + ";\n";
        }
    }
    ret += "\n";

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

//    std::string main_func;
//    for (auto& b : m_blocks)
//    {
//        ret += b->GetBlockCode();
//
//        main_func += b->GetMainCode();
//    }
//
//    ret += cpputil::StringHelper::Format(R"(
//void main()
//{
//%s
//}
//	)", main_func.c_str());

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

void Evaluator::ResolveFunctions()
{
    for (auto& b : m_blocks)
    {
        auto& funcs = b->GetFunctions();

        auto curr_func = funcs[b->GetCurrFuncIdx()];
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