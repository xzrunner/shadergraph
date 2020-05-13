#pragma once

#include "shadergraph/Block.h"

namespace shadergraph
{
namespace block
{

class TilingAndOffset : public Block
{
public:
    TilingAndOffset() : Block(R"(

vec2 tiling_and_offset(vec2 uv, vec2 tiling, vec2 offset)
{
    return uv * tiling + offset;
}

)") {}

    RTTR_ENABLE(Block)

}; // TilingAndOffset

}
}