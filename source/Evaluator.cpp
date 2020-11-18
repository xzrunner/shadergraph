#include "shadergraph/Evaluator.h"
#include "shadergraph/Variant.h"
#include "shadergraph/Block.h"
#include "shadergraph/ParserUtility.h"
#include "shadergraph/ValueImpl.h"
#include "shadergraph/EvalVar.h"

#include "shadergraph/block/Int.h"
#include "shadergraph/block/Float.h"
#include "shadergraph/block/Split.h"
#include "shadergraph/block/Time.h"
#include "shadergraph/block/VertexAttribute.h"
#include "shadergraph/block/VertToFrag.h"

#include "shadergraph/block/Input.h"

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

bool Evaluator::HasBlock(const BlockPtr& block) const
{
    for (auto& b : m_blocks) {
        if (b == block) {
            return true;
        }
    }
    return false;
}

void Evaluator::AddBlock(const BlockPtr& block)
{
    if (!HasBlock(block)) {
        m_blocks.push_back(block);
    }
}

std::string Evaluator::GenShaderCode(ShaderType shader_type) const
{
    std::string ret;

    // header
    ret += GenShaderHeaderCode(shader_type);

    // global variants
    ret += GenShaderGlobalVarsCode(false);

    // ubo
    auto ubo = GenShaderGlobalVarsCode(true);
    if (!ubo.empty()) 
    {
        ret += "\nuniform UBO {\n";
        ret += ubo;
        ret += "};\n";
    }

    // functions
    ret += GenShaderFuncsCode();

    // main()
    ret += cpputil::StringHelper::Format(R"(
void main()
{
%s
}
	)", GenShaderMainCode(shader_type).c_str());

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
        auto& vars = b->GetVariants();
        for (auto& src : vars)
        {
            if (src.type != VarType::Uniform) {
                continue;
            }

            Uniform dst;

            dst.name = src.name;
            dst.type = src.type;

            // read default value
            dst.val = src.val;

            assert(src.type == VarType::Uniform);

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

VarType Evaluator::QueryRealType(const Variant* var) const
{
    auto itr = m_real_types.find(var);
    return itr == m_real_types.end() ? var->type : itr->second;
}

std::string Evaluator::QueryRealName(const Variant* var) const
{
    auto itr = m_real_names.var2name.find(var);
    return itr == m_real_names.var2name.end() ? var->name : itr->second;
}

std::string Evaluator::GenShaderHeaderCode(ShaderType shader_type) const
{
    std::string code;
    for (auto& b : m_blocks) 
    {
        auto block_type = b->get_type();
        // skip VertexAttribute
        if (shader_type != ShaderType::Vert && block_type == rttr::type::get<block::VertexAttribute>()) {
            continue;
        }

        if (block_type == rttr::type::get<block::VertToFrag>())
        {
            Evaluator eval;
            eval.Rebuild(b);
            auto vt = eval.QueryRealType(&b->GetImports()[0].var.type);
            if (vt != VarType::Invalid && vt != VarType::Dynamic) 
            {
                auto var_name = std::static_pointer_cast<block::VertToFrag>(b)->GetVarName();
                auto var_type = TypeToString(vt);
                switch (shader_type)
                {
                case ShaderType::Vert:
                    code += cpputil::StringHelper::Format("out %s %s;\n", var_type.c_str(), var_name.c_str());
                    break;
                case ShaderType::Frag:
                    code += cpputil::StringHelper::Format("in %s %s;\n", var_type.c_str(), var_name.c_str());
                    break;
                default:
                    assert(0);
                }
            }
            continue;
        }

        auto str = b->GetHeader(*this);
        if (!str.empty()) {
            Rename(str, *b);
            code += str + "\n";
        }
    }
    return code;
}

std::string Evaluator::GenShaderGlobalVarsCode(bool only_uniform) const
{
    std::string code;

    std::set<std::string> unique;
    for (auto& b : m_blocks)
    {
        auto& vars = b->GetVariants();
        for (auto& var : vars)
        {
            if (unique.find(var.name) != unique.end()) {
                continue;
            }

            bool uniform = false;
            Variant v;
            if (var.type == VarType::Uniform) {
                v = std::static_pointer_cast<shadergraph::UniformVal>(var.val)->var;
                uniform = true;
            } else {
                v = var;
            }
            if (v.type == VarType::Invalid) {
                continue;
            }

            if ((only_uniform && !uniform) ||
                (!only_uniform && uniform)) {
                continue;
            }

            //if (uniform) {
            //    code += "uniform ";
            //}

            if (v.type == VarType::Array)
            {
                auto v_array = std::static_pointer_cast<ArrayVal>(v.val);
                int num = v_array->items.size();
                if (num > 0) {
                    code += cpputil::StringHelper::Format("%s %s[%d];\n",
                        TypeToString(v_array->type).c_str(), v.name.c_str(), num);
                }
            }
            else
            {
                if (v.val && !uniform)
                {
                    switch (v.type)
                    {
                    case VarType::Int:
                    {
                        auto i_val = std::static_pointer_cast<IntVal>(v.val);
                        code += cpputil::StringHelper::Format("%s %s = %d;\n", TypeToString(v.type).c_str(), v.name.c_str(), i_val->x);
                    }
                        break;

                    case VarType::Float:
                    {
                        auto f_val = std::static_pointer_cast<FloatVal>(v.val);
                        code += cpputil::StringHelper::Format("%s %s = %f;\n", TypeToString(v.type).c_str(), v.name.c_str(), f_val->x);
                    }
                        break;

                    case VarType::Float2:
                    {
                        auto f2_val = std::static_pointer_cast<Float2Val>(v.val);
                        code += cpputil::StringHelper::Format("%s %s = vec2(%f, %f);\n", TypeToString(v.type).c_str(), v.name.c_str(), f2_val->xy[0], f2_val->xy[1]);
                    }
                        break;

                    case VarType::Float3:
                    {
                        auto f3_val = std::static_pointer_cast<Float3Val>(v.val);
                        code += cpputil::StringHelper::Format("%s %s = vec3(%f, %f, %f);\n", TypeToString(v.type).c_str(), v.name.c_str(), f3_val->xyz[0], f3_val->xyz[1], f3_val->xyz[2]);
                    }
                        break;

                    default:
                        assert(0);
                    }
                }
                else
                {
                    switch (v.qualifier)
                    {
                    case TypeQualifier::Const:
                        code += "const ";
                        break;
                    case TypeQualifier::Input:
                        code += "in ";
                        break;
                    case TypeQualifier::Output:
                        code += "out ";
                        break;
                    case TypeQualifier::Attribute:
                        code += "attribute ";
                        break;
                    case TypeQualifier::Volatile:
                        code += "volatile ";
                        break;
                    case TypeQualifier::Varying:
                        code += "varying ";
                        break;
                    }
                    code += TypeToString(v) + " " + v.name + ";\n";
                }
            }
            unique.insert(var.name);
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

    std::set<std::string> unique;
    for (auto& b : m_blocks)
    {
        auto& funcs = b->GetFunctions();
        for (size_t i = 0, n = funcs.size(); i < n; ++i)
        {
            if (unique.find(funcs[i].first.name) != unique.end()) {
                continue;
            }

            if (funcs[i].second && b->GetCurrFuncIdx() != i) {
                continue;
            }

            auto& f = funcs[i];
            auto f_val = std::static_pointer_cast<FunctionVal>(f.first.val);

            std::string f_code;
            auto itr = m_real_funcs.find(&f.first);
            if (itr != m_real_funcs.end()) {
                f_code = itr->second;
            } else {
                f_code = f_val->code;
            }

            unique.insert(funcs[i].first.name);

            code += f_code;
        }
    }

    return code;
}

std::string Evaluator::GenShaderMainCode(ShaderType shader_type) const
{
    std::string ret;
    for (auto& b : m_blocks)
    {
        auto block_type = b->get_type();

        // gen vert2frag for vert
        if (shader_type == ShaderType::Vert && block_type == rttr::type::get<block::VertToFrag>()) 
        {
            auto& conns = b->GetImports()[0].conns;
            if (conns.empty()) {
                continue;
            }

            Evaluator eval;
            eval.Rebuild(b);
            auto vt = eval.QueryRealType(&b->GetImports()[0].var.type);

            auto dst = std::static_pointer_cast<block::VertToFrag>(b)->GetVarName();

            auto node = conns[0].node.lock();
            int idx = conns[0].idx;
            auto src = eval.QueryRealName(&node->GetExports()[idx].var.type);

            ret += cpputil::StringHelper::Format("%s = %s;", dst.c_str(), src.c_str());

            continue;
        }

        auto str = b->GetBody(*this);
        if (str.empty()) 
        {
            auto& outputs = b->GetExports();
            for (int i = 0, n = outputs.size(); i < n; ++i) {
                if (!outputs[i].conns.empty()) {
                    str = ResolveFuncCall(*b, i);
                    break;
                }
            }
        }

        if (!str.empty()) {
            Rename(str, *b);
            ret.append(str + "\n");
        }
    }
    return ret;
}

void Evaluator::Clear()
{
    m_block.reset();
    m_blocks.clear();
    m_real_names.Clear();
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
    // for dynamic
    for (auto& b : m_blocks)
    {
        bool has_dynamic = false;
        VarType max_type = VarType::Invalid;
        for (auto& i : b->GetImports())
        {
            if (i.var.type.type == VarType::Dynamic)
            {
                has_dynamic = true;
                if (!i.conns.empty())
                {
                    assert(i.conns.size() == 1);
                    auto& conn = i.conns[0];
                    auto real_type = conn.node.lock()->GetExports()[conn.idx].var.type.type;
                    //m_real_types.insert({ &i.var.type, real_type });
                    m_real_types[&i.var.type] = real_type;
                    if (real_type > max_type) {
                        max_type = real_type;
                    }
                }
            }
            if (i.var.type.type > max_type) {
                max_type = i.var.type.type;
            }
        }
        for (auto& o : b->GetExports())
        {
            if (o.var.type.type == VarType::Dynamic)
            {
                has_dynamic = true;
                if (!o.conns.empty())
                {
                    for (auto& conn : o.conns)
                    {
                        auto real_type = conn.node.lock()->GetImports()[conn.idx].var.type.type;
                        //m_real_types.insert({ &o.var.type, real_type });
                        m_real_types[&o.var.type] = real_type;
                        if (real_type > max_type) {
                            max_type = real_type;
                        }
                    }
                }
            }
            if (o.var.type.type > max_type) {
                max_type = o.var.type.type;
            }
        }
        if (has_dynamic)
        {
            for (auto& i : b->GetImports()) {
                if (i.var.type.type == VarType::Dynamic) {
                    //assert(!i.var.type.val);
                    //m_real_types.insert({ &i.var.type, max_type });
                    m_real_types[&i.var.type] = max_type;
                }
            }
            for (auto& o : b->GetExports()) {
                if (o.var.type.type == VarType::Dynamic) {
                    //assert(!o.var.type.val);
                    //m_real_types.insert({ &o.var.type, max_type });
                    m_real_types[&o.var.type] = max_type;
                }
            }
        }
    }

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
            auto f_itr = m_real_names.var2name.find(&f_var);
            if (f_itr == m_real_names.var2name.end()) {
                continue;
            }
            auto t_itr = m_real_names.var2name.find(&t_var);
            if (t_itr != m_real_names.var2name.end()) {
                t_itr->second = f_itr->second;
            } else {
                m_real_names.Insert(&t_var, f_itr->second);
            }

            auto f_type = f_var.type;
            auto t_type = t_var.type;
            if (f_type == VarType::Dynamic) {
                f_type = QueryRealType(&f_var);
            }
            if (t_type == VarType::Dynamic) {
                t_type = QueryRealType(&t_var);
            }

            if (f_type != t_type && f_type != VarType::Dynamic && t_type != VarType::Dynamic)
            {
                int f_idx = -1, t_idx = -1;
                if (f_type >= VarType::Bool && f_type <= VarType::Bool4)
                {
                    assert(t_type >= VarType::Bool && t_type <= VarType::Bool4);
                    f_idx = static_cast<int>(f_type) - static_cast<int>(VarType::Bool);
                    t_idx = static_cast<int>(t_type) - static_cast<int>(VarType::Bool);
                }
                else if (f_type >= VarType::Int && f_type <= VarType::Int4)
                {
                    assert(t_type >= VarType::Int && t_type <= VarType::Int4);
                    f_idx = static_cast<int>(f_type) - static_cast<int>(VarType::Int);
                    t_idx = static_cast<int>(t_type) - static_cast<int>(VarType::Int);
                }
                else if (f_type >= VarType::Float && f_type <= VarType::Float4)
                {
                    assert(t_type >= VarType::Float && t_type <= VarType::Float4);
                    f_idx = static_cast<int>(f_type) - static_cast<int>(VarType::Float);
                    t_idx = static_cast<int>(t_type) - static_cast<int>(VarType::Float);
                }

                if (f_idx >= 0 && t_idx >= 0)
                {
                    auto itr = m_real_names.var2name.find(&t_var);
                    assert(itr != m_real_names.var2name.end());
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
                                TypeToString(t_type).c_str(), itr->second.c_str(), itr->second.c_str());
                            break;
                        case 2:
                            assert(f_idx >= 0 && f_idx <= 1);
                            switch (f_idx)
                            {
                            case 0:
                                itr->second = cpputil::StringHelper::Format("%s(%s, %s, %s)",
                                    TypeToString(t_type).c_str(), itr->second.c_str(), itr->second.c_str(), itr->second.c_str());
                                break;
                            case 1:
                                itr->second = cpputil::StringHelper::Format("%s(%s, 0)",
                                    TypeToString(t_type).c_str(), itr->second.c_str());
                                break;
                            }
                            break;
                        case 3:
                            assert(f_idx >= 0 && f_idx <= 2);
                            switch (f_idx)
                            {
                            case 0:
                                itr->second = cpputil::StringHelper::Format("%s(%s, %s, %s, %s)",
                                    TypeToString(t_type).c_str(), itr->second.c_str(), itr->second.c_str(), itr->second.c_str(), itr->second.c_str());
                                break;
                            case 1:
                                itr->second = cpputil::StringHelper::Format("%s(%s, 0, 0)",
                                    TypeToString(t_type).c_str(), itr->second.c_str());
                                break;
                            case 2:
                                itr->second = cpputil::StringHelper::Format("%s(%s, 0)",
                                    TypeToString(t_type).c_str(), itr->second.c_str());
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
        for (size_t i = 0, n = funcs.size(); i < n; ++i)
        {
            if (funcs[i].second && b->GetCurrFuncIdx() != i) {
                continue;
            }

            auto f = funcs[i];
            auto f_val = std::static_pointer_cast<FunctionVal>(f.first.val);
            for (auto& d : f_val->desc)
            {
                if (d->GetType() != ParserProp::Type::Function) {
                    continue;
                }

                auto prop_f = std::static_pointer_cast<PropFunction>(d);

                int in_idx = -1;
                auto& inputs = b->GetImports();
                for (int i = 0, n = inputs.size(); i < n; ++i) {
                    if (inputs[i].var.type.name == prop_f->name) {
                        in_idx = i;
                        break;
                    }
                }

                if (in_idx < 0) {
                    continue;
                }

                auto& conns = inputs[in_idx].conns;
                if (conns.empty()) {
                    continue;
                }
                assert(conns.size() == 1);
                auto& conn = conns[0];

                auto prev_node = conn.node.lock();
                if (!prev_node) {
                    continue;
                }

                std::string from = prop_f->name;
                std::string to = prev_node->GetExports()[conn.idx].var.type.name;
                for (auto& func : funcs)
                {
                    std::string f_code;
                    auto itr = m_real_funcs.find(&func.first);
                    if (itr != m_real_funcs.end()) {
                        f_code = itr->second;;
                    } else {
                        auto f_val = std::static_pointer_cast<FunctionVal>(func.first.val);
                        f_code = f_val->code;
                    }
                    cpputil::StringHelper::ReplaceAll(f_code, from, to);

                    if (itr != m_real_funcs.end()) {
                        itr->second = f_code;
                    } else {
                        m_real_funcs.insert({ &func.first, f_code });
                    }
                }
            }
        }
    }
}

void Evaluator::ResolveVariants()
{
    for (auto& b : m_blocks)
    {
        auto block_type = b->get_type();
        for (int i = 0, n = b->GetExports().size(); i < n; ++i)
        {
            auto& o = b->GetExports()[i];
            if (o.conns.empty()) {
                continue;
            }

            auto& var = o.var.type;
            if (m_real_names.var2name.find(&var) != m_real_names.var2name.end()) {
                continue;
            }

            auto name = var.name;

            auto& default_outs = b->GetDefaultOutValues();
            if (default_outs[i].val && default_outs[i].type == VarType::String) {
                name = std::static_pointer_cast<StringVal>(default_outs[i].val)->str;
            }

            if (block_type == rttr::type::get<block::VertexAttribute>()) {
                name = std::static_pointer_cast<block::VertexAttribute>(b)->GetVarName();
            } else if (block_type == rttr::type::get<block::VertToFrag>()) {
                name = std::static_pointer_cast<block::VertToFrag>(b)->GetVarName();
            } else if (block_type == rttr::type::get<block::Input>()) {
                name = std::static_pointer_cast<block::Input>(b)->GetVarName();
                if (!b->GetImports()[0].conns.empty()) 
                {
                    assert(b->GetImports()[0].conns.size() == 1);
                    auto node = b->GetImports()[0].conns[0].node.lock();
                    int idx = b->GetImports()[0].conns[0].idx;
                    auto& v = node->GetExports()[idx].var.type;
                    m_real_names.Insert(&var, v.name);
                    continue;
                }
            }

            auto itr = m_real_names.used.find(name);
            if (itr == m_real_names.used.end())
            {
                m_real_names.Insert(&var, name);
            }
            else
            {
                int idx = 0;
                do {
                    auto _name = name + std::to_string(idx++);
                    auto itr = m_real_names.used.find(_name);
                    if (itr == m_real_names.used.end())
                    {
                        m_real_names.Insert(&var, _name);
                        break;
                    }
                } while (true);
            }
        }
    }
}

void Evaluator::Rename(std::string& str, const Block& block) const
{
    for (int i = 0, n = block.GetImports().size(); i < n; ++i)
    {
        auto& p = block.GetImports()[i];
        const auto f = "#" + p.var.type.name + "#";
        std::string t = p.var.type.name;
        auto itr = m_real_names.var2name.find(&p.var.type);
        if (itr != m_real_names.var2name.end()) {
            t = itr->second;
        }

        if (p.conns.empty()) 
        {
            auto& default_vals = block.GetDefaultInValues();
            if (!default_vals[i].val) {
                continue;
            }

            auto type = p.var.type.type;
            if (type == VarType::Invalid || type == VarType::Dynamic) {
                type = QueryRealType(&p.var.type);
            }
            t = EvalVar::VariantToString(default_vals[i], type);
        }
        cpputil::StringHelper::ReplaceAll(str, f, t);
    }
    for (int i = 0, n = block.GetExports().size(); i < n; ++i)
    {
        auto& p = block.GetExports()[i];
        const auto f = "#" + p.var.full_name + "#";
        std::string t = p.var.type.name;
        auto itr = m_real_names.var2name.find(&p.var.type);
        if (itr != m_real_names.var2name.end()) {
            t = itr->second;
        }

        if (p.conns.empty()) 
        {
            auto& default_vals = block.GetDefaultOutValues();
            if (!default_vals[i].val) {
                continue;
            }

            auto type = p.var.type.type;
            if (type == VarType::Invalid || type == VarType::Dynamic) {
                type = QueryRealType(&p.var.type);
            }
            t = EvalVar::VariantToString(default_vals[i], type);
        }
        cpputil::StringHelper::ReplaceAll(str, f, t);
    }
}

std::string Evaluator::ResolveFuncCall(const Block& block, size_t out_idx) const
{
    std::string ret;

    auto& outputs = block.GetExports();
    assert(out_idx >= 0 && out_idx < static_cast<int>(outputs.size()));
    auto& var = outputs[out_idx].var.type;
    if (var.type == VarType::Function) {
        return ret;
    }

    auto& funcs = block.GetFunctions();
    auto func_idx = block.GetCurrFuncIdx();
    if (func_idx < 0 || func_idx >= static_cast<int>(funcs.size())) {
        return ret;
    }

    auto func = funcs[func_idx];
    assert(func.first.type == VarType::Function);
    auto f_val = std::static_pointer_cast<FunctionVal>(func.first.val);

    auto& inputs = block.GetImports();
    auto& in_vars = block.GetDefaultInValues();

    auto& output = outputs[out_idx];
    ret += TypeToString(var.type) + " #" + output.var.type.name + "# = ";
    ret += func.first.name + "(";
    for (int i = 0, n = f_val->inputs.size(); i < n; ++i)
    {
        auto& param = f_val->inputs[i];

        int in_idx = -1;
        for (size_t j = 0, m = inputs.size(); j < m; ++j) {
            if (inputs[j].var.type.name == param.name) {
                in_idx = j;
                break;
            }
        }
        assert(in_idx >= 0);

        if (!inputs[in_idx].conns.empty()) {
            ret += "#" + param.name + "#";
        } else if (in_vars[in_idx].type != VarType::Invalid) {
            ret += EvalVar::VariantToString(in_vars[in_idx], in_vars[in_idx].type);
        } else {
            ret += EvalVar::GetDefaultValueString(param.type);
        }

        if (i != n - 1) {
            ret += ", ";
        }
    }
    ret += ");\n";

    return ret;
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
        assert(conn.idx >= 0 && conn.idx < static_cast<int>(exports.size()));
        ret = exports[conn.idx].var.type;
    }

    return ret;
}

}