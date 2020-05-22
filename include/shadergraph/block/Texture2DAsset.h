#pragma once

#include "shadergraph/Block.h"

namespace shadergraph
{
namespace block
{

class Texture2DAsset : public Block
{
public:
    Texture2DAsset()
    {
        SetupPorts({
        }, {
            { VarType::Sampler2D, "tex" },
        });
    }

    virtual std::string GetHeader(const Evaluator& eval) const override
    {
        return "uniform sampler2D #tex#;";
    }

    RTTR_ENABLE(Block)

#define PARM_FILEPATH "shadergraph/block/Texture2DAsset.parm.h"
#include <dag/node_parms_gen.h>
#undef PARM_FILEPATH

}; // Texture2DAsset

}
}