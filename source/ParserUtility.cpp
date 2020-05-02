#include "shadergraph/ParserUtility.h"

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

    default:
        assert(0);
    }
    return ret;
}

}