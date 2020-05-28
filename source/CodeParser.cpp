#include "shadergraph/CodeParser.h"
#include "shadergraph/ValueImpl.h"

#include <cslang/GenCode.h>

#include <sstream>

namespace
{

shadergraph::ValPtr create_value(shadergraph::VarType type)
{
    shadergraph::ValPtr val = nullptr;

    switch (type)
    {
    case shadergraph::VarType::Bool:
        val = std::make_shared<shadergraph::BoolVal>();
        break;
    case shadergraph::VarType::Bool2:
        val = std::make_shared<shadergraph::Bool2Val>();
        break;
    case shadergraph::VarType::Bool3:
        val = std::make_shared<shadergraph::Bool3Val>();
        break;
    case shadergraph::VarType::Bool4:
        val = std::make_shared<shadergraph::Bool4Val>();
        break;

    case shadergraph::VarType::Int:
        val = std::make_shared<shadergraph::IntVal>();
        break;
    case shadergraph::VarType::Int2:
        val = std::make_shared<shadergraph::Int2Val>();
        break;
    case shadergraph::VarType::Int3:
        val = std::make_shared<shadergraph::Int3Val>();
        break;
    case shadergraph::VarType::Int4:
        val = std::make_shared<shadergraph::Int4Val>();
        break;

    case shadergraph::VarType::Float:
        val = std::make_shared<shadergraph::FloatVal>();
        break;
    case shadergraph::VarType::Float2:
        val = std::make_shared<shadergraph::Float2Val>();
        break;
    case shadergraph::VarType::Float3:
        val = std::make_shared<shadergraph::Float3Val>();
        break;
    case shadergraph::VarType::Float4:
        val = std::make_shared<shadergraph::Float4Val>();
        break;

    case shadergraph::VarType::Matrix2:
        val = std::make_shared<shadergraph::Matrix2Val>();
        break;
    case shadergraph::VarType::Matrix3:
        val = std::make_shared<shadergraph::Matrix3Val>();
        break;
    case shadergraph::VarType::Matrix4:
        val = std::make_shared<shadergraph::Matrix4Val>();
        break;
    }

    return val;
}

shadergraph::VarType trans_var_type(int cs_type)
{
    switch (cs_type)
    {
    case cslang::TK_VOID:
        return shadergraph::VarType::Void;
    case cslang::TK_BOOL:
        return shadergraph::VarType::Bool;
    case cslang::TK_BOOL2:
        return shadergraph::VarType::Bool2;
    case cslang::TK_BOOL3:
        return shadergraph::VarType::Bool3;
    case cslang::TK_BOOL4:
        return shadergraph::VarType::Bool4;
    case cslang::TK_UINT:
        return shadergraph::VarType::UInt;
    case cslang::TK_INT:
        return shadergraph::VarType::Int;
    case cslang::TK_INT2:
        return shadergraph::VarType::Int2;
    case cslang::TK_INT3:
        return shadergraph::VarType::Int3;
    case cslang::TK_INT4:
        return shadergraph::VarType::Int4;
    case cslang::TK_FLOAT:
        return shadergraph::VarType::Float;
    case cslang::TK_FLOAT2:
        return shadergraph::VarType::Float2;
    case cslang::TK_FLOAT3:
        return shadergraph::VarType::Float3;
    case cslang::TK_FLOAT4:
        return shadergraph::VarType::Float4;
    case cslang::TK_MATRIX2:
        return shadergraph::VarType::Matrix2;
    case cslang::TK_MATRIX3:
        return shadergraph::VarType::Matrix3;
    case cslang::TK_MATRIX4:
        return shadergraph::VarType::Matrix4;
    case cslang::TK_SAMPLER_2D:
        return shadergraph::VarType::Sampler2D;
    case cslang::TK_SAMPLER_CUBE:
        return shadergraph::VarType::SamplerCube;
    default:
        assert(0);
        return shadergraph::VarType::Invalid;
    }
}

}

namespace shadergraph
{

Variant CodeParser::ToVariant(const cslang::ast::DeclarationNodePtr& var)
{
    bool is_uniform = false;
    if (var->specs->tyQuals)
    {
        auto storage = std::static_pointer_cast<cslang::ast::TokenNode>(var->specs->tyQuals)->token;
        if (storage == cslang::TK_UNIFORM) {
            is_uniform = true;
        }
    }

    auto type = trans_var_type(std::static_pointer_cast<cslang::ast::TokenNode>(var->specs->tySpecs)->token);

    std::string name;
    auto initDec = std::static_pointer_cast<cslang::ast::InitDeclaratorNode>(var->initDecs);
    switch (initDec->dec->kind)
    {
    case cslang::NK_NameDeclarator:
        name = initDec->dec->id;
        break;

    case cslang::NK_ArrayDeclarator:
    {
        auto decl = std::static_pointer_cast<cslang::ast::ArrayDeclaratorNode>(initDec->dec);
        name = decl->dec->id;
    }
        break;

    //case NK_FunctionDeclarator:
    //    break;

    //case NK_PointerDeclarator:
    //    break;

    default:
        assert(0);
    }

    Variant ret;
    ret.name = name;
    auto val = create_value(type);
    if (is_uniform)
    {
        ret.type = VarType::Uniform;

        auto unif = std::make_shared<UniformVal>();
        unif->var.name = name;
        //unif->desc = m_desc;
        unif->var.type = type;
        unif->var.val  = val;
        ret.val = unif;
    }
    else
    {
        ret.type = type;
        ret.val  = val;
    }

    return ret;
}

Variant CodeParser::ToVariant(const cslang::ast::FunctionNodePtr& func)
{
    Variant ret;
    ret.name = func->fdec->dec->id;
    ret.type = VarType::Function;

    auto f_var = std::make_shared<FunctionVal>();
    auto ret_token = std::static_pointer_cast<cslang::ast::TokenNode>(func->specs->tySpecs)->token;
    f_var->output.type = trans_var_type(ret_token);
    f_var->output.name = "ret";

    // params
    if (func->fdec->paramTyList)
    {
        auto p = func->fdec->paramTyList->paramDecls;
        while (p)
        {
            Variant var;
            auto p_decl = std::static_pointer_cast<cslang::ast::ParameterDeclarationNode>(p);
            var.type = trans_var_type(std::static_pointer_cast<cslang::ast::TokenNode>(p_decl->specs->tySpecs)->token);
            var.name = p_decl->dec->id;
            f_var->inputs.push_back(var);
            p = p->next;
        }
    }

    std::stringstream ss;
    cslang::GenFunction(ss, func);
    f_var->code = ss.str();

    ret.val = f_var;

    return ret;
}

}