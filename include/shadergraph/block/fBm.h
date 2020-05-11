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

/// @default 0
uniform int u_octaves;

/// @default 2
uniform float u_lacunarity;

/// @default 0.5
uniform float u_gain;

/////////////////////////////////////////////////
/// @uniform   u_octaves     int            Octaves
/// @uniform   u_lacunarity  float          Lacunarity
/// @uniform   u_gain        float          Gain
/// @function  f_noise       (vec2)->float  Noise
/////////////////////////////////////////////////
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