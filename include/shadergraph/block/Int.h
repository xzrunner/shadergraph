#pragma once

#include "shadergraph/Block.h"
#include "shadergraph/ParserUtility.h"

#include <cpputil/StringHelper.h>

namespace shadergraph
{
namespace block
{

class Int : public Block
{
public:
    Int()
    {
        SetupPorts({
        }, {
            { VarType::Int, "i" }
        });
    }

    virtual std::string GetBody() const
    {
        auto& exports = GetExports();
        assert(exports.size() == 1);
        auto& out = GetExports()[0].var;
        return cpputil::StringHelper::Format(
            "%s #%s# = %d;",
            TypeToString(out.type.type).c_str(),
            out.type.name.c_str(),
            m_val
        );
    }

    RTTR_ENABLE(Block)

#define PARM_FILEPATH "shadergraph/block/Int.parm.h"
#include <dag/node_parms_gen.h>
#undef PARM_FILEPATH

}; // Int

}
}