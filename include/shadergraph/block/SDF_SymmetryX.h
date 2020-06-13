// from https://iquilezles.org/www/articles/distfunctions/distfunctions.htm

#pragma once

#include "shadergraph/Block.h"

namespace shadergraph
{
namespace block
{

class SDF_SymmetryX : public Block
{
public:
    SDF_SymmetryX() : Block(R"(

/////////////////////////////////////////////////
/// <opSymX> @function f_primitive (vec3, ...)->float
/////////////////////////////////////////////////

float opSymX(in vec3 p)
{
    p.x = abs(p.x);
    return f_primitive(p);
}

)") {}

    RTTR_ENABLE(Block)

}; // SDF_SymmetryX

}
}