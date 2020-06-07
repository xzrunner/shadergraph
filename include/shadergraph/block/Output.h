#pragma once

#include "shadergraph/Block.h"

namespace shadergraph
{
namespace block
{

class Output : public Block
{
public:
    Output()
    {
        SetupPorts({
            { VarType::Dynamic, "_in" }
        }, {
        });
    }

    RTTR_ENABLE(Block)

#define PARM_FILEPATH "shadergraph/block/Output.parm.h"
#include <dag/node_parms_gen.h>
#undef PARM_FILEPATH

}; // Output

}
}