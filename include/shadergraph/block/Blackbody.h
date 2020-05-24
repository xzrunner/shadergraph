#pragma once

#include "shadergraph/Block.h"

namespace shadergraph
{
namespace block
{

class Blackbody : public Block
{
public:
    Blackbody() : Block(R"(

// https://docs.unity3d.com/Packages/com.unity.shadergraph@9.0/manual/Blackbody-Node.html
vec3 blackbody(float temperature)
{
    vec3 color = vec3(255.0, 255.0, 255.0);
    color.x = 56100000. * pow(temperature,(-3.0 / 2.0)) + 148.0;
    color.y = 100.04 * log(temperature) - 623.6;
    if (temperature > 6500.0) {color.y = 35200000.0 * pow(temperature,(-3.0 / 2.0)) + 184.0;}
    color.z = 194.18 * log(temperature) - 1448.6;
    color = clamp(color, 0.0, 255.0)/255.0;
    if (temperature < 1000.0) {color *= temperature/1000.0;}
    return color;
}

)") {}

    RTTR_ENABLE(Block)

}; // Blackbody

}
}