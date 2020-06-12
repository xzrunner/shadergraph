// from https://iquilezles.org/www/articles/distfunctions/distfunctions.htm

#pragma once

#include "shadergraph/Block.h"

namespace shadergraph
{
namespace block
{

class SDF_Octahedron2 : public Block
{
public:
    SDF_Octahedron2() : Block(R"(

float sdOctahedron(vec3 p, float s)
{
    p = abs(p);
    return (p.x + p.y + p.z - s) * 0.57735027;
}

)") {}

    RTTR_ENABLE(Block)

}; // SDF_Octahedron2

}
}