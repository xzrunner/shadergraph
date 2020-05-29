#pragma once

#include "shadergraph/Block.h"

namespace shadergraph
{
namespace block
{

class SimpleNoise : public Block
{
public:
    SimpleNoise() : Block(R"(

/////////////////////////////////////////////////
/// <simple_noise> @export
/////////////////////////////////////////////////

// https://docs.unity3d.com/Packages/com.unity.shadergraph@9.0/manual/Simple-Noise-Node.html

float simple_noise_random_value (vec2 uv)
{
    return fract(sin(dot(uv, vec2(12.9898, 78.233)))*43758.5453);
}

float simple_noise_interpolate (float a, float b, float t)
{
    return (1.0-t)*a + (t*b);
}

float simple_noise_value(vec2 uv)
{
    vec2 i = floor(uv);
    vec2 f = fract(uv);
    f = f * f * (3.0 - 2.0 * f);

    uv = abs(fract(uv) - 0.5);
    vec2 c0 = i + vec2(0.0, 0.0);
    vec2 c1 = i + vec2(1.0, 0.0);
    vec2 c2 = i + vec2(0.0, 1.0);
    vec2 c3 = i + vec2(1.0, 1.0);
    float r0 = simple_noise_random_value(c0);
    float r1 = simple_noise_random_value(c1);
    float r2 = simple_noise_random_value(c2);
    float r3 = simple_noise_random_value(c3);

    float bottomOfGrid = simple_noise_interpolate(r0, r1, f.x);
    float topOfGrid = simple_noise_interpolate(r2, r3, f.x);
    float t = simple_noise_interpolate(bottomOfGrid, topOfGrid, f.y);
    return t;
}

float simple_noise(vec2 uv, float scale)
{
    float t = 0.0;

    float freq = pow(2.0, float(0));
    float amp = pow(0.5, float(3-0));
    t += simple_noise_value(vec2(uv.x*scale/freq, uv.y*scale/freq))*amp;

    freq = pow(2.0, float(1));
    amp = pow(0.5, float(3-1));
    t += simple_noise_value(vec2(uv.x*scale/freq, uv.y*scale/freq))*amp;

    freq = pow(2.0, float(2));
    amp = pow(0.5, float(3-2));
    t += simple_noise_value(vec2(uv.x*scale/freq, uv.y*scale/freq))*amp;

    return t;
}

)") {}

    RTTR_ENABLE(Block)

}; // SimpleNoise

}
}