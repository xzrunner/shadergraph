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

class Flip : public Block
{
public:
    Flip()
    {
        SetupPorts({
            { VarType::Dynamic, "_in" },
        }, {
            { VarType::Dynamic, "_out" },
        });
    }

    virtual std::string GetHeader(const Evaluator& eval) const
    {
        return R"(
// https://docs.unity3d.com/Packages/com.unity.shadergraph@9.0/manual/Flip-Node.html
vec4 flip(vec4 _in, vec4 flips)
{
    return (flips * -2 + 1) * _in;
}
)";
    }

    virtual std::string GetBody(const Evaluator& eval) const override
    {
        std::string ret;

        auto type = eval.QueryRealType(&m_imports[0].var.type);
        switch (type)
        {
        case VarType::Float:
            ret = "%s #_out# = flip(vec4(#_in#, 0.0, 0.0, 0.0), %s).r;";
            break;
        case VarType::Float2:
            ret = "%s #_out# = flip(vec4(#_in#, 0.0, 0.0), %s).rg;";
            break;
        case VarType::Float3:
            ret = "%s #_out# = flip(vec4(#_in#, 0.0), %s).rgb;";
            break;
        default:
            ret = "%s #_out# = flip(#_in#, %s);";
        }

        auto flips_str = cpputil::StringHelper::Format("vec4(%f, %f, %f, %f)",
            m_flips.x ? 1.0 : 0.0, m_flips.y ? 1.0 : 0.0, m_flips.z ? 1.0 : 0.0, m_flips.w ? 1.0 : 0.0);

        return cpputil::StringHelper::Format(ret, TypeToString(type).c_str(), flips_str.c_str());

    }

    RTTR_ENABLE(Block)

#define PARM_FILEPATH "shadergraph/block/Flip.parm.h"
#include <dag/node_parms_gen.h>
#undef PARM_FILEPATH

}; // Flip

}
}