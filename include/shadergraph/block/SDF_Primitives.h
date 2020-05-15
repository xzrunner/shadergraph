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

//float sdSphere(vec3 p, float s)
//{
//    return length(p) - s;
//}

float sdSphere(vec3 p)
{
    return length(p) - 0.5;
}

)") {}

    RTTR_ENABLE(Block)

}; // SDF_Primitives

}
}