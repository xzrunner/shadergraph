// from https://iquilezles.org/www/articles/distfunctions/distfunctions.htm

#pragma once

#include "shadergraph/Block.h"

namespace shadergraph
{
namespace block
{

class SDF_Pyramid : public Block
{
public:
    SDF_Pyramid() : Block(R"(

float sdPyramid(vec3 p, float h)
{
    float m2 = h * h + 0.25;
    
    p.xz = abs(p.xz);
    p.xz = (p.z > p.x) ? p.zx : p.xz;
    p.xz -= 0.5;

    vec3 q = vec3(p.z, h * p.y - 0.5 * p.x, h * p.x + 0.5 * p.y);
   
    float s = max(-q.x, 0.0);
    float t = clamp((q.y - 0.5 * p.z) / (m2 + 0.25), 0.0, 1.0);
    
    float a = m2 * (q.x + s) * (q.x + s) + q.y * q.y;
    float b = m2 * (q.x + 0.5 * t) * (q.x + 0.5 * t) + (q.y - m2 * t) * (q.y - m2 * t);
    
    float d2 = min(q.y, -q.x * m2-q.y * 0.5) > 0.0 ? 0.0 : min(a, b);
    
    return sqrt((d2 + q.z * q.z) / m2) * sign(max(q.z, -p.y));
}

)") {}

    RTTR_ENABLE(Block)

}; // SDF_Pyramid

}
}