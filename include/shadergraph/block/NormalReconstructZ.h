#pragma once

#include "shadergraph/Block.h"

namespace shadergraph
{
namespace block
{

class NormalReconstructZ : public Block
{
public:
    NormalReconstructZ() : Block(R"(

// https://docs.unity3d.com/Packages/com.unity.shadergraph@9.0/manual/Normal-Reconstruct-Z-Node.html
vec3 normal_reconstruct_z(vec2 xy)
{
    float reconstructZ = sqrt(1.0 - clamp(dot(xy.xy, xy.xy), 0.0, 1.0));
    vec3 normalVector = vec3(xy.x, xy.y, reconstructZ);
    return normalize(normalVector);
}

)") {}


    RTTR_ENABLE(Block)

}; // NormalReconstructZ

}
}