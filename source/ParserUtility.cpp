#include "shadergraph/ParserUtility.h"

#include <glsl-parser/ast.h>
#include <glsl-parser/lexer.h>

#include <assert.h>

namespace shadergraph
{

VarType StringToType(const std::string& token)
{
    VarType ret = VarType::Invalid;

    if (token == "void") {
        ret = VarType::Void;
    } else if (token == "bool") {
        ret = VarType::Bool;
    } else if (token == "bvec2") {
        ret = VarType::Bool2;
    } else if (token == "bvec3") {
        ret = VarType::Bool3;
    } else if (token == "bvec4") {
        ret = VarType::Bool4;
    } else if (token == "uint") {
        ret = VarType::UInt;
    } else if (token == "int") {
        ret = VarType::Int;
    } else if (token == "ivec2") {
        ret = VarType::Int2;
    } else if (token == "ivec3") {
        ret = VarType::Int3;
    } else if (token == "ivec4") {
        ret = VarType::Int4;
    } else if (token == "float") {
        ret = VarType::Float;
    } else if (token == "vec2") {
        ret = VarType::Float2;
    } else if (token == "vec3") {
        ret = VarType::Float3;
    } else if (token == "vec4") {
        ret = VarType::Float4;
    } else if (token == "mat2") {
        ret = VarType::Matrix2;
    } else if (token == "mat3") {
        ret = VarType::Matrix3;
    } else if (token == "mat4") {
        ret = VarType::Matrix4;
    } else if (token == "sampler2D") {
        ret = VarType::Sampler2D;
    } else if (token == "samplerCube") {
        ret = VarType::SamplerCube;
    } else {
        assert(0);
    }

    return ret;
}

std::string TypeToString(VarType type)
{
    std::string ret;
    switch (type)
    {
    case VarType::Void:
        ret = "void";
        break;

    case VarType::Bool:
        ret = "bool";
        break;
    case VarType::Bool2:
        ret = "bvec2";
        break;
    case VarType::Bool3:
        ret = "bvec3";
        break;
    case VarType::Bool4:
        ret = "bvec4";
        break;

    case VarType::UInt:
        ret = "uint";
        break;

    case VarType::Int:
        ret = "int";
        break;
    case VarType::Int2:
        ret = "ivec2";
        break;
    case VarType::Int3:
        ret = "ivec3";
        break;
    case VarType::Int4:
        ret = "ivec4";
        break;

    case VarType::Float:
        ret = "float";
        break;
    case VarType::Float2:
        ret = "vec2";
        break;
    case VarType::Float3:
        ret = "vec3";
        break;
    case VarType::Float4:
        ret = "vec4";
        break;

    case VarType::Matrix2:
        ret = "mat2";
        break;
    case VarType::Matrix3:
        ret = "mat3";
        break;
    case VarType::Matrix4:
        ret = "mat4";
        break;

    case VarType::Sampler2D:
        ret = "sampler2D";
        break;
    case VarType::SamplerCube:
        ret = "samplerCube";
        break;

    case VarType::Array:
        // todo
        break;

    default:
        assert(0);
    }
    return ret;
}

VarType TypeAstToVar(const glsl::astType* type)
{
    if (!type->builtin) {
        return VarType::Struct;
    }

    auto bi_type = (glsl::astBuiltin*)type;
    switch (bi_type->type)
    {
    case glsl::kKeyword_void:
        return VarType::Void;
    case glsl::kKeyword_bool:
        return VarType::Bool;
    case glsl::kKeyword_bvec2:
        return VarType::Bool2;
    case glsl::kKeyword_bvec3:
        return VarType::Bool3;
    case glsl::kKeyword_bvec4:
        return VarType::Bool4;
    case glsl::kKeyword_uint:
        return VarType::UInt;
    case glsl::kKeyword_int:
        return VarType::Int;
    case glsl::kKeyword_ivec2:
        return VarType::Int2;
    case glsl::kKeyword_ivec3:
        return VarType::Int3;
    case glsl::kKeyword_ivec4:
        return VarType::Int4;
    case glsl::kKeyword_float:
        return VarType::Float;
    case glsl::kKeyword_vec2:
        return VarType::Float2;
    case glsl::kKeyword_vec3:
        return VarType::Float3;
    case glsl::kKeyword_vec4:
        return VarType::Float4;
    case glsl::kKeyword_mat2:
        return VarType::Matrix2;
    case glsl::kKeyword_mat3:
        return VarType::Matrix3;
    case glsl::kKeyword_mat4:
        return VarType::Matrix4;
    case glsl::kKeyword_sampler2D:
        return VarType::Sampler2D;
    case glsl::kKeyword_samplerCube:
        return VarType::SamplerCube;
    default:
        assert(0);
        return VarType::Invalid;
    }
}

}