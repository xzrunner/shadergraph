#pragma once

#include "shadergraph/Block.h"
#include "shadergraph/ParserUtility.h"
#include "shadergraph/Evaluator.h"

#include <cpputil/StringHelper.h>
#include <SM_Vector.h>

namespace shadergraph
{
namespace block
{

class ChannelMask : public Block
{
public:
    ChannelMask()
    {
        SetupPorts({
            { VarType::Dynamic, "_in" },
        }, {
            { VarType::Dynamic, "_out" },
        });
    }

    virtual std::string GetHeader(const Evaluator& eval) const
    {
        return cpputil::StringHelper::Format(R"(
// https://docs.unity3d.com/Packages/com.unity.shadergraph@9.0/manual/Channel-Mask-Node.html
vec4 channel_mask(vec4 rgba)
{
    return vec4(%s, %s, %s, %s);
}
)", m_r ? "rgba.r" : "0.0", m_g ? "rgba.g" : "0.0", m_b ? "rgba.b" : "0.0", m_a ? "rgba.a" : "0.0");
    }

    virtual std::string GetBody(const Evaluator& eval) const override
    {
        std::string ret;
        auto type = eval.QueryRealType(&m_imports[0].var.type);
        switch (type)
        {
        case VarType::Float:
            ret = "%s #_out# = channel_mask(vec4(#_in#, 0.0, 0.0, 0.0)).r;";
            break;
        case VarType::Float2:
            ret = "%s #_out# = channel_mask(vec4(#_in#, 0.0, 0.0)).rg;";
            break;
        case VarType::Float3:
            ret = "%s #_out# = channel_mask(vec4(#_in#, 0.0)).rgb;";
            break;
        default:
            ret = "%s #_out# = channel_mask(#_in#);";
        }
        return cpputil::StringHelper::Format(ret, TypeToString(type).c_str());
    }

    RTTR_ENABLE(Block)

#define PARM_FILEPATH "shadergraph/block/ChannelMask.parm.h"
#include <dag/node_parms_gen.h>
#undef PARM_FILEPATH

}; // ChannelMask

}
}