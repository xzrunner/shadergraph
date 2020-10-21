#pragma once

#include "shadergraph/Block.h"

namespace shadergraph
{
namespace block
{

class DotProduct : public Block
{
public:
    DotProduct()
    {
        SetupPorts({
            { VarType::Float3, "a" },
            { VarType::Float3, "b" }
        }, {
            { VarType::Float, "ret" }
        });
    }
    virtual std::string GetBody(const Evaluator& eval) const override
    {
        return "float #ret# = dot(#a#, #b#);";
    }

    RTTR_ENABLE(Block)

}; // DotProduct

}
}