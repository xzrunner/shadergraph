#pragma once

#include "shadergraph/Block.h"

namespace shadergraph
{
namespace block
{

class NormalUnpack : public Block
{
public:
    NormalUnpack() : Block(R"(

vec3 normal_unpack(vec4 _in)
{
    vec3 _out;
    vec4 packed_normal = _in;
    packed_normal.w *= packed_normal.x;
    _out.xy = packed_normal.wy * 2.0 - 1.0;
    _out.z = sqrt(1.0 - clamp(dot(_out.xy, _out.xy), 0.0, 1.0));
    return _out;
}

)") {}

    RTTR_ENABLE(Block)

}; // NormalUnpack

}
}