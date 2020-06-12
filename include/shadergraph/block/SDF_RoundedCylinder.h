// from https://iquilezles.org/www/articles/distfunctions/distfunctions.htm

#pragma once

#include "shadergraph/Block.h"

namespace shadergraph
{
namespace block
{

class SDF_RoundedCylinder : public Block
{
public:
    SDF_RoundedCylinder() : Block(R"(

float sdRoundedCylinder(vec3 p, float ra, float rb, float h)
{
    vec2 d = vec2(length(p.xz) - 2.0 * ra + rb, abs(p.y) - h);
    return min(max(d.x, d.y), 0.0) + length(max(d, 0.0)) - rb;
}

)") {}

    RTTR_ENABLE(Block)

}; // SDF_RoundedCylinder

}
}