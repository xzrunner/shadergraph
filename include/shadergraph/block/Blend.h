#pragma once

#include "shadergraph/Block.h"

#include <SM_Vector.h>

namespace shadergraph
{
namespace block
{

class Blend : public Block
{
public:
    enum class Mode
    {
        Burn,
        Darken,
        Difference,
        Dodge,
        Divide,
        Exclusion,
        HardLight,
        HardMix,
        Lighten,
        LinearBurn,
        LinearDodge,
        LinearLight,
        LinearLightAddSub,
        Multiply,
        Negation,
        Overlay,
        PinLight,
        Screen,
        SoftLight,
        Subtract,
        VividLight,
        Overwrite
    };

public:
    Blend()
    {
        SetupPorts({
            { VarType::Float3, "base" },
            { VarType::Float3, "blend" },
            { VarType::Float,  "opacity" },
        }, {
            { VarType::Float3, "_out" },
        });
    }

    virtual std::string GetHeader(const Evaluator& eval) const
    {
        return R"(

// https://docs.unity3d.com/Packages/com.unity.shadergraph@9.0/manual/Blend-Node.html

// Burn
vec3 blend_burn(vec3 base, vec3 blend, float opacity)
{
    vec3 ret = 1.0 - (1.0 - blend)/base;
    return mix(base, ret, opacity);
}

// Darken
vec3 blend_darken(vec3 base, vec3 blend, float opacity)
{
    vec3 ret = min(blend, base);
    return mix(base, ret, opacity);
}

// Difference
vec3 blend_difference(vec3 base, vec3 blend, float opacity)
{
    vec3 ret = abs(blend - base);
    return mix(base, ret, opacity);
}

// Dodge
vec3 blend_dodge(vec3 base, vec3 blend, float opacity)
{
    vec3 ret = base / (1.0 - blend);
    return mix(base, ret, opacity);
}

// Divide
vec3 blend_divide(vec3 base, vec3 blend, float opacity)
{
    vec3 ret = base / (blend + 0.000000000001);
    return mix(base, ret, opacity);
}

// Exclusion
vec3 blend_exclusion(vec3 base, vec3 blend, float opacity)
{
    vec3 ret = blend + base - (2.0 * blend * base);
    return mix(base, ret, opacity);
}

// HardLight
vec3 blend_hard_light(vec3 base, vec3 blend, float opacity)
{
    vec3 result1 = 1.0 - 2.0 * (1.0 - base) * (1.0 - blend);
    vec3 result2 = 2.0 * base * blend;
    vec3 zeroOrOne = step(blend, vec3(0.5));
    vec3 ret = result2 * zeroOrOne + (1 - zeroOrOne) * result1;
    return mix(base, ret, opacity);
}

// HardMix
vec3 blend_hard_mix(vec3 base, vec3 blend, float opacity)
{
    vec3 ret = step(1 - base, blend);
    return mix(base, ret, opacity);
}

// Lighten
vec3 blend_lighten(vec3 base, vec3 blend, float opacity)
{
    vec3 ret = max(blend, base);
    return mix(base, ret, opacity);
}

// LinearBurn
vec3 blend_linear_burn(vec3 base, vec3 blend, float opacity)
{
    vec3 ret = base + blend - 1.0;
    return mix(base, ret, opacity);
}

// LinearDodge
vec3 blend_linear_dodge(vec3 base, vec3 blend, float opacity)
{
    vec3 ret = base + blend;
    return mix(base, ret, opacity);
}

// LinearLight
vec3 blend_linear_light(vec3 base, vec3 blend, float opacity)
{
    vec3 ret;
    ret.x = blend.x < 0.5 ? max(base.x + (2 * blend.x) - 1, 0) : min(base.x + 2 * (blend.x - 0.5), 1);
    ret.y = blend.y < 0.5 ? max(base.y + (2 * blend.y) - 1, 0) : min(base.y + 2 * (blend.y - 0.5), 1);
    ret.z = blend.z < 0.5 ? max(base.z + (2 * blend.z) - 1, 0) : min(base.z + 2 * (blend.z - 0.5), 1);
    return mix(base, ret, opacity);
}

// LinearLightAddSub
vec3 blend_linear_light_add_sub(vec3 base, vec3 blend, float opacity)
{
    vec3 ret = blend + 2.0 * base - 1.0;
    return mix(base, ret, opacity);
}

// Multiply
vec3 blend_multiply(vec3 base, vec3 blend, float opacity)
{
    vec3 ret = base * blend;
    return mix(base, ret, opacity);
}

// Negation
vec3 blend_negation(vec3 base, vec3 blend, float opacity)
{
    vec3 ret = 1.0 - abs(1.0 - blend - base);
    return mix(base, ret, opacity);
}

// Overlay
vec3 blend_overlay(vec3 base, vec3 blend, float opacity)
{
    vec3 result1 = 1.0 - 2.0 * (1.0 - base) * (1.0 - blend);
    vec3 result2 = 2.0 * base * blend;
    vec3 zeroOrOne = step(base, vec3(0.5));
    vec3 ret = result2 * zeroOrOne + (1 - zeroOrOne) * result1;
    return mix(base, ret, opacity);
}

// PinLight
vec3 blend_pin_light(vec3 base, vec3 blend, float opacity)
{
    vec3 check = step (0.5, blend);
    vec3 result1 = check * max(2.0 * (base - 0.5), blend);
    vec3 ret = result1 + (1.0 - check) * min(2.0 * base, blend);
    return mix(base, ret, opacity);
}

// Screen
vec3 blend_screen(vec3 base, vec3 blend, float opacity)
{
    vec3 ret = 1.0 - (1.0 - blend) * (1.0 - base);
    return mix(base, ret, opacity);
}

// SoftLight
vec3 blend_soft_light(vec3 base, vec3 blend, float opacity)
{
    vec3 result1 = 2.0 * base * blend + base * base * (1.0 - 2.0 * blend);
    vec3 result2 = sqrt(base) * (2.0 * blend - 1.0) + 2.0 * base * (1.0 - blend);
    vec3 zeroOrOne = step(0.5, blend);
    vec3 ret = result2 * zeroOrOne + (1 - zeroOrOne) * result1;
    return mix(base, ret, opacity);
}

// Subtract
vec3 blend_subtract(vec3 base, vec3 blend, float opacity)
{
    vec3 ret = base - blend;
    return mix(base, ret, opacity);
}

// VividLight
vec3 blend_vivid_light(vec3 base, vec3 blend, float opacity)
{
    vec3 result1 = 1.0 - (1.0 - blend) / (2.0 * base);
    vec3 result2 = blend / (2.0 * (1.0 - base));
    vec3 zeroOrOne = step(0.5, base);
    vec3 ret = result2 * zeroOrOne + (1 - zeroOrOne) * result1;
    return mix(base, ret, opacity);
}

// Overwrite
vec3 blend_overwrite(vec3 base, vec3 blend, float opacity)
{
    return mix(base, blend, opacity);
}

)";
    }

    virtual std::string GetBody(const Evaluator& eval) const override
    {
        std::string func;
        switch (m_mode)
        {
        case Mode::Burn:
            func = "blend_burn";
            break;
        case Mode::Darken:
            func = "blend_darken";
            break;
        case Mode::Difference:
            func = "blend_difference";
            break;
        case Mode::Dodge:
            func = "blend_dodge";
            break;
        case Mode::Divide:
            func = "blend_divide";
            break;
        case Mode::Exclusion:
            func = "blend_exclusion";
            break;
        case Mode::HardLight:
            func = "blend_hard_light";
            break;
        case Mode::HardMix:
            func = "blend_hard_mix";
            break;
        case Mode::Lighten:
            func = "blend_lighten";
            break;
        case Mode::LinearBurn:
            func = "blend_linear_burn";
            break;
        case Mode::LinearDodge:
            func = "blend_linear_dodge";
            break;
        case Mode::LinearLight:
            func = "blend_linear_light";
            break;
        case Mode::LinearLightAddSub:
            func = "blend_linear_light_add_sub";
            break;
        case Mode::Multiply:
            func = "blend_multiply";
            break;
        case Mode::Negation:
            func = "blend_negation";
            break;
        case Mode::Overlay:
            func = "blend_overlay";
            break;
        case Mode::PinLight:
            func = "blend_pin_light";
            break;
        case Mode::Screen:
            func = "blend_screen";
            break;
        case Mode::SoftLight:
            func = "blend_soft_light";
            break;
        case Mode::Subtract:
            func = "blend_subtract";
            break;
        case Mode::VividLight:
            func = "blend_vivid_light";
            break;
        case Mode::Overwrite:
            func = "blend_overwrite";
            break;
        default:
            assert(0);
        }

        return "vec3 #_out# = " + func + "(#base#, #blend#, #opacity#);";
    }

    RTTR_ENABLE(Block)

#define PARM_FILEPATH "shadergraph/block/blend.parm.h"
#include <dag/node_parms_gen.h>
#undef PARM_FILEPATH

}; // Blend

}
}