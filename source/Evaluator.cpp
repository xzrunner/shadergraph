#include "shadergraph/Evaluator.h"
#include "shadergraph/Variant.h"
#include "shadergraph/Block.h"
#include "shadergraph/ParserUtility.h"
#include "shadergraph/ValueImpl.h"

#include "shadergraph/block/Int.h"
#include "shadergraph/block/Float.h"
#include "shadergraph/block/Split.h"
#include "shadergraph/block/Time.h"

#include <dag/Graph.h>
#include <cpputil/StringHelper.h>

namespace shadergraph
{

void Evaluator::Rebuild(const BlockPtr& block)
{
    Clear();

    m_block = block;

    std::vector<BlockPtr> blocks;
    GetAntecedentNodes(block, blocks);

    Sort(blocks);
    Resolve();
    Concatenate();
}

std::string Evaluator::GenShaderCode() const
{
    std::string ret;

    // header
    ret += GenShaderHeaderCode();

    // uniforms
    ret += GenShaderUniformsCode();

    // functions
    ret += GenShaderFuncsCode();

    // main()
    ret += GenShaderMainCode();

    // version
    if (!ret.empty()) {
        ret = "#version 330 core\n" + ret;
    }

#ifdef _DEBUG
    printf("%s\n", ret.c_str());
#endif // _DEBUG

    return ret;
}

std::vector<Evaluator::Uniform>
Evaluator::CalcUniformValues() const
{
    std::vector<Uniform> ret;

    for (auto& b : m_blocks)
    {
        auto& unifs = b->GetUniforms();
        for (auto& src : unifs)
        {
            Uniform dst;

            dst.name = src.name;
            dst.type = src.type;

            // read default value
            dst.val = src.val;

            // calc input value
            assert(dst.type == VarType::Uniform);
            auto u_val = std::static_pointer_cast<UniformVal>(dst.val);
            for (auto& input : b->GetImports()) {
                if (input.var.full_name == src.name && !input.conns.empty()) {
                    assert(input.conns.size() == 1);
                    u_val->var = CalcValue(input.conns[0]);
                    break;
                }
            }

            ret.push_back(dst);
        }
    }

    return ret;
}

std::string Evaluator::GenShaderHeaderCode() const
{
    std::string code;
    for (auto& b : m_blocks) {
        auto str = b->GetHeader();
        if (!str.empty()) {
            Rename(str, *b);
            code += str + "\n";
        }
    }
    return code;
}

std::string Evaluator::GenShaderUniformsCode() const
{
    std::string code;
    for (auto& b : m_blocks)
    {
        auto& unifs = b->GetUniforms();
        for (auto& u : unifs)
        {
            assert(u.type == VarType::Uniform);
            auto u_var = std::static_pointer_cast<shadergraph::UniformVal>(u.val)->var;
            if (u_var.type != VarType::Invalid) {
                code += "uniform " + TypeToString(u_var.type) + " " + u.name + ";\n";
            }
        }
    }
    if (!code.empty()) {
        code += "\n";
    }
    return code;
}

std::string Evaluator::GenShaderFuncsCode() const
{
    std::string code;

    for (auto& b : m_blocks)
    {
        auto& funcs = b->GetFunctions();
        for (size_t i = 0, n = funcs.size(); i < n; ++i)
        {
            if (IsFuncNotExport(*b, i)) {
                continue;
            }

            std::string params;

            auto& f = funcs[i];
            auto f_val = std::static_pointer_cast<FunctionVal>(f.val);
            for (int i = 0, n = f_val->inputs.size(); i < n; ++i)
            {
                auto var = f_val->inputs[i];
                params += TypeToString(var.type) + " " + var.name;
                if (i != n - 1) {
                    params += ", ";
                }
            }

            std::string f_code;
            auto itr = m_real_funcs.find(&f);
            if (itr != m_real_funcs.end()) {
                f_code = itr->second;
            } else {
                f_code = f_val->code;
            }

            code += cpputil::StringHelper::Format(R"(
%s %s(%s)
{
%s
}

)", TypeToString(f_val->output.type).c_str(), f.name.c_str(), params.c_str(), f_code.c_str());
        }
    }

    return code;
}

std::string Evaluator::GenShaderMainCode() const
{
    std::string code;
    std::queue<dag::Node<Variant>::PortAddr> buf;
    buf.push({ m_block, -1 });
    std::set<BlockPtr> unique;
    while (!buf.empty())
    {
        auto c = buf.front(); buf.pop();
        auto b = std::static_pointer_cast<Block>(c.node.lock());
        assert(b);
        if (unique.find(b) != unique.end()) {
            continue;
        }
        unique.insert(b);

        auto str = b->GetBody();
        if (str.empty() && c.idx >= 0)
        {
            auto& outputs = b->GetExports();
            assert(c.idx >= 0 && c.idx < outputs.size());
            auto& var = outputs[c.idx].var.type;
            if (var.type != VarType::Function)
            {
                auto& funcs = b->GetFunctions();
                auto func_idx = b->GetCurrFuncIdx();
                if (func_idx >= 0 && func_idx < funcs.size())
                {
                    auto func = funcs[func_idx];
                    assert(func.type == VarType::Function);
                    auto f_val = std::static_pointer_cast<FunctionVal>(func.val);

                    auto& inputs = b->GetImports();

                    auto& outputs = b->GetExports();
                    assert(c.idx >= 0 && c.idx < outputs.size());
                    auto& output = outputs[c.idx];
                    str += TypeToString(var.type) + " #" + output.var.type.name + "# = ";
                    str += func.name + "(";
                    for (int i = 0, n = f_val->inputs.size(); i < n; ++i)
                    {
                        auto& param = f_val->inputs[i];
                        if (!inputs[i].conns.empty()) {
                            str += "#" + param.name + "#";
                        } else {
                            str += GetDefaultValueString(param.type);
                        }

                        if (i != n - 1) {
                            str += ", ";
                        }
                    }
                    str += ");\n";
                }
            }
        }

        if (!str.empty())
        {
            Rename(str, *b);
            code = str + "\n" + code;
        }

        for (auto& port : b->GetImports()) {
            for (auto& conn : port.conns) {
                if (port.var.type.type == VarType::Uniform) {
                    continue;
                }
                if (port.var.type.type == VarType::Function &&
                    b != m_block) {
                    continue;
                }
                buf.push(conn);
            }
        }
    }

    return cpputil::StringHelper::Format(R"(
void main()
{
%s
}
	)", code.c_str());
}

void Evaluator::Clear()
{
    m_block.reset();
    m_blocks.clear();
    m_symbols.clear();
    m_real_names.clear();
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
    ResolveVariants();
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

            if (f_var.type != t_var.type && f_var.type != VarType::Dynamic && t_var.type != VarType::Dynamic)
            {
                int f_idx = -1, t_idx = -1;
                if (f_var.type >= VarType::Bool && f_var.type <= VarType::Bool4)
                {
                    assert(t_var.type >= VarType::Bool && t_var.type <= VarType::Bool4);
                    f_idx = static_cast<int>(f_var.type) - static_cast<int>(VarType::Bool);
                    t_idx = static_cast<int>(t_var.type) - static_cast<int>(VarType::Bool);
                }
                else if (f_var.type >= VarType::Int && f_var.type <= VarType::Int4)
                {
                    assert(t_var.type >= VarType::Int && t_var.type <= VarType::Int4);
                    f_idx = static_cast<int>(f_var.type) - static_cast<int>(VarType::Int);
                    t_idx = static_cast<int>(t_var.type) - static_cast<int>(VarType::Int);
                }
                else if (f_var.type >= VarType::Float && f_var.type <= VarType::Float4)
                {
                    assert(t_var.type >= VarType::Float && t_var.type <= VarType::Float4);
                    f_idx = static_cast<int>(f_var.type) - static_cast<int>(VarType::Float);
                    t_idx = static_cast<int>(t_var.type) - static_cast<int>(VarType::Float);
                }

                if (f_idx >= 0 && t_idx >= 0)
                {
                    auto itr = m_real_names.find(&t_var);
                    assert(itr != m_real_names.end());
                    if (f_idx > t_idx)
                    {
                        assert(t_idx >= 0 && t_idx <= 2);
                        switch (t_idx)
                        {
                        case 0:
                            itr->second = itr->second + ".x";
                            break;
                        case 1:
                            itr->second = itr->second + ".xy";
                            break;
                        case 2:
                            itr->second = itr->second + ".xyz";
                            break;
                        }
                    }
                    else
                    {
                        assert(f_idx < t_idx);;
                        assert(t_idx >= 1 && t_idx <= 3);
                        switch (t_idx)
                        {
                        case 1:
                            assert(f_idx == 0);
                            itr->second = cpputil::StringHelper::Format("%s(%s, %s)",
                                TypeToString(t_var.type).c_str(), itr->second.c_str(), itr->second.c_str());
                            break;
                        case 2:
                            assert(f_idx >= 0 && f_idx <= 1);
                            switch (f_idx)
                            {
                            case 0:
                                itr->second = cpputil::StringHelper::Format("%s(%s, %s, %s)",
                                    TypeToString(t_var.type).c_str(), itr->second.c_str(), itr->second.c_str(), itr->second.c_str());
                                break;
                            case 1:
                                itr->second = cpputil::StringHelper::Format("%s(%s, 0)",
                                    TypeToString(t_var.type).c_str(), itr->second.c_str());
                                break;
                            }
                            break;
                        case 3:
                            assert(f_idx >= 0 && f_idx <= 2);
                            switch (f_idx)
                            {
                            case 0:
                                itr->second = cpputil::StringHelper::Format("%s(%s, %s, %s, %s)",
                                    TypeToString(t_var.type).c_str(), itr->second.c_str(), itr->second.c_str(), itr->second.c_str(), itr->second.c_str());
                                break;
                            case 1:
                                itr->second = cpputil::StringHelper::Format("%s(%s, 0, 0)",
                                    TypeToString(t_var.type).c_str(), itr->second.c_str());
                                break;
                            case 2:
                                itr->second = cpputil::StringHelper::Format("%s(%s, 0)",
                                    TypeToString(t_var.type).c_str(), itr->second.c_str());
                                break;
                            }
                            break;
                        }
                    }
                }
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
                for (auto& func : funcs)
                {
                    std::string f_code;
                    auto itr = m_real_funcs.find(&func);
                    if (itr != m_real_funcs.end()) {
                        f_code = itr->second;;
                    } else {
                        auto f_val = std::static_pointer_cast<FunctionVal>(func.val);
                        f_code = f_val->code;
                    }
                    cpputil::StringHelper::ReplaceAll(f_code, from, to);

                    if (itr != m_real_funcs.end()) {
                        itr->second = f_code;
                    } else {
                        m_real_funcs.insert({ &func, f_code });
                    }
                }

                ++idx;
            }
        }
    }
}

void Evaluator::ResolveVariants()
{
    for (auto& b : m_blocks)
    {
        for (auto& o : b->GetExports())
        {
            if (o.conns.empty()) {
                continue;
            }

            auto& var = o.var.type;
            std::string name = var.default_name.empty() ? var.name : var.default_name;
            auto itr = m_symbols.find(name);
            if (itr == m_symbols.end())
            {
                m_real_names.insert({ &var, name });
                m_symbols.insert(name);
            }
            else
            {
                int idx = 0;
                do {
                    auto _name = name + std::to_string(idx++);
                    auto itr = m_symbols.find(_name);
                    if (itr == m_symbols.end())
                    {
                        m_real_names.insert({ &var, _name });
                        m_symbols.insert(_name);
                        break;
                    }
                } while (true);
            }
        }
    }
}

void Evaluator::Rename(std::string& str, const Block& block) const
{
    for (auto& i : block.GetImports())
    {
        const auto f = "#" + i.var.type.name + "#";
        std::string t = i.var.type.default_name.empty() ?
            i.var.type.name : i.var.type.default_name;
        auto itr = m_real_names.find(&i.var.type);
        if (itr != m_real_names.end()) {
            t = itr->second;
        }
        cpputil::StringHelper::ReplaceAll(str, f, t);
    }
    for (auto& o : block.GetExports())
    {
        const auto f = "#" + o.var.type.name + "#";
        std::string t = o.var.type.default_name.empty() ?
            o.var.type.name : o.var.type.default_name;
        auto itr = m_real_names.find(&o.var.type);
        if (itr != m_real_names.end()) {
            t = itr->second;
        }
        cpputil::StringHelper::ReplaceAll(str, f, t);
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

Variant Evaluator::CalcValue(const dag::Node<Variant>::PortAddr& conn)
{
    Variant ret;

    auto node = conn.node.lock();
    auto node_type = node->get_type();
    if (node_type == rttr::type::get<block::Int>())
    {
        ret.type = VarType::Int;

        auto val = std::make_shared<IntVal>();

        auto i_node = std::static_pointer_cast<block::Int>(node);
        val->x = i_node->GetValue();

        ret.val = val;
    }
    else if (node_type == rttr::type::get<block::Float>())
    {
        ret.type = VarType::Float;

        auto val = std::make_shared<FloatVal>();

        auto f_node = std::static_pointer_cast<block::Float>(node);
        val->x = f_node->GetValue();

        ret.val = val;
    }
    else if (node_type == rttr::type::get<block::Split>())
    {
        int input_idx = -1;
        for (int i = 0; i <= static_cast<int>(block::Split::Input::RGBA); ++i) {
            if (!node->GetImports()[i].conns.empty()) {
                input_idx = i;
                break;
            }
        }

        if (input_idx >= 0)
        {
            auto& in_conns = node->GetImports()[input_idx].conns;
            assert(in_conns.size() == 1);
            auto in_conn = in_conns[0];
            auto input = CalcValue(in_conn);
            if (input.val)
            {
                switch (input.type)
                {
                case VarType::Float4:
                {
                    ret.type = VarType::Float;

                    auto val = std::make_shared<FloatVal>();
                    auto v_f4 = std::static_pointer_cast<Float4Val>(input.val);
                    assert(in_conn.idx >= 0 && in_conn.idx < 4);
                    val->x = v_f4->xyzw[conn.idx];
                    ret.val = val;
                }
                    break;
                case VarType::Float3:
                {
                    ret.type = VarType::Float;

                    auto val = std::make_shared<FloatVal>();
                    auto v_f3 = std::static_pointer_cast<Float3Val>(input.val);
                    assert(in_conn.idx >= 0 && in_conn.idx < 3);
                    val->x = v_f3->xyz[conn.idx];
                    ret.val = val;
                }
                    break;
                case VarType::Float2:
                {
                    ret.type = VarType::Float;

                    auto val = std::make_shared<FloatVal>();
                    auto v_f2 = std::static_pointer_cast<Float2Val>(input.val);
                    assert(in_conn.idx >= 0 && in_conn.idx < 2);
                    val->x = v_f2->xy[conn.idx];
                    ret.val = val;
                }
                    break;
                default:
                    assert(0);
                }
            }
        }
    }
    else
    {
        auto& exports = node->GetExports();
        assert(conn.idx >= 0 && conn.idx < exports.size());
        ret = exports[conn.idx].var.type;
    }

    return ret;
}

std::string Evaluator::GetDefaultValueString(VarType type)
{
    switch (type)
    {
    case VarType::Bool:
        return "false";
    case VarType::Bool2:
        return "bvec2(false, false)";
    case VarType::Bool3:
        return "bvec3(false, false, false)";
    case VarType::Bool4:
        return "bvec4(false, false, false, false)";
    case VarType::UInt:
    case VarType::Int:
        return "0";
    case VarType::Int2:
        return "ivec2(0, 0)";
    case VarType::Int3:
        return "ivec3(0, 0, 0)";
    case VarType::Int4:
        return "ivec4(0, 0, 0, 0)";
    case VarType::Float:
        return "0.0";
    case VarType::Float2:
        return "vec2(0.0, 0.0)";
    case VarType::Float3:
        return "vec3(0.0, 0.0, 0.0)";
    case VarType::Float4:
        return "vec4(0.0, 0.0, 0.0, 0.0)";
    case VarType::Matrix2:
        return "mat2()";
    case VarType::Matrix3:
        return "mat3()";
    case VarType::Matrix4:
        return "mat4()";
    default:
        return "";
    }
}

}