// from https://iquilezles.org/www/articles/distfunctions/distfunctions.htm

#pragma once

#include "shadergraph/Block.h"

namespace shadergraph
{
namespace block
{

class SDF_Displacement : public Block
{
public:
    SDF_Displacement() : Block(R"(

/////////////////////////////////////////////////
/// <opDisplace> @function f_primitive (vec3, ...)->float
/// <opDisplace> @function f_displacement (vec3)->float
/////////////////////////////////////////////////

float opDisplace(in vec3 p)
{
    float d1 = f_primitive(p);
    float d2 = f_displacement(p);
    return d1 + d2;
}

)") {}

    RTTR_ENABLE(Block)

}; // SDF_Displacement

}
}