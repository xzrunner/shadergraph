#include "shadergraph/Block.h"
#include "shadergraph/BlockParser.h"
#include "shadergraph/ValueImpl.h"
#include "shadergraph/ParserProp.h"

namespace shadergraph
{

Block::Block(const std::string& str)
{
    if (!str.empty()) {
        Parser(str);
    }
}

void Block::SetupPorts(const std::vector<Variant>& inputs,
                       const std::vector<Variant>& outputs)
{
    m_imports.resize(inputs.size());
    for (size_t i = 0, n = inputs.size(); i < n; ++i) {
        m_imports[i].var.type = inputs[i];
        m_imports[i].var.full_name = inputs[i].name;
    }

    m_exports.resize(outputs.size());
    for (size_t i = 0, n = outputs.size(); i < n; ++i) {
        m_exports[i].var.type = outputs[i];
        m_exports[i].var.full_name = outputs[i].name;
    }
}

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

    // setup uniform default value
    for (auto& u : m_uniforms)
    {
        auto u_val = std::static_pointer_cast<UniformVal>(u.val);
        for (auto& d : u_val->desc) {
            if (d->GetType() == ParserProp::Type::Default) {
                u_val->var.val = std::static_pointer_cast<PropDefault>(d)->val;
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

    for (auto& u : m_uniforms)
    {
        dag::Node<Variant>::PortVar port;

        port.full_name = u.name;
        port.type.name = u.name;

        assert(u.type == VarType::Uniform);
        auto u_val = std::static_pointer_cast<UniformVal>(u.val);
        port.type.type = u_val->var.type;
        port.type.val  = u_val->var.val;

        m_imports.push_back(port);
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