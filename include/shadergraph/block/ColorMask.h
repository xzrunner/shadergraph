#pragma once

#include "shadergraph/Block.h"

namespace shadergraph
{
namespace block
{

class ColorMask : public Block
{
public:
    ColorMask() : Block(R"(

// https://docs.unity3d.com/Packages/com.unity.shadergraph@9.0/manual/Color-Mask-Node.html
float color_mask(vec3 rgb, vec3 mask, float range, float fuzziness)
{
    float dist = distance(mask, rgb);
    return clamp(1 - (dist - range) / max(fuzziness, 1e-5), 0.0, 1.0);
}

)") {}


    RTTR_ENABLE(Block)

}; // ColorMask

}
}