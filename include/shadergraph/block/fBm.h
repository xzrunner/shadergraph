#pragma once

#include "shadergraph/Block.h"

namespace shadergraph
{
namespace block
{

class fBm : public Block
{
public:
    fBm() : Block(R"(

/////////////////////////////////////////////////
/// <u_lacunarity> @default 2.0
/// <u_gain>       @default 0.5
/// <fBm> @function f_noise (vec2)->float
/////////////////////////////////////////////////

uniform int   u_octaves;
uniform float u_lacunarity;
uniform float u_gain;

float fBm(vec2 st)
{
    float ret = 0;

    float amplitude = 1;
    float frequency = 1;
    for (int i = 0; i <= u_octaves; ++i)
    {
        float v = f_noise(vec2(st.x * frequency, st.y * frequency));
        ret += amplitude * v;

        frequency *= u_lacunarity;
        amplitude *= u_gain;
    }

    return ret;
}

)") {}

    RTTR_ENABLE(Block)

}; // fBm

}
}