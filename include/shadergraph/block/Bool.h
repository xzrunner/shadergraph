#pragma once

#include "shadergraph/Block.h"
#include "shadergraph/ParserUtility.h"

#include <cpputil/StringHelper.h>

namespace shadergraph
{
namespace block
{

class Bool : public Block
{
public:
    Bool()
    {
        SetupPorts({
        }, {
            { VarType::Bool, "b" }
        });
    }

    virtual std::string GetBody() const
    {
        std::string s_val = m_val ? "true" : "false";
        return "bool #b# = " + s_val + ";";
    }

    RTTR_ENABLE(Block)

#define PARM_FILEPATH "shadergraph/block/Bool.parm.h"
#include <dag/node_parms_gen.h>
#undef PARM_FILEPATH

}; // Bool

}
}