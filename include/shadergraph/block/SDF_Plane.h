// from https://iquilezles.org/www/articles/distfunctions/distfunctions.htm

#pragma once

#include "shadergraph/Block.h"

namespace shadergraph
{
namespace block
{

class SDF_Plane : public Block
{
public:
    SDF_Plane() : Block(R"(

float sdPlane(vec3 p, vec3 n, float h)
{
    // n must be normalized
    return dot(p, n) + h;
}

)") {}

    RTTR_ENABLE(Block)

}; // SDF_Plane

}
}