// from https://iquilezles.org/www/articles/distfunctions/distfunctions.htm

#pragma once

#include "shadergraph/Block.h"

namespace shadergraph
{
namespace block
{

class SDF_Bend : public Block
{
public:
    SDF_Bend() : Block(R"(

/////////////////////////////////////////////////
/// <opDisplace> @function f_primitive (vec3, ...)->float
/////////////////////////////////////////////////

float opCheapBend(in vec3 p)
{
    const float k = 10.0; // or some other amount
    float c = cos(k * p.x);
    float s = sin(k* p .x);
    mat2  m = mat2(c, -s, s, c);
    vec3  q = vec3(m * p.xy, p.z);
    return primitive(q);
}

)") {}

    RTTR_ENABLE(Block)

}; // SDF_Bend

}
}