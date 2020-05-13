#pragma once

#include "shadergraph/Block.h"

namespace shadergraph
{
namespace block
{

class Rotate : public Block
{
public:
    Rotate() : Block(R"(

// radians
vec2 rotate(vec2 uv, vec2 center, float rotation)
{
    uv -= center;
    float s = sin(rotation);
    float c = cos(rotation);
    mat2 rMatrix = mat2(c, -s, s, c);
    rMatrix *= 0.5;
    rMatrix += 0.5;
    rMatrix = rMatrix * 2 - 1;
    uv.xy = rMatrix * uv.xy;
    uv += center;
    return uv;
}

)") {}

    RTTR_ENABLE(Block)

}; // Rotate

}
}