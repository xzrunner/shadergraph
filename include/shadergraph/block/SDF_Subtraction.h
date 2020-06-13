// from https://iquilezles.org/www/articles/distfunctions/distfunctions.htm

#pragma once

#include "shadergraph/Block.h"

namespace shadergraph
{
namespace block
{

class SDF_Subtraction : public Block
{
public:
    SDF_Subtraction() : Block(R"(

float opSubtraction(float d1, float d2) { return max(-d1, d2); }

)") {}

    RTTR_ENABLE(Block)

}; // SDF_Subtraction

}
}