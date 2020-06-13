// from https://iquilezles.org/www/articles/distfunctions/distfunctions.htm

#pragma once

#include "shadergraph/Block.h"

namespace shadergraph
{
namespace block
{

class SDF_SymmetryXZ : public Block
{
public:
    SDF_SymmetryXZ() : Block(R"(

/////////////////////////////////////////////////
/// <opSymX> @function f_primitive (vec3, ...)->float
/////////////////////////////////////////////////

float opSymXZ(in vec3 p)
{
    p.xz = abs(p.xz);
    return f_primitive(p);
}

)") {}

    RTTR_ENABLE(Block)

}; // SDF_SymmetryXZ

}
}