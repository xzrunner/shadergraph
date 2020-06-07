#pragma once

#include "shadergraph/Block.h"

namespace shadergraph
{
namespace block
{

class Input : public Block
{
public:
    Input()
    {
        SetupPorts({
        }, {
            { VarType::Dynamic, "_out" }
        });
    }

    RTTR_ENABLE(Block)

#define PARM_FILEPATH "shadergraph/block/Input.parm.h"
#include <dag/node_parms_gen.h>
#undef PARM_FILEPATH

}; // Input

}
}