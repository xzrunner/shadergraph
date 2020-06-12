// from https://iquilezles.org/www/articles/distfunctions/distfunctions.htm

#pragma once

#include "shadergraph/Block.h"

namespace shadergraph
{
namespace block
{

class SDF_Ellipsoid : public Block
{
public:
    SDF_Ellipsoid() : Block(R"(

float sdEllipsoid(vec3 p, vec3 r)
{
    float k0 = length(p / r);
    float k1 = length(p / (r * r));
    return k0 * (k0 - 1.0) / k1;
}

)") {}

    RTTR_ENABLE(Block)

}; // SDF_Ellipsoid

}
}