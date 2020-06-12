// from https://iquilezles.org/www/articles/distfunctions/distfunctions.htm

#pragma once

#include "shadergraph/Block.h"

namespace shadergraph
{
namespace block
{

class SDF_VerticalCapsule : public Block
{
public:
    SDF_VerticalCapsule() : Block(R"(

float sdVerticalCapsule(vec3 p, float h, float r)
{
    p.y -= clamp(p.y, 0.0, h);
    return length(p) - r;
}

)") {}

    RTTR_ENABLE(Block)

}; // SDF_VerticalCapsule

}
}