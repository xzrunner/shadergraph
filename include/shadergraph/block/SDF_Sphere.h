// from https://iquilezles.org/www/articles/distfunctions/distfunctions.htm

#pragma once

#include "shadergraph/Block.h"

namespace shadergraph
{
namespace block
{

class SDF_Sphere : public Block
{
public:
    SDF_Sphere() : Block(R"(

float sdSphere(vec3 p, float s)
{
    return length(p) - s;
}

)") {}

    RTTR_ENABLE(Block)

}; // SDF_Sphere

}
}