#pragma once

#include "shadergraph/Block.h"

#include <cpputil/StringHelper.h>
#include <SM_Vector.h>

namespace shadergraph
{
namespace block
{

class ChannelMixer : public Block
{
public:
    ChannelMixer()
    {
        SetupPorts({
            { VarType::Float3, "_in" },
        }, {
            { VarType::Float3, "_out" },
        });
    }

    virtual std::string GetHeader() const
    {
        return cpputil::StringHelper::Format(R"(
// https://docs.unity3d.com/Packages/com.unity.shadergraph@9.0/manual/Channel-Mixer-Node.html
vec3 channel_mixer(vec3 rgb)
{
    return vec3(
        dot(rgb, vec3(%f, %f, %f)),
        dot(rgb, vec3(%f, %f, %f)),
        dot(rgb, vec3(%f, %f, %f))
    );
}
)", m_r.x, m_r.y, m_r.z, m_g.x, m_g.y, m_g.z, m_b.x, m_b.y, m_b.z);
    }

    virtual std::string GetBody() const override
    {
        return "vec3 #_out# = channel_mixer(#_in#);";
    }

    RTTR_ENABLE(Block)

#define PARM_FILEPATH "shadergraph/block/ChannelMixer.parm.h"
#include <dag/node_parms_gen.h>
#undef PARM_FILEPATH

}; // ChannelMixer

}
}