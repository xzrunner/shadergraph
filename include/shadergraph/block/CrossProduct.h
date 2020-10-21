#pragma once

#include "shadergraph/Block.h"

namespace shadergraph
{
namespace block
{

class CrossProduct : public Block
{
public:
    CrossProduct()
    {
        SetupPorts({
            { VarType::Float3, "a" },
            { VarType::Float3, "b" }
        }, {
            { VarType::Float3, "ret" }
        });
    }
    virtual std::string GetBody(const Evaluator& eval) const override
    {
        return "vec3 #ret# = cross(#a#, #b#);";
    }

    RTTR_ENABLE(Block)

}; // CrossProduct

}
}