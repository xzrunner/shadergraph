#pragma once

#include "shadergraph/Block.h"
#include "shadergraph/ParserUtility.h"

#include <cpputil/StringHelper.h>

namespace shadergraph
{
namespace block
{

class Float : public Block
{
public:
    Float()
    {
        SetupPorts({
        }, {
            { VarType::Float, "f" }
        });
    }

    virtual std::string GetBody() const
    {
        auto& exports = GetExports();
        assert(exports.size() == 1);
        auto& out = GetExports()[0].var;
        return cpputil::StringHelper::Format(
            "%s #%s# = %f;",
            TypeToString(out.type.type).c_str(),
            out.type.name.c_str(),
            m_val
        );
    }

    RTTR_ENABLE(Block)

#define PARM_FILEPATH "shadergraph/block/Float.parm.h"
#include <dag/node_parms_gen.h>
#undef PARM_FILEPATH

}; // Float

}
}