#include "shadergraph/Block.h"
#include "shadergraph/BlockParser.h"
#include "shadergraph/ValueImpl.h"
#include "shadergraph/ParserProp.h"

namespace shadergraph
{

Block::Block(const std::string& str)
//    : m_code(str)
{
    Parser(str);
}

//std::string Block::GetMainCode() const
//{
//    std::string ret;
//    if (m_curr_func == -1) {
//        return ret;
//    }
//
//    assert(m_curr_func >= 0
//        && m_curr_func < static_cast<int>(m_funcs.size()));
//
//    auto func = m_funcs[m_curr_func];
//
//    auto f_val = std::static_pointer_cast<FunctionVal>(func.val);
//
//    ret += "    ";
//
//    if (f_val->output.type != VarType::Void)
//    {
//        ret += TypeToString(f_val->output.type);
//        ret += " " + func.name + "_ret = ";
//    }
//    ret += func.name + "(";
//    for (int i = 0, n = f_val->inputs.size(); i < n; ++i)
//    {
//        auto var = f_val->inputs[i];
//        ret += var.name;
//        if (i != n - 1) {
//            ret += ", ";
//        }
//    }
//    ret += ");\n";
//
//    return ret;
//}

void Block::Parser(const std::string& str)
{
    BlockParser parser(str);
    parser.Parse();

    m_uniforms = parser.GetUniforms();

    m_funcs = parser.GetFunctions();
    if (m_funcs.size() == 1)
    {
        m_curr_func = 0;
        SetupPorts();
    }
    else if (m_funcs.size() > 1)
    {
        for (size_t i = 0, n = m_funcs.size(); i < n; ++i)
        {
            auto f_val = std::static_pointer_cast<FunctionVal>(m_funcs[i].val);
            bool exp = false;
            for (auto& desc : f_val->desc) {
                if (desc->GetType() == ParserProp::Type::Export) {
                    exp = true;
                    break;
                }
            }
            if (exp) {
                m_curr_func = i;
                SetupPorts();
                break;
            }
        }
    }
}

void Block::SetupPorts()
{
    assert(m_curr_func >= 0 && m_curr_func < static_cast<int>(m_funcs.size()));
    auto func = m_funcs[m_curr_func];
    auto f_val = std::static_pointer_cast<FunctionVal>(func.val);
    m_imports.reserve(f_val->inputs.size());
    for (auto& input : f_val->inputs) {
        m_imports.push_back(PortFromVar(input));
    }

    for (auto& desc : f_val->desc)
    {
        if (desc->GetType() == ParserProp::Type::Function)
        {
            auto f_prop = std::static_pointer_cast<PropFunction>(desc);
            dag::Node<Variant>::PortVar port;
            port.full_name = f_prop->name;
            port.type.name = f_prop->name;
            port.type.type = VarType::Function;
            m_imports.push_back(port);
        }
    }

    if (f_val->output.type == VarType::Void) {
        m_exports.clear();
    } else {
        m_exports.push_back(PortFromVar(f_val->output));
    }
    m_exports.push_back(PortFromVar(func));
}

dag::Node<Variant>::PortVar
Block::PortFromVar(const Variant& var)
{
    dag::Node<Variant>::PortVar ret;
    ret.full_name = var.name;
    ret.type = var;
    return ret;
}

}