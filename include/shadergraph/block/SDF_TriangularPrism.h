// from https://iquilezles.org/www/articles/distfunctions/distfunctions.htm

#pragma once

#include "shadergraph/Block.h"

namespace shadergraph
{
namespace block
{

class SDF_TriangularPrism : public Block
{
public:
    SDF_TriangularPrism() : Block(R"(

float sdTriPrism(vec3 p, vec2 h)
{
    vec3 q = abs(p);
    return max(q.z - h.y, max(q.x * 0.866025 + p.y * 0.5, -p.y) - h.x * 0.5);
}

)") {}

    RTTR_ENABLE(Block)

}; // SDF_TriangularPrism

}
}