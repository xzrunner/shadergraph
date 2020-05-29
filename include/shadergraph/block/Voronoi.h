#pragma once

#include "shadergraph/Block.h"

namespace shadergraph
{
namespace block
{

class Voronoi : public Block
{
public:
    Voronoi() : Block(R"(

/////////////////////////////////////////////////
/// <voronoi> @export
/////////////////////////////////////////////////

// https://docs.unity3d.com/Packages/com.unity.shadergraph@9.0/manual/Voronoi-Node.html

vec2 voronoi_noise_random_vector(vec2 uv, float offset)
{
    mat2 m = mat2(15.27, 47.63, 99.41, 89.98);
    uv = fract(sin(m * uv) * 46839.32);
    return vec2(sin(uv.y*+offset)*0.5+0.5, cos(uv.x*offset)*0.5+0.5);
}

float voronoi(vec2 uv, float angle_offset, float cell_density)
{
    vec2 g = floor(uv * cell_density);
    vec2 f = fract(uv * cell_density);
    float t = 8.0;
    vec3 res = vec3(8.0, 0.0, 0.0);

    float ret = 0;
    for(int y=-1; y<=1; y++)
    {
        for(int x=-1; x<=1; x++)
        {
            vec2 lattice = vec2(x,y);
            vec2 offset = voronoi_noise_random_vector(lattice + g, angle_offset);
            float d = distance(lattice + offset, f);
            if(d < res.x)
            {
                res = vec3(d, offset.x, offset.y);
                ret = res.x;
//                Cells = res.y;
            }
        }
    }

    return ret;
}

)") {}

    RTTR_ENABLE(Block)

}; // Voronoi

}
}