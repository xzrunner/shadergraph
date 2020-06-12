// from https://iquilezles.org/www/articles/distfunctions/distfunctions.htm

#pragma once

#include "shadergraph/Block.h"

namespace shadergraph
{
namespace block
{

class SDF_BoundCone : public Block
{
public:
    SDF_BoundCone() : Block(R"(

float sdCone(vec3 p, vec2 c, float h)
{
    float q = length(p.xz);
    return max(dot(c.xy, vec2(q, p.y)), -h - p.y);
}

)") {}

    RTTR_ENABLE(Block)

}; // SDF_BoundCone

}
}