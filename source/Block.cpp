#include "shadergraph/Block.h"
#include "shadergraph/ValueImpl.h"
#include "shadergraph/ParserProp.h"
#include "shadergraph/CommentParser.h"
#include "shadergraph/CodeParser.h"

#include <cslang/Parser.h>

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
    std::vector<std::shared_ptr<ParserProp>> props;
    CommentParser parser(str);
    parser.Parse(props);

    cslang::Parser cs_parser(str.c_str());
    auto root = cslang::ast::DeclarationParser::ParseTranslationUnit(cs_parser);
    if (!root) {
        return;
    }

	auto p = root->extDecls;
	while (p)
	{
        switch (p->kind)
        {
        case cslang::NK_Declaration:
        {
            auto var = std::static_pointer_cast<cslang::ast::DeclarationNode>(p);
            m_global_vars.push_back(CodeParser::ToVariant(var));
        }
            break;
        case cslang::NK_Function:
        {
            auto var = std::static_pointer_cast<cslang::ast::FunctionNode>(p);
            m_funcs.push_back({ CodeParser::ToVariant(var), false });
        }
            break;
        }
		p = p->next;
	}

    SetupCurrFunc(props);
    SetupPorts();
}

void Block::SetupCurrFunc(const std::vector<std::shared_ptr<ParserProp>>& props)
{
    if (m_funcs.empty()) {
        return;
    }

    if (m_funcs.size() == 1) {
        m_curr_func = 0;
        return;
    }

    std::vector<std::string> exports;
    for (auto& prop : props)
    {
        if (prop->GetType() == ParserProp::Type::Export)
        {
            auto exp = std::static_pointer_cast<PropExport>(prop);
            exports.push_back(exp->func_name);
        }
    }

    m_curr_func = -1;
    for (int i = 0, n = m_funcs.size(); i < n; ++i)
    {
        bool is_export = false;
        for (auto& name : exports) {
            if (m_funcs[i].first.name == name) {
                is_export = true;
                break;
            }
        }
        if (is_export && m_curr_func < 0) {
            m_curr_func = i;
        }
        m_funcs[i].second = is_export;
    }
}

void Block::SetupPorts()
{
    // clear connections
    for (int i = 0, n = m_imports.size(); i < n; ++i) {
        for (auto& c : m_imports[i].conns) {
            dag::disconnect<Variant>(*c.node.lock(), c.idx, *this, i);
        }
    }
    for (int i = 0, n = m_exports.size(); i < n; ++i) {
        for (auto& c : m_exports[i].conns) {
            dag::disconnect<Variant>(*this, i, *c.node.lock(), c.idx);
        }
    }

    m_imports.clear();
    m_exports.clear();

    for (auto& var : m_global_vars)
    {
        dag::Node<Variant>::PortVar port;

        port.full_name = var.name;
        port.type.name = var.name;

        if (var.type == VarType::Uniform)
        {
            auto u_val = std::static_pointer_cast<UniformVal>(var.val);
            port.type.type = u_val->var.type;
            port.type.val  = u_val->var.val;
        }
        else
        {
            port.type.type = var.type;
            port.type.val  = var.val;
        }

        m_imports.push_back(port);
    }

    assert(m_curr_func >= 0 && m_curr_func < static_cast<int>(m_funcs.size()));
    auto func = m_funcs[m_curr_func];
    auto f_val = std::static_pointer_cast<FunctionVal>(func.first.val);
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
    m_exports.push_back(PortFromVar(func.first));
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