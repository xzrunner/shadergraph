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