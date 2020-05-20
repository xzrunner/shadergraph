#pragma once

#include "shadergraph/Block.h"

#include <cpputil/StringHelper.h>
#include <SM_Vector.h>

namespace shadergraph
{
namespace block
{

class InvertColors : public Block
{
public:
    InvertColors()
    {
        SetupPorts({
            { VarType::Float4, "_in" },
        }, {
            { VarType::Float4, "_out" },
        });
    }

    virtual std::string GetHeader(const Evaluator& eval) const
    {
        return cpputil::StringHelper::Format(R"(
// https://docs.unity3d.com/Packages/com.unity.shadergraph@9.0/manual/Invert-Colors-Node.html
vec4 invert_colors(vec4 rgba)
{
    return abs(vec4(%f, %f, %f, %f) - rgba);
}
)", m_inv_col.x, m_inv_col.y, m_inv_col.z, m_inv_col.w);
    }

    virtual std::string GetBody(const Evaluator& eval) const override
    {
        return "vec4 #_out# = invert_colors(#_in#);";
    }

    RTTR_ENABLE(Block)

#define PARM_FILEPATH "shadergraph/block/InvertColors.parm.h"
#include <dag/node_parms_gen.h>
#undef PARM_FILEPATH

}; // InvertColors

}
}