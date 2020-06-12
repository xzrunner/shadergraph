// from https://iquilezles.org/www/articles/distfunctions/distfunctions.htm

#pragma once

#include "shadergraph/Block.h"

namespace shadergraph
{
namespace block
{

class SDF_Primitives : public Block
{
public:
    SDF_Primitives() : Block(R"(

/////////////////////////////////////////////////
/// <sdSphere> @export
/// <sdSphere> @param [s=0.5]
/////////////////////////////////////////////////

float sdSphere(vec3 p, float s)
{
    return length(p) - s;
}



)") {}

    RTTR_ENABLE(Block)

}; // SDF_Primitives

}
}