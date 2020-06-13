// from https://iquilezles.org/www/articles/distfunctions/distfunctions.htm

#pragma once

#include "shadergraph/Block.h"

namespace shadergraph
{
namespace block
{

class SDF_Twist : public Block
{
public:
    SDF_Twist() : Block(R"(

/////////////////////////////////////////////////
/// <opDisplace> @function f_primitive (vec3, ...)->float
/////////////////////////////////////////////////

float opTwist(in vec3 p)
{
    const float k = 10.0; // or some other amount
    float c = cos(k * p.y);
    float s = sin(k * p.y);
    mat2  m = mat2(c, -s, s, c);
    vec3  q = vec3(m * p.xz, p.y);
    return f_primitive(q);
}

)") {}

    RTTR_ENABLE(Block)

}; // SDF_Twist

}
}