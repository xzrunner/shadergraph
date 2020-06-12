// from https://iquilezles.org/www/articles/distfunctions/distfunctions.htm

#pragma once

#include "shadergraph/Block.h"

namespace shadergraph
{
namespace block
{

class SDF_CappedCylinder : public Block
{
public:
    SDF_CappedCylinder() : Block(R"(

float sdCappedCylinder(vec3 p, float h, float r)
{
    vec2 d = abs(vec2(length(p.xz), p.y)) - vec2(h, r);
    return min(max(d.x, d.y), 0.0) + length(max(d, 0.0));
}

)") {}

    RTTR_ENABLE(Block)

}; // SDF_CappedCylinder

}
}