// from https://iquilezles.org/www/articles/distfunctions/distfunctions.htm

#pragma once

#include "shadergraph/Block.h"

namespace shadergraph
{
namespace block
{

class SDF_Revolution : public Block
{
public:
    SDF_Revolution() : Block(R"(

/////////////////////////////////////////////////
/// <opElongate> @function f_primitive (vec2, ...)->float
/////////////////////////////////////////////////

float opRevolution(in vec3 p, float o)
{
    vec2 q = vec2(length(p.xz) - o, p.y);
    return f_primitive(q);
}

)") {}

    RTTR_ENABLE(Block)

}; // SDF_Revolution

}
}