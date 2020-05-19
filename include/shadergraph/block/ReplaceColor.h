#pragma once

#include "shadergraph/Block.h"

namespace shadergraph
{
namespace block
{

class ReplaceColor : public Block
{
public:
    ReplaceColor() : Block(R"(

// https://docs.unity3d.com/Packages/com.unity.shadergraph@9.0/manual/Replace-Color-Node.html
vec3 replace_color(vec3 rgb, vec3 from, vec3 to, float range, float fuzziness)
{
    float distance = distance(from, rgb);
    return mix(to, rgb, clamp((distance - range) / max(fuzziness, 1e-5f), 0.0, 1.0));
}

)") {}


    RTTR_ENABLE(Block)

}; // ReplaceColor

}
}