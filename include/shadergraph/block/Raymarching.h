#pragma once

#include "shadergraph/Block.h"

namespace shadergraph
{
namespace block
{

class Raymarching : public Block
{
public:
    Raymarching() : Block(R"(

/////////////////////////////////////////////////
/// <raymarching> @export
/// <raymarching> @function f_scene (vec3, ...)->float
/// <raymarching> @function f_lighting (vec3, vec3, vec3, float, vec3, vec3)->vec3
/////////////////////////////////////////////////

const int MAX_MARCHING_STEPS = 255;
const float MIN_DIST = 0.0;
const float MAX_DIST = 100.0;
const float EPSILON = 0.0001;

float shortest_distance_to_surface(vec3 eye, vec3 marchingDirection, float start, float end)
{
    float depth = start;
    for (int i = 0; i < MAX_MARCHING_STEPS; i++) {
        float dist = f_scene(eye + depth * marchingDirection);
        if (dist < EPSILON) {
			return depth;
        }
        depth += dist;
        if (depth >= end) {
            return end;
        }
    }
    return end;
}

vec3 ray_dir(float fov, vec2 size, vec2 frag_coord)
{
    vec2 xy = frag_coord - size / 2.0;
    float z = size.y / tan(radians(fov) / 2.0);
    return normalize(vec3(xy, -z));
}

mat3 viewMatrix(vec3 eye, vec3 center, vec3 up)
{
    // Based on gluLookAt man page
    vec3 f = normalize(center - eye);
    vec3 s = normalize(cross(f, up));
    vec3 u = cross(s, f);
    return mat3(s, u, -f);
}

vec3 raymarching(vec2 st)
{
    vec3 dir = ray_dir(45.0, vec2(1, 1), st);
    vec3 eye = vec3(0.0, 0.0, 7.0);
    float dist = shortest_distance_to_surface(eye, dir, MIN_DIST, MAX_DIST);

    if (dist > MAX_DIST - EPSILON) {
        // Didn't hit anything
        return vec3(0.0, 0.0, 0.0);
    }

    // The closest point on the surface to the eyepoint along the view ray
    vec3 p = eye + dist * dir;

    vec3 K_a = vec3(0.2, 0.2, 0.2);
    vec3 K_d = vec3(0.7, 0.2, 0.2);
    vec3 K_s = vec3(1.0, 1.0, 1.0);
    float shininess = 10.0;

    vec3 color = f_lighting(K_a, K_d, K_s, shininess, p, eye);

    return color;
}

)") {}

    virtual std::string GetHeader(const Evaluator& eval) const { return R"(

const int MAX_MARCHING_STEPS = 255;
const float MIN_DIST = 0.0;
const float MAX_DIST = 100.0;
const float EPSILON = 0.0001;

)"; }

    RTTR_ENABLE(Block)

}; // Raymarching

}
}