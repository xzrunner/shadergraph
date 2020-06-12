// from https://iquilezles.org/www/articles/distfunctions/distfunctions.htm

#pragma once

#include "shadergraph/Block.h"

namespace shadergraph
{
namespace block
{

class SDF_InfiniteCone : public Block
{
public:
    SDF_InfiniteCone() : Block(R"(

float sdCone(vec3 p, vec2 c)
{
    // c is the sin/cos of the angle
    vec2 q = vec2(length(p.xz), -p.y);
    float d = length(q - c * max(dot(q, c), 0.0));
    return d * ((q.x * c.y - q.y * c.x < 0.0) ? -1.0 : 1.0);
}

)") {}

    RTTR_ENABLE(Block)

}; // SDF_InfiniteCone

}
}