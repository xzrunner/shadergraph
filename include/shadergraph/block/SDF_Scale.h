// from https://iquilezles.org/www/articles/distfunctions/distfunctions.htm

#pragma once

#include "shadergraph/Block.h"

namespace shadergraph
{
namespace block
{

class SDF_Scale : public Block
{
public:
    SDF_Scale() : Block(R"(

/////////////////////////////////////////////////
/// <opScale> @function f_primitive (vec3, ...)->float
/////////////////////////////////////////////////

float opScale(in vec3 p, in float s)
{
    return f_primitive(p / s) * s;
}

)") {}

    RTTR_ENABLE(Block)

}; // SDF_Scale

}
}