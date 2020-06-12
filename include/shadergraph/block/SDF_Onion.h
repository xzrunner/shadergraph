// from https://iquilezles.org/www/articles/distfunctions/distfunctions.htm

#pragma once

#include "shadergraph/Block.h"

namespace shadergraph
{
namespace block
{

class SDF_Onion : public Block
{
public:
    SDF_Onion() : Block(R"(

float opOnion(in float sdf, in float thickness)
{
    return abs(sdf) - thickness;
}

)") {}

    RTTR_ENABLE(Block)

}; // SDF_Onion

}
}