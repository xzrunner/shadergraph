// from https://iquilezles.org/www/articles/distfunctions/distfunctions.htm

#pragma once

#include "shadergraph/Block.h"

namespace shadergraph
{
namespace block
{

class SDF_Torus : public Block
{
public:
    SDF_Torus() : Block(R"(

float sdTorus(vec3 p, vec2 t)
{
    vec2 q = vec2(length(p.xz) - t.x, p.y);
    return length(q) - t.y;
}

)") {}

    RTTR_ENABLE(Block)

}; // SDF_Torus

}
}