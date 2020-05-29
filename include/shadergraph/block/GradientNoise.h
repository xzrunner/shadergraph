#pragma once

#include "shadergraph/Block.h"

namespace shadergraph
{
namespace block
{

class GradientNoise : public Block
{
public:
    GradientNoise() : Block(R"(

/////////////////////////////////////////////////
/// <gradient_noise> @export
/////////////////////////////////////////////////

// https://docs.unity3d.com/Packages/com.unity.shadergraph@9.0/manual/Gradient-Noise-Node.html

vec2 gradient_noise_dir(vec2 p)
{
    p = mod(p, 289);
    float x = mod((34 * p.x + 1) * p.x, 289) + p.y;
    x = mod((34 * x + 1) * x, 289);
    x = fract(x / 41) * 2 - 1;
    return normalize(vec2(x - floor(x + 0.5), abs(x) - 0.5));
}

float gradient_noise(vec2 p)
{
    vec2 ip = floor(p);
    vec2 fp = fract(p);
    float d00 = dot(gradient_noise_dir(ip), fp);
    float d01 = dot(gradient_noise_dir(ip + vec2(0, 1)), fp - vec2(0, 1));
    float d10 = dot(gradient_noise_dir(ip + vec2(1, 0)), fp - vec2(1, 0));
    float d11 = dot(gradient_noise_dir(ip + vec2(1, 1)), fp - vec2(1, 1));
    fp = fp * fp * fp * (fp * (fp * 6 - 15) + 10);
    return mix(mix(d00, d01, fp.y), mix(d10, d11, fp.y), fp.x);
}

)") {}

    RTTR_ENABLE(Block)

}; // GradientNoise

}
}