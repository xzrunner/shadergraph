#pragma once

#include "shadergraph/Block.h"

namespace shadergraph
{
namespace block
{

class VertToFrag : public Block
{
public:
    VertToFrag()
    {
        SetupPorts({
            { VarType::Dynamic, "_in" }
        }, {
            { VarType::Dynamic, "_out" }
        });
    }

    RTTR_ENABLE(Block)

#define PARM_FILEPATH "shadergraph/block/VertToFrag.parm.h"
#include <dag/node_parms_gen.h>
#undef PARM_FILEPATH

}; // VertToFrag

}
}