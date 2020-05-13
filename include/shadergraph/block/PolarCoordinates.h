#pragma once

#include "shadergraph/Block.h"

namespace shadergraph
{
namespace block
{

class PolarCoordinates : public Block
{
public:
    PolarCoordinates() : Block(R"(

// proposed solution from
// http://stackoverflow.com/questions/26070410/robust-atany-x-on-glsl-for-converting-xy-coordinate-to-angle
// swaps params when |x| <= |y|
float atan2(float y, float x)
{
    bool s = (abs(x) > abs(y));
    return mix(3.14159265358979/2.0 - atan(x,y), atan(y,x), s);
}

// https://docs.unity3d.com/Packages/com.unity.shadergraph@9.0/manual/Polar-Coordinates-Node.html
/////////////////////////////////////////////////
/// @export PolarCoordinates
/////////////////////////////////////////////////
vec2 polar_coordinates(vec2 uv, vec2 center, float radial_scale, float length_scale)
{
    vec2 delta = uv - center;
    float radius = length(delta) * 2 * radial_scale;
    float angle = atan2(delta.x, delta.y) * 1.0/6.28 * length_scale;
    return vec2(radius, angle);
}

)") {}

    RTTR_ENABLE(Block)

}; // PolarCoordinates

}
}