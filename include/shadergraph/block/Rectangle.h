#pragma once

#include "shadergraph/Block.h"

namespace shadergraph
{
namespace block
{

class Rectangle : public Block
{
public:
    Rectangle() : Block(R"(

// https://docs.unity3d.com/Packages/com.unity.shadergraph@9.0/manual/Rectangle-Node.html
float rectangle(vec2 uv, float width, float height)
{
    vec2 d = abs(uv * 2 - 1) - vec2(width, height);
    d = 1 - d / fwidth(d);
    return clamp(min(d.x, d.y), 0.0, 1.0);
}

)") {}

    RTTR_ENABLE(Block)

}; // Rectangle

}
}