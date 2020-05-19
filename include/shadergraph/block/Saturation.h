#pragma once

#include "shadergraph/Block.h"

namespace shadergraph
{
namespace block
{

class Saturation : public Block
{
public:
    Saturation() : Block(R"(

vec3 saturation(vec3 rgb, float saturation)
{
    float luma = dot(rgb, vec3(0.2126729, 0.7151522, 0.0721750));
    vec3 luma3 = vec3(luma, luma, luma);
    return luma3 + vec3(saturation, saturation, saturation) * (rgb - luma3);
}

)") {}

    RTTR_ENABLE(Block)

}; // Saturation

}
}