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

    m_default_in_vals.resize(inputs.size());
    m_default_out_vals.resize(outputs.size());
}

void Block::Parser(const std::string& str)
{
    // reset
    m_funcs.clear();
    m_curr_func = -1;
    m_global_vars.clear();
    m_structs.clear();

    CommentParser parser(str);
    parser.Parse();

    cslang::Parser cs_parser(str.c_str());
    auto root = cslang::ast::DeclarationParser::ParseTranslationUnit(cs_parser);
    if (!root) {
        return;
    }

    m_curr_func = -1;

    std::set<cslang::NodePtr> unique;

	auto p = root->extDecls;
	while (p)
	{
        if (unique.find(p) != unique.end()) {
            p = p->next;
            continue;
        }

        switch (p->kind)
        {
        case cslang::NK_Declaration:
        {
            unique.insert(p);
            auto src = std::static_pointer_cast<cslang::ast::DeclarationNode>(p);
            auto dst = CodeParser::ToVariant(parser, src);
            if (dst.type != VarType::Invalid) 
            {
                // ubo
                if (dst.type == VarType::Struct)
                {
                    auto st_val = std::static_pointer_cast<StructVal>(dst.val);
                    std::copy(st_val->children.begin(), st_val->children.end(), std::back_inserter(m_global_vars));
                }
                else
                {
                    m_global_vars.push_back(dst);
                }
            }
        }
            break;
        case cslang::NK_Function:
        {
            unique.insert(p);

            auto src = std::static_pointer_cast<cslang::ast::FunctionNode>(p);
            auto dst = CodeParser::ToVariant(parser, src);

            bool is_export = false;
            auto f_dst = std::static_pointer_cast<FunctionVal>(dst.val);
            for (auto& prop : f_dst->desc)
            {
                auto type = prop->GetType();
                if (type == ParserProp::Type::Export) {
                    is_export = true;
                }
            }

            m_funcs.push_back({ dst, is_export });
        }
            break;
        }
		p = p->next;
	}

    // set curr_func idx
    if (m_funcs.size() == 1) {
        m_curr_func = 0;
    } else {
        for (int i = 0, n = m_funcs.size(); i < n; ++i) {
            if (m_funcs[i].second) {
                m_curr_func = i;
                break;
            }
        }
    }
    // fixme
    if (m_curr_func < 0) {
        m_curr_func = 0;
    }

    SetupPorts();
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

    m_default_in_vals.clear();
    m_default_out_vals.clear();

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
    m_default_in_vals.resize(m_imports.size());

    if (m_funcs.empty()) {
        return;
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

            auto f_val = std::make_shared<FunctionVal>();
            f_val->output.type = f_prop->output;
            f_val->inputs.resize(f_prop->inputs.size());
            for (int i = 0, n = f_prop->inputs.size(); i < n; ++i) {
                f_val->inputs[i].type = f_prop->inputs[i].first;
                f_val->inputs[i].name = f_prop->inputs[i].second;
            }
            port.type.val = f_val;

            m_imports.push_back(port);
        }
    }

    m_default_in_vals.resize(m_imports.size());
    m_default_out_vals.resize(m_exports.size());

    if (f_val->output.type == VarType::Void) {
        m_exports.clear();
    } else {
        m_exports.push_back(PortFromVar(f_val->output));
    }
    m_exports.push_back(PortFromVar(func.first));

    m_default_out_vals.resize(m_exports.size());
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