// from https://iquilezles.org/www/articles/distfunctions/distfunctions.htm

#pragma once

#include "shadergraph/Block.h"

namespace shadergraph
{
namespace block
{

class SDF_Link : public Block
{
public:
    SDF_Link() : Block(R"(

float sdLink(vec3 p, float le, float r1, float r2)
{
    vec3 q = vec3(p.x, max(abs(p.y) - le, 0.0), p.z);
    return length(vec2(length(q.xy) - r1,q.z)) - r2;
}

)") {}

    RTTR_ENABLE(Block)

}; // SDF_Link

}
}