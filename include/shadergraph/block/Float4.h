#pragma once

#include "shadergraph/Block.h"
#include "shadergraph/ParserUtility.h"

#include <SM_Vector.h>
#include <cpputil/StringHelper.h>

namespace shadergraph
{
namespace block
{

class Float4 : public Block
{
public:
    Float4()
    {
        SetupPorts({
        }, {
            { VarType::Float4, "f4" }
        });
    }

    virtual std::string GetBody(const Evaluator& eval) const override
    {
        auto& exports = GetExports();
        assert(exports.size() == 1);
        auto& out = GetExports()[0].var;
        return cpputil::StringHelper::Format(
            "%s #%s# = vec4(%f, %f, %f, %f);",
            TypeToString(out.type.type).c_str(),
            out.type.name.c_str(),
            m_val.x, m_val.y, m_val.z, m_val.w
        );
    }

    RTTR_ENABLE(Block)

#define PARM_FILEPATH "shadergraph/block/Float4.parm.h"
#include <dag/node_parms_gen.h>
#undef PARM_FILEPATH

}; // Float4

}
}