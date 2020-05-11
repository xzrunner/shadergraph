#pragma once

#include "shadergraph/Block.h"

namespace shadergraph
{
namespace block
{

class Ellipse : public Block
{
public:
    Ellipse() : Block(R"(

// https://docs.unity3d.com/Packages/com.unity.shadergraph@9.0/manual/Ellipse-Node.html
float ellipse(vec2 uv, float width, float height)
{
    float d = length((uv * 2 - 1) / vec2(width, height));
    return clamp((1 - d) / fwidth(d), 0.0, 1.0);
}

)") {}

    RTTR_ENABLE(Block)

}; // Ellipse

}
}