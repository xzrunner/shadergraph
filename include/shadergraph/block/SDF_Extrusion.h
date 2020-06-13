// from https://iquilezles.org/www/articles/distfunctions/distfunctions.htm

#pragma once

#include "shadergraph/Block.h"

namespace shadergraph
{
namespace block
{

class SDF_Extrusion : public Block
{
public:
    SDF_Extrusion() : Block(R"(

/////////////////////////////////////////////////
/// <opExtrusion> @function f_primitive (vec2, ...)->float
/////////////////////////////////////////////////

float opExtrusion(in vec3 p, in float h)
{
    float d = f_primitive(p.xy)
    vec2 w = vec2(d, abs(p.z) - h);
    return min(max(w.x, w.y), 0.0) + length(max(w, 0.0));
}

)") {}

    RTTR_ENABLE(Block)

}; // SDF_Extrusion

}
