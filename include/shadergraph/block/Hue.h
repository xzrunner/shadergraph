#pragma once

#include "shadergraph/Block.h"

namespace shadergraph
{
namespace block
{

class Hue : public Block
{
public:
    Hue() : Block(R"(

// https://docs.unity3d.com/Packages/com.unity.shadergraph@9.0/manual/Hue-Node.html
vec3 hue(vec3 rgb, float offset)
{
    vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
    vec4 P = mix(vec4(rgb.bg, K.wz), vec4(rgb.gb, K.xy), step(rgb.b, rgb.g));
    vec4 Q = mix(vec4(P.xyw, rgb.r), vec4(rgb.r, P.yzx), step(P.x, rgb.r));
    float D = Q.x - min(Q.w, Q.y);
    float E = 1e-10;
    vec3 hsv = vec3(abs(Q.z + (Q.w - Q.y)/(6.0 * D + E)), D / (Q.x + E), Q.x);

    float hue = hsv.x + offset;
    hsv.x = (hue < 0)
            ? hue + 1
            : (hue > 1)
                ? hue - 1
                : hue;

    // HSV to RGB
    vec4 K2 = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 P2 = abs(fract(hsv.xxx + K2.xyz) * 6.0 - K2.www);
    return hsv.z * mix(K2.xxx, clamp(P2 - K2.xxx, 0.0, 1.0), hsv.y);
}

)") {}

    RTTR_ENABLE(Block)

}; // Hue

}
}