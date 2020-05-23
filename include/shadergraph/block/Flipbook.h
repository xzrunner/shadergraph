#pragma once

#include "shadergraph/Block.h"

namespace shadergraph
{
namespace block
{

class Flipbook : public Block
{
public:
    Flipbook() : Block(R"(

// https://stackoverflow.com/questions/7610631/glsl-mod-vs-hlsl-fmod
float fmod(float x, float y)
{
    //return x ¨C y * trunc(x/y);
    return x - y * floor(x/y);
}

// https://docs.unity3d.com/Packages/com.unity.shadergraph@9.0/manual/Flipbook-Node.html
/////////////////////////////////////////////////
/// @export flipbook
/////////////////////////////////////////////////
vec2 flipbook(vec2 uv, float width, float height, float tile, vec2 invert)
{
    tile = fmod(tile, width * height);
    vec2 tile_count = vec2(1.0, 1.0) / vec2(width, height);
    float tile_y = abs(invert.y * height - (floor(tile * tile_count.x) + invert.y * 1));
    float tile_x = abs(invert.x * width - ((tile - width * floor(tile * tile_count.x)) + invert.x * 1));
    return (uv + vec2(tile_x, tile_y)) * tile_count;
}

)") {}

    RTTR_ENABLE(Block)

}; // Flipbook

}
}