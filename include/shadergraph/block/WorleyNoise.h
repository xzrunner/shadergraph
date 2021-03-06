#pragma once

#include "shadergraph/Block.h"

namespace shadergraph
{
namespace block
{

class WorleyNoise : public Block
{
public:
    WorleyNoise() : Block(R"(

/////////////////////////////////////////////////
/// <u_dist_op>         @enum Euclideanm, Manhattan, Chebyshev
/// <u_features_mode>   @enum F1, F2, F1_ADD_F2, F2_SUB_F1, F1_MUL_F2, F1_DIV_F2
/// <u_manhattan_scale> @region 0.01, 0.5
/// <u_manhattan_scale> @default 0.5
/// <worley_cells>      @export
/// <worley_no_cells>   @export
/// <worley_no_cells>   @function poisson_number          (float)->uint
/// <worley_no_cells>   @function poisson_uniform_0_1     ()->float
/// <worley_no_cells>   @function poisson_morton          (uint, uint)->uint
/// <worley_no_cells>   @function poisson_seeding         (uint)->void
/////////////////////////////////////////////////

uniform float u_seed;
uniform int   u_dist_op;
uniform int   u_features_mode;
uniform float u_manhattan_scale;
uniform vec2  u_poisson_random_offset;

vec2 _random2(vec2 p)
{
    return fract(sin(vec2(dot(p, vec2(127.1, 311.7)), dot(p, vec2(269.5, 183.3)))) * 43758.5453);
}

float _euclidean_dist(vec2 diff)
{
    return diff.x * diff.x + diff.y * diff.y;
}

float _manhattan_dist(vec2 diff)
{
    return u_manhattan_scale * (abs(diff.x) + abs(diff.y));
}

float _chebyshev_dist(vec2 diff)
{
    float dx = abs(diff.x);
    float dy = abs(diff.y);
    return max(dx, dy);
}

float worley_cells(vec2 st)
{
    vec2 i_st = floor(st);
    vec2 f_st = fract(st);

    float f1 = 1., f2 = 1.;

    for (int y = -1; y <= 1; y++) {
        for (int x = -1; x <= 1; x++) {
            vec2 neighbor = vec2(float(x),float(y));

            vec2 point = _random2(i_st + neighbor);
            point = 0.5 + 0.5*sin(u_seed + 6.2831 * point);

            vec2 diff = neighbor + point - f_st;
            float dist = 0;
            if (u_dist_op == 0) {
                dist = _euclidean_dist(diff);
            } else if (u_dist_op == 1) {
                dist = _manhattan_dist(diff);
            } else if (u_dist_op == 2) {
                dist = _chebyshev_dist(diff);
            }

            if (dist < f1) {
                f2 = f1;
                f1 = dist;
            } else if (dist > f1 && dist < f2) {
                f2 = dist;
            }
        }
    }

    if (u_features_mode == 0) {
        return f1;
    } else if (u_features_mode == 1) {
        return f2;
    } else if (u_features_mode == 2) {
        return f1 + f2;
    } else if (u_features_mode == 3) {
        return f2 - f1;
    } else if (u_features_mode == 4) {
        return f1 * f2;
    } else if (u_features_mode == 5) {
        return f1 / f2;
    } else {
        return 0;
    }

    //// debug draw
    //float color = 0;
    //color += min_dist;                                  // Draw the min distance (distance field)
    //color += 1.-step(.001, min_dist);                   // Draw cell center
    //color += step(.98, f_st.x) + step(.98, f_st.y);     // Draw grid
    //return color;
}

float worley_no_cells(vec2 st, float frequency)
{
    uint s = poisson_morton(uint(u_poisson_random_offset.x), uint(u_poisson_random_offset.y));
    if (s == 0u) {
        s = 1u;
    }
    poisson_seeding(s);

    float min_dist = 1;
    const float density = 1.5;
    uint num = poisson_number(frequency * frequency * density);
    for (uint i = 0u; i < num; ++i)
    {
        float x = poisson_uniform_0_1();
        float y = poisson_uniform_0_1();
        vec2 diff = vec2(x, y) * frequency - st;

        float dist = 0;
        if (u_dist_op == 0) {
            dist = _euclidean_dist(diff);
        } else if (u_dist_op == 1) {
            dist = _manhattan_dist(diff);
        } else if (u_dist_op == 2) {
            dist = _chebyshev_dist(diff);
        }
        min_dist = min(min_dist, dist);
    }

    // debug draw
    min_dist += 1.-step(.002, min_dist);   // Draw cell center

    return min_dist;
}

)") {}

    RTTR_ENABLE(Block)

}; // WorleyNoise

}
}