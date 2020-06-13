// from https://iquilezles.org/www/articles/distfunctions/distfunctions.htm

#pragma once

#include "shadergraph/Block.h"

namespace shadergraph
{
namespace block
{

class SDF_SmoothSubtraction : public Block
{
public:
    SDF_SmoothSubtraction() : Block(R"(

float opSmoothSubtraction(float d1, float d2, float k) 
{
    float h = clamp(0.5 - 0.5 * (d2 + d1) / k, 0.0, 1.0);
    return mix(d2, -d1, h) + k * h * (1.0 - h); 
}

)") {}

    RTTR_ENABLE(Block)

}; // SDF_SmoothSubtraction

}
}