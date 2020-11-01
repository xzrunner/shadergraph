#pragma once

#include "shadergraph/Block.h"

#include <SM_Vector.h>

namespace shadergraph
{
namespace block
{

class NormalBlend : public Block
{
public:
    enum class Mode
    {
        Default,
        Reoriented,
        Linear,
        Overlay,
        PartialDerivative,
        Whiteout,
        UDN,
        Unity,
    };

public:
    NormalBlend()
    {
        SetupPorts({
            { VarType::Float3, "a" },
            { VarType::Float3, "b" },
        }, {
            { VarType::Float3, "_out" },
        });
    }

    virtual std::string GetHeader(const Evaluator& eval) const
    {
        return R"(
// https://docs.unity3d.com/Packages/com.unity.shadergraph@9.0/manual/Normal-Blend-Node.html
vec3 normal_blend(vec3 a, vec3 b)
{
    return normalize(vec3(a.rg + b.rg, a.b * b.b));
}
vec3 normal_blend_reoriented(vec3 a, vec3 b)
{
    vec3 t = a.xyz + vec3(0.0, 0.0, 1.0);
    vec3 u = b.xyz * vec3(-1.0, -1.0, 1.0);
    return (t / t.z) * dot(t, u) - u;
}

// https://blog.selfshadow.com/publications/blending-in-detail/
vec3 normal_blend_linear(vec3 a, vec3 b)
{
	return normalize(a + b);
}
vec3 normal_blend_overlay(vec3 a, vec3 b)
{
    vec3 n1 = a * 0.5 + 0.5;
    vec3 n2 = b * 0.5 + 0.5;
    float x = n1.x < 0.5 ? 2*n1.x*n2.x : 1 - 2*(1 - n1.x)*(1 - n2.x);
    float y = n1.y < 0.5 ? 2*n1.y*n2.y : 1 - 2*(1 - n1.y)*(1 - n2.y);
    float z = n1.z < 0.5 ? 2*n1.z*n2.z : 1 - 2*(1 - n1.z)*(1 - n2.z);
    return normalize(vec3(x, y, z));
}
vec3 normal_blend_partial_derivative(vec3 a, vec3 b)
{
	return normalize(vec3(a.xz*b.y + b.xz*a.y, a.y*b.y)).xzy;
}
vec3 normal_blend_whiteout(vec3 a, vec3 b)
{
	return normalize(vec3(a.x + b.x, a.y * b.y, a.z + b.z));
}
vec3 normal_blend_udn(vec3 a, vec3 b)
{
	return normalize(vec3(a.x + b.x, a.y, a.z + b.z));
}
vec3 normal_blend_unity(vec3 a, vec3 b)
{
	mat3 nBasis = mat3(
	    vec3(a.z, a.y, -a.x), // +90 degree rotation around y axis
	    vec3(a.x, a.z, -a.y), // -90 degree rotation around x axis
	    vec3(a.x, a.y,  a.z));
	vec3 r = normalize(b.x*nBasis[0] + b.y*nBasis[1] + b.z*nBasis[2]);
	return r.xzy;
}

)";
    }

    virtual std::string GetBody(const Evaluator& eval) const override
    {
        std::string func;
        switch (m_mode)
        {
        case Mode::Default:
            func = "normal_blend";
            break;
        case Mode::Reoriented:
            func = "normal_blend_reoriented";
            break;
        case Mode::Linear:
            func = "normal_blend_linear";
            break;
        case Mode::Overlay:
            func = "normal_blend_overlay";
            break;
        case Mode::PartialDerivative:
            func = "normal_blend_partial_derivative";
            break;
        case Mode::Whiteout:
            func = "normal_blend_whiteout";
            break;
        case Mode::UDN:
            func = "normal_blend_udn";
            break;
        case Mode::Unity:
            func = "normal_blend_unity";
            break;
        default:
            assert(0);
        }
        return "vec3 #_out# = " + func + "(#a#, #b#);";
    }

    RTTR_ENABLE(Block)

#define PARM_FILEPATH "shadergraph/block/NormalBlend.parm.h"
#include <dag/node_parms_gen.h>
#undef PARM_FILEPATH

}; // NormalBlend

}
}