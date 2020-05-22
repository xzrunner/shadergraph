#pragma once

#include "shadergraph/Block.h"

namespace shadergraph
{
namespace block
{

class SampleGradient : public Block
{
public:
    SampleGradient() : Block(R"(

// https://docs.unity3d.com/Packages/com.unity.shadergraph@9.0/manual/Sample-gradient-Node.html
vec4 SampleGradient(vec4 gradient, float time)
{
    vec3 color = gradient.colors[0].rgb;
    //[unroll]
    for (int c = 1; c < 8; c++)
    {
        float colorPos = clamp((time - gradient.colors[c-1].w) / (gradient.colors[c].w - gradient.colors[c-1].w), 0.0, 1.0) * step(c, gradient.colorsLength-1);
        color = lerp(color, gradient.colors[c].rgb, lerp(colorPos, step(0.01, colorPos), gradient.type));
    }
//#ifndef UNITY_COLORSPACE_GAMMA
//    color = SRGBToLinear(color);
//#endif
    float alpha = gradient.alphas[0].x;
    //[unroll]
    for (int a = 1; a < 8; a++)
    {
        float alphaPos = clamp((time - gradient.alphas[a-1].y) / (gradient.alphas[a].y - gradient.alphas[a-1].y), 0.0, 1.0) * step(a, gradient.alphasLength-1);
        alpha = lerp(alpha, gradient.alphas[a].x, lerp(alphaPos, step(0.01, alphaPos), gradient.type));
    }
    return vec4(color, alpha);
}

)") {}

    RTTR_ENABLE(Block)

}; // SampleGradient

}
}