#pragma once

#include "shadergraph/Block.h"

namespace shadergraph
{
namespace block
{

class Spherize : public Block
{
public:
    Spherize() : Block(R"(

vec2 spherize(vec2 uv, vec2 center, vec2 strength, vec2 offset)
{
    vec2 delta = uv - center;
    float delta2 = dot(delta.xy, delta.xy);
    float delta4 = delta2 * delta2;
    vec2 delta_offset = delta4 * strength;
    return uv + delta * delta_offset + offset;
}

)") {}

    RTTR_ENABLE(Block)

}; // Spherize

}
}