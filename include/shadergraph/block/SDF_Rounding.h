// from https://iquilezles.org/www/articles/distfunctions/distfunctions.htm

#pragma once

#include "shadergraph/Block.h"

namespace shadergraph
{
namespace block
{

class SDF_Rounding : public Block
{
public:
    SDF_Rounding() : Block(R"(

/////////////////////////////////////////////////
/// <opRound> @function f_primitive (vec3, ...)->float
/////////////////////////////////////////////////

float opRound(float rad)
{
    return f_primitive(p) - rad;
}

)") {}

    RTTR_ENABLE(Block)

}; // SDF_Rounding

}
}