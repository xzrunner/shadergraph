#pragma once

#include "shadergraph/Block.h"
#include "shadergraph/ParserUtility.h"

#include <SM_Vector.h>
#include <cpputil/StringHelper.h>

namespace shadergraph
{
namespace block
{

class Float3 : public Block
{
public:
    Float3()
    {
        SetupPorts({
        }, {
            { VarType::Float3, "f3" }
        });
    }

    virtual std::string GetBody(const Evaluator& eval) const override
    {
        auto& exports = GetExports();
        assert(exports.size() == 1);
        auto& out = GetExports()[0].var;
        return cpputil::StringHelper::Format(
            "%s #%s# = vec3(%f, %f, %f);",
            TypeToString(out.type.type).c_str(),
            out.type.name.c_str(),
            m_val.x, m_val.y, m_val.z
        );
    }

    RTTR_ENABLE(Block)

#define PARM_FILEPATH "shadergraph/block/Float3.parm.h"
#include <dag/node_parms_gen.h>
#undef PARM_FILEPATH

}; // Float3

}
}