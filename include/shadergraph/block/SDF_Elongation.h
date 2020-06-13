// from https://iquilezles.org/www/articles/distfunctions/distfunctions.htm

#pragma once

#include "shadergraph/Block.h"

namespace shadergraph
{
namespace block
{

class SDF_Elongation : public Block
{
public:
    SDF_Elongation() : Block(R"(

/////////////////////////////////////////////////
/// <opElongate> @function f_primitive (vec3, ...)->float
/////////////////////////////////////////////////

float opElongate(in vec3 p, in vec3 h)
{
    vec3 q = p - clamp(p, -h, h);
    return f_primitive(q);
}

)") {}

    RTTR_ENABLE(Block)

}; // SDF_Elongation

}
}