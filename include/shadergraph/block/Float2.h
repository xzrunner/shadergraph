#pragma once

#include "shadergraph/Block.h"
#include "shadergraph/ParserUtility.h"

#include <SM_Vector.h>
#include <cpputil/StringHelper.h>

namespace shadergraph
{
namespace block
{

class Float2 : public Block
{
public:
    Float2()
    {
        SetupPorts({
        }, {
            { VarType::Float2, "f2" }
        });
    }

    virtual std::string GetBody(const Evaluator& eval) const override
    {
        auto& exports = GetExports();
        assert(exports.size() == 1);
        auto& out = GetExports()[0].var;
        return cpputil::StringHelper::Format(
            "%s #%s# = vec2(%f, %f);",
            TypeToString(out.type.type).c_str(),
            out.type.name.c_str(),
            m_val.x, m_val.y
        );
    }

    RTTR_ENABLE(Block)

#define PARM_FILEPATH "shadergraph/block/Float2.parm.h"
#include <dag/node_parms_gen.h>
#undef PARM_FILEPATH

}; // Float2

}
}