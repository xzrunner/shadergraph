#pragma once

#include "shadergraph/Block.h"

namespace shadergraph
{
namespace block
{

class NormalStrength : public Block
{
public:
    NormalStrength() : Block(R"(

// https://docs.unity3d.com/Packages/com.unity.shadergraph@9.0/manual/Normal-strength-Node.html
vec3 normal_strength(vec3 rgb, float strength)
{
    return vec3(rgb.rg * strength, mix(1, rgb.b, clamp(strength, 0.0, 1.0)));
}

)") {}


    RTTR_ENABLE(Block)

}; // NormalStrength

}
}