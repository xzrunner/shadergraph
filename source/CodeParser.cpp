#include "shadergraph/CodeParser.h"
#include "shadergraph/ValueImpl.h"
#include "shadergraph/CommentParser.h"

#include <cslang/GenCode.h>
#include <cslang/EvalAST.h>

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

shadergraph::VarType trans_src_type(int cs_type)
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

    case cslang::TK_STRUCT:
        return shadergraph::VarType::Struct;

    default:
        assert(0);
        return shadergraph::VarType::Invalid;
    }
}

}

namespace shadergraph
{
    
Variant CodeParser::ToVariant(const CommentParser& desc,
                              const cslang::ast::DeclarationNodePtr& src)
{
    VarType type = VarType::Invalid;
    std::string name;
    ValPtr val = nullptr;

    bool is_uniform = false;

    switch (src->specs->tySpecs->kind)
    {
    case cslang::NK_EnumSpecifier:
    case cslang::NK_StructSpecifier:
    case cslang::NK_UnionSpecifier:
        return Variant();

    case cslang::NK_UniformSpecifier:
    {
        auto struct_node = std::static_pointer_cast<cslang::ast::StructSpecifierNode>(src->specs->tySpecs);

        type = VarType::Struct;

        name = struct_node->id;

        auto st_val = std::make_shared<StructVal>();
        st_val->type_name = name;

        auto member = struct_node->next;
        while (member)
        {
            Variant child;

            child.type = VarType::Uniform;

            auto unif = std::make_shared<UniformVal>();

            assert(member->kind == cslang::NK_StructDeclaration);
            auto st_decl = std::static_pointer_cast<cslang::ast::StructDeclarationNode>(member);

            unif->var.type = trans_src_type(std::static_pointer_cast<cslang::ast::TokenNode>(st_decl->specs->tySpecs)->token);

            assert(st_decl->stDecs->kind == cslang::NK_StructDeclarator);
            unif->var.name = std::static_pointer_cast<cslang::ast::StructDeclaratorNode>(st_decl->stDecs)->dec->id;
            child.name = unif->var.name;

            child.val = unif;

            st_val->children.push_back(child);

            member = member->next;
        }
        val = st_val;
    }
        break;

    case cslang::NK_TypedefName:
        return Variant();

    case cslang::NK_NameDeclarator:
    {
        auto decl_node = std::static_pointer_cast<cslang::ast::DeclaratorNode>(src->specs->tySpecs);

        Variant ret;
        ret.type = VarType::Struct;
        ret.name = decl_node->id;
        return ret;
    }
        break;
    }

    TypeQualifier qualifier = TypeQualifier::Null;
    if (src->specs->tyQuals)
    {
        auto storage = std::static_pointer_cast<cslang::ast::TokenNode>(src->specs->tyQuals)->token;
        switch (storage)
        {
        case cslang::TK_UNIFORM:
            is_uniform = true;
            break;
        case cslang::TK_CONST:
            qualifier = TypeQualifier::Const;
            break;
        case cslang::TK_IN:
            qualifier = TypeQualifier::Input;
            break;
        case cslang::TK_OUT:
            qualifier = TypeQualifier::Output;
            break;
        case cslang::TK_ATTRIBUTE:
            qualifier = TypeQualifier::Attribute;
            break;
        case cslang::TK_VOLATILE:
            qualifier = TypeQualifier::Volatile;
            break;
        case cslang::TK_VARYING:
            qualifier = TypeQualifier::Varying;
            break;
        }
    }
    
    switch (src->specs->tySpecs->kind)
    {
    case cslang::NK_Token:
        type = trans_src_type(std::static_pointer_cast<cslang::ast::TokenNode>(src->specs->tySpecs)->token);
        break;
    }

    if (src->initDecs)
    {
        auto initDec = std::static_pointer_cast<cslang::ast::InitDeclaratorNode>(src->initDecs);
        switch (initDec->dec->kind)
        {
        case cslang::NK_NameDeclarator:
            name = initDec->dec->id;
            break;

        case cslang::NK_ArrayDeclarator:
        {
            auto decl = std::static_pointer_cast<cslang::ast::ArrayDeclaratorNode>(initDec->dec);
            name = decl->dec->id;

            auto array_v = std::make_shared<ArrayVal>();
            array_v->type = type;
            array_v->items.resize(decl->expr->val.i[0]);

            type = VarType::Array;
            val = array_v;
        }
            break;

        //case NK_FunctionDeclarator:
        //    break;

        //case NK_PointerDeclarator:
        //    break;

        default:
            assert(0);
        }

        if (initDec->init)
        {
            if (!val) {
                val = create_value(type);
            }
            assert(initDec->init->lbrace == 0);
            auto init_v = cslang::EvalExpression(initDec->init->expr);
            switch (type)
            {
            case VarType::Int:
            {
                assert(init_v.type == cslang::VarType::Int);
                std::static_pointer_cast<IntVal>(val)->x = init_v.i;
            }
                break;

            case VarType::Float:
            {
                switch (init_v.type)
                {
                case cslang::VarType::Int:
                    std::static_pointer_cast<FloatVal>(val)->x = static_cast<float>(init_v.i);
                    break;

                case cslang::VarType::Float:
                    std::static_pointer_cast<FloatVal>(val)->x = init_v.f;
                    break;

                case cslang::VarType::Double:
                    std::static_pointer_cast<FloatVal>(val)->x = static_cast<float>(init_v.d);
                    break;

                default:
                    assert(0);
                }
            }
                break;

            default:
                assert(0);
            }
        }
    }

    Variant ret;
    ret.name = name;
    ret.qualifier = qualifier;
    if (is_uniform)
    {
        ret.type = VarType::Uniform;

        auto unif = std::make_shared<UniformVal>();
        unif->var.name = name;
        unif->desc     = desc.QueryProps(name);
        unif->var.type = type;
        unif->var.val  = val;
        for (auto& d : unif->desc) {
            if (d->GetType() == ParserProp::Type::Default) {
                unif->var.val = std::static_pointer_cast<PropDefault>(d)->val;
                break;
            }
        }
        ret.val = unif;
    }
    else
    {
        ret.type = type;
        ret.val  = val;
    }

    return ret;
}

Variant CodeParser::ToVariant(const CommentParser& desc,
                              const cslang::ast::FunctionNodePtr& func)
{
    Variant ret;
    ret.name = func->fdec->dec->id;
    ret.type = VarType::Function;

    auto f_src = std::make_shared<FunctionVal>();
    auto ret_token = std::static_pointer_cast<cslang::ast::TokenNode>(func->specs->tySpecs)->token;
    f_src->output.type = trans_src_type(ret_token);
    f_src->output.name = "ret";
    f_src->desc = desc.QueryProps(ret.name);

    // params
    if (func->fdec->paramTyList)
    {
        auto p = func->fdec->paramTyList->paramDecls;
        while (p)
        {
            Variant src;
            auto p_decl = std::static_pointer_cast<cslang::ast::ParameterDeclarationNode>(p);
            src.type = trans_src_type(std::static_pointer_cast<cslang::ast::TokenNode>(p_decl->specs->tySpecs)->token);
            src.name = p_decl->dec->id;
            f_src->inputs.push_back(src);
            p = p->next;
        }
    }

    std::stringstream ss;
    cslang::GenFunction(ss, func);
    f_src->code = ss.str();

    ret.val = f_src;

    return ret;
}

}