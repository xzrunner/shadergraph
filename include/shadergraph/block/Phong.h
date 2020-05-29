#pragma once

#include "shadergraph/Block.h"

namespace shadergraph
{
namespace block
{

class Phong : public Block
{
public:
    Phong() : Block(R"(

/////////////////////////////////////////////////
/// <phongIllumination> @export
/// <phongIllumination> @function  f_scene (vec3)->float  Scene
/////////////////////////////////////////////////

const float EPSILON = 0.0001;

vec3 estimateNormal(vec3 p) {
    return normalize(vec3(
        f_scene(vec3(p.x + EPSILON, p.y, p.z)) - f_scene(vec3(p.x - EPSILON, p.y, p.z)),
        f_scene(vec3(p.x, p.y + EPSILON, p.z)) - f_scene(vec3(p.x, p.y - EPSILON, p.z)),
        f_scene(vec3(p.x, p.y, p.z  + EPSILON)) - f_scene(vec3(p.x, p.y, p.z - EPSILON))
    ));
}

vec3 phongContribForLight(vec3 k_d, vec3 k_s, float alpha, vec3 p, vec3 eye,
                          vec3 lightPos, vec3 lightIntensity) {
    vec3 N = estimateNormal(p);
    vec3 L = normalize(lightPos - p);
    vec3 V = normalize(eye - p);
    vec3 R = normalize(reflect(-L, N));

    float dotLN = dot(L, N);
    float dotRV = dot(R, V);

    if (dotLN < 0.0) {
        // Light not visible from this point on the surface
        return vec3(0.0, 0.0, 0.0);
    }

    if (dotRV < 0.0) {
        // Light reflection in opposite direction as viewer, apply only diffuse
        // component
        return lightIntensity * (k_d * dotLN);
    }
    return lightIntensity * (k_d * dotLN + k_s * pow(dotRV, alpha));
}

vec3 phongIllumination(vec3 k_a, vec3 k_d, vec3 k_s, float alpha, vec3 p, vec3 eye) {
    const vec3 ambientLight = 0.5 * vec3(1.0, 1.0, 1.0);
    vec3 color = ambientLight * k_a;

    float iTime = 0;

    vec3 light1Pos = vec3(4.0 * sin(iTime),
                          2.0,
                          4.0 * cos(iTime));
    vec3 light1Intensity = vec3(0.4, 0.4, 0.4);

    color += phongContribForLight(k_d, k_s, alpha, p, eye,
                                  light1Pos,
                                  light1Intensity);

    vec3 light2Pos = vec3(2.0 * sin(0.37 * iTime),
                          2.0 * cos(0.37 * iTime),
                          2.0);
    vec3 light2Intensity = vec3(0.4, 0.4, 0.4);

    color += phongContribForLight(k_d, k_s, alpha, p, eye,
                                  light2Pos,
                                  light2Intensity);
    return color;
}

)") {}

    RTTR_ENABLE(Block)

}; // Phong

}
}