// from https://iquilezles.org/www/articles/distfunctions/distfunctions.htm

#pragma once

#include "shadergraph/Block.h"

namespace shadergraph
{
namespace block
{

class SDF_InfiniteRepetition : public Block
{
public:
    SDF_InfiniteRepetition() : Block(R"(

/////////////////////////////////////////////////
/// <opRep> @function f_primitive (vec3, ...)->float
/////////////////////////////////////////////////

float opRep(in vec3 p, in vec3 c)
{
    vec3 q = mod(p + 0.5 * c, c) - 0.5 * c;
    return f_primitive(q);
}

)") {}

    RTTR_ENABLE(Block)

}; // SDF_InfiniteRepetition

}
}