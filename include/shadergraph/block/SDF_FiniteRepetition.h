// from https://iquilezles.org/www/articles/distfunctions/distfunctions.htm

#pragma once

#include "shadergraph/Block.h"

namespace shadergraph
{
namespace block
{

class SDF_FiniteRepetition : public Block
{
public:
    SDF_FiniteRepetition() : Block(R"(

/////////////////////////////////////////////////
/// <opRep> @function f_primitive (vec3, ...)->float
/////////////////////////////////////////////////

vec3 opRepLim(in vec3 p, in float c, in vec3 l)
{
    vec3 q = p - c * clamp(round(p / c), -l, l);
    return f_primitive(q);
}

)") {}

    RTTR_ENABLE(Block)

}; // SDF_FiniteRepetition

}
}