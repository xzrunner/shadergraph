// from https://iquilezles.org/www/articles/distfunctions/distfunctions.htm

#pragma once

#include "shadergraph/Block.h"

namespace shadergraph
{
namespace block
{

class SDF_Elongation2 : public Block
{
public:
    SDF_Elongation2() : Block(R"(

/////////////////////////////////////////////////
/// <opElongate> @function f_primitive (vec3, ...)->float
/////////////////////////////////////////////////

float opElongate(in vec3 p, in vec3 h)
{
    vec3 q = abs(p) - h;
    return f_primitive(max(q, 0.0)) + min(max(q.x, max(q.y, q.z)), 0.0);
}

)") {}

    RTTR_ENABLE(Block)

}; // SDF_Elongation2

}
}