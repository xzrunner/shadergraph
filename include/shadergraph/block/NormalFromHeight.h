#pragma once

#include "shadergraph/Block.h"

namespace shadergraph
{
namespace block
{

class NormalFromHeight : public Block
{
public:
    NormalFromHeight() : Block(R"(

vec3 compute_normal_sobel_filter(sampler2D heightmap, vec2 position, float height_exaggeration)
{
    vec2 tex_sz_inv = 1.0 / textureSize(heightmap, 0);
    float upperLeft = texture(heightmap, position + vec2(-1.0, 1.0) * tex_sz_inv).r * height_exaggeration;
    float upperCenter = texture(heightmap, position + vec2(0.0, 1.0) * tex_sz_inv).r * height_exaggeration;
    float upperRight = texture(heightmap, position + vec2(1.0, 1.0) * tex_sz_inv).r * height_exaggeration;
    float left = texture(heightmap, position + vec2(-1.0, 0.0) * tex_sz_inv).r * height_exaggeration;
    float right = texture(heightmap, position + vec2(1.0, 0.0) * tex_sz_inv).r * height_exaggeration;
    float lowerLeft = texture(heightmap, position + vec2(-1.0, -1.0) * tex_sz_inv).r * height_exaggeration;
    float lowerCenter = texture(heightmap, position + vec2(0.0, -1.0) * tex_sz_inv).r * height_exaggeration;
    float lowerRight = texture(heightmap, position + vec2(1.0, -1.0) * tex_sz_inv).r * height_exaggeration;

    float x = upperRight + (2.0 * right) + lowerRight - upperLeft - (2.0 * left) - lowerLeft;
    float y = lowerLeft + (2.0 * lowerCenter) + lowerRight - upperLeft - (2.0 * upperCenter) - upperRight;

    return normalize(vec3(-x, 1.0, y));
}

vec3 compute_normal_central_difference(sampler2D heightmap, vec2 position, float height_exaggeration)
{
    vec2 tex_sz_inv = 1.0 / textureSize(heightmap, 0);
    float leftHeight = texture(heightmap, position - vec2(1.0, 0.0) * tex_sz_inv).r * height_exaggeration;
    float rightHeight = texture(heightmap, position + vec2(1.0, 0.0) * tex_sz_inv).r * height_exaggeration;
    float bottomHeight = texture(heightmap, position - vec2(0.0, 1.0) * tex_sz_inv).r * height_exaggeration;
    float topHeight = texture(heightmap, position + vec2(0.0, 1.0) * tex_sz_inv).r * height_exaggeration;
    return normalize(vec3(leftHeight - rightHeight, 2.0, bottomHeight - topHeight));
}

vec3 normal_from_height(sampler2D heightmap, vec2 texcoord, float height_exaggeration)
{
//    return compute_normal_sobel_filter(heightmap, texcoord, height_exaggeration);
    return compute_normal_central_difference(heightmap, texcoord, height_exaggeration);
}

)") {}

    RTTR_ENABLE(Block)

}; // NormalFromHeight

}
}