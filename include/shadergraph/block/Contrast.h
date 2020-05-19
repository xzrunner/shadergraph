#pragma once

#include "shadergraph/Block.h"

namespace shadergraph
{
namespace block
{

class Contrast : public Block
{
public:
    Contrast() : Block(R"(

// https://docs.unity3d.com/Packages/com.unity.shadergraph@9.0/manual/Contrast-Node.html
vec3 contrast(vec3 _in, float contrast)
{
    float midpoint = pow(0.5, 2.2);
    return (_in - midpoint) * contrast + midpoint;
}

)") {}


    RTTR_ENABLE(Block)

}; // Contrast

}
}