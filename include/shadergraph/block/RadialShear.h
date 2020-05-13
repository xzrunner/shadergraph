#pragma once

#include "shadergraph/Block.h"

namespace shadergraph
{
namespace block
{

class RadialShear : public Block
{
public:
    RadialShear() : Block(R"(

vec2 radial_shear(vec2 uv, vec2 center, vec2 strength, vec2 offset)
{
    vec2 delta = uv - center;
    float delta2 = dot(delta.xy, delta.xy);
    vec2 delta_offset = delta2 * strength;
    return uv + vec2(delta.y, -delta.x) * delta_offset + offset;
}

)") {}

    RTTR_ENABLE(Block)

}; // RadialShear

}
}