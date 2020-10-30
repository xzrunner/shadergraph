#include "shadergraph/EvalVar.h"
#include "shadergraph/Variant.h"
#include "shadergraph/ValueImpl.h"

#include <cpputil/StringHelper.h>

#include <assert.h>

namespace shadergraph
{

std::shared_ptr<Value> 
EvalVar::ValueTrans(const Variant& var, VarType type)
{
    if (var.type == type) {
        return var.val;
    }

    std::shared_ptr<Value> ret = nullptr;
    switch (var.type)
    {
    case VarType::Int:
    {
        int i = std::static_pointer_cast<IntVal>(var.val)->x;
        float f = static_cast<float>(i);
        switch (type)
        {
        case VarType::Int2:
            ret = std::make_shared<Int2Val>(i, i);
            break;
        case VarType::Int3:
            ret = std::make_shared<Int3Val>(i, i, i);
            break;
        case VarType::Int4:
            ret = std::make_shared<Int4Val>(i, i, i, i);
            break;
        case VarType::Float:
            ret = std::make_shared<FloatVal>(f);
            break;
        case VarType::Float2:
            ret = std::make_shared<Float2Val>(f, f);
            break;
        case VarType::Float3:
            ret = std::make_shared<Float3Val>(f, f, f);
            break;
        case VarType::Float4:
            ret = std::make_shared<Float4Val>(f, f, f, f);
            break;
        default:
            assert(0);
        }
    }
        break;
    case VarType::Float:
    {
        float f = std::static_pointer_cast<FloatVal>(var.val)->x;
        switch (type)
        {
        case VarType::Float2:
            ret = std::make_shared<Float2Val>(f, f);
            break;
        case VarType::Float3:
            ret = std::make_shared<Float3Val>(f, f, f);
            break;
        case VarType::Float4:
            ret = std::make_shared<Float4Val>(f, f, f, f);
            break;
        default:
            assert(0);
        }
    }
        break;
    default:
        assert(0);
    }

    assert(ret);
    return ret;
}

std::string EvalVar::VariantToString(const Variant& var, VarType type)
{
    if (var.type == VarType::String) {
        return std::static_pointer_cast<StringVal>(var.val)->str;
    }

    auto val = ValueTrans(var, type);
    switch (type)
    {
    //case VarType::Bool:
    //    return "false";
    //case VarType::Bool2:
    //    return "bvec2(false, false)";
    //case VarType::Bool3:
    //    return "bvec3(false, false, false)";
    //case VarType::Bool4:
    //    return "bvec4(false, false, false, false)";
    case VarType::UInt:
    case VarType::Int:
        return std::to_string(std::static_pointer_cast<IntVal>(val)->x).c_str();
    case VarType::Int2:
    {
        auto v = std::static_pointer_cast<Int2Val>(val);
        return cpputil::StringHelper::Format("ivec2(%s, %s)",
            std::to_string(v->xy[0]), std::to_string(v->xy[1]).c_str());
    }
    case VarType::Int3:
    {
        auto v = std::static_pointer_cast<Int3Val>(val);
        return cpputil::StringHelper::Format("ivec3(%s, %s, %s)",
            std::to_string(v->xyz[0]).c_str(), std::to_string(v->xyz[1]).c_str(), std::to_string(v->xyz[2]).c_str());
    }
    case VarType::Int4:
    {
        auto v = std::static_pointer_cast<Int4Val>(val);
        return cpputil::StringHelper::Format("ivec4(%s, %s, %s, %s)",
            std::to_string(v->xyzw[0]).c_str(), std::to_string(v->xyzw[1]).c_str(), std::to_string(v->xyzw[2]).c_str(), std::to_string(v->xyzw[3]).c_str());
    }
    case VarType::Float:
        return std::to_string(std::static_pointer_cast<FloatVal>(val)->x).c_str();
    case VarType::Float2:
    {
        auto v = std::static_pointer_cast<Float2Val>(val);
        return cpputil::StringHelper::Format("vec2(%s, %s)",
            std::to_string(v->xy[0]).c_str(), std::to_string(v->xy[1]).c_str());
    }
    case VarType::Float3:
    {
        auto v = std::static_pointer_cast<Float3Val>(val);
        return cpputil::StringHelper::Format("vec3(%s, %s, %s)",
            std::to_string(v->xyz[0]).c_str(), std::to_string(v->xyz[1]).c_str(), std::to_string(v->xyz[2]).c_str());
    }
    case VarType::Float4:
    {
        auto v = std::static_pointer_cast<Float4Val>(val);
        return cpputil::StringHelper::Format("vec4(%s, %s, %s, %s)",
            std::to_string(v->xyzw[0]).c_str(), std::to_string(v->xyzw[1]).c_str(), std::to_string(v->xyzw[2]).c_str(), std::to_string(v->xyzw[3]).c_str());
    }
    default:
        return "";
    }
}

std::string EvalVar::GetDefaultValueString(VarType type)
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