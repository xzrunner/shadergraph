#pragma once

#include "shadergraph/Block.h"

namespace shadergraph
{
namespace block
{

class Polygon : public Block
{
public:
    Polygon() : Block(R"(

// proposed solution from
// http://stackoverflow.com/questions/26070410/robust-atany-x-on-glsl-for-converting-xy-coordinate-to-angle
// swaps params when |x| <= |y|
float atan2(float y, float x)
{
    bool s = (abs(x) > abs(y));
    return mix(3.14159265358979/2.0 - atan(x,y), atan(y,x), s);
}

// https://docs.unity3d.com/Packages/com.unity.shadergraph@9.0/manual/Polygon-Node.html
/////////////////////////////////////////////////
/// @export Polygon
/////////////////////////////////////////////////
float polygon(vec2 uv, float sides, float width, float height)
{
    float pi = 3.14159265359;
    float aWidth = width * cos(pi / sides);
    float aHeight = height * cos(pi / sides);
    uv = (uv * 2 - 1) / vec2(aWidth, aHeight);
    uv.y *= -1;
    float pCoord = atan2(uv.x, uv.y);
    float r = 2 * pi / sides;
    float distance = cos(floor(0.5 + pCoord / r) * r - pCoord) * length(uv);
    return clamp((1 - distance) / fwidth(distance), 0.0, 1.0);
}

)") {}

    RTTR_ENABLE(Block)

}; // Polygon

}
}