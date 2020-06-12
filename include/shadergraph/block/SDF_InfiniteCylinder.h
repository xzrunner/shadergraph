// from https://iquilezles.org/www/articles/distfunctions/distfunctions.htm

#pragma once

#include "shadergraph/Block.h"

namespace shadergraph
{
namespace block
{

class SDF_InfiniteCylinder : public Block
{
public:
    SDF_InfiniteCylinder() : Block(R"(

float sdCylinder(vec3 p, vec3 c)
{
    return length(p.xz - c.xy) - c.z;
}

)") {}

    RTTR_ENABLE(Block)

}; // SDF_InfiniteCylinder

}
}