#pragma once

#include "shadergraph/Block.h"
#include "shadergraph/Evaluator.h"
#include "shadergraph/ParserUtility.h"

#include <cpputil/StringHelper.h>

namespace shadergraph
{
namespace block
{

class Length : public Block
{
public:
    Length()
    {
        SetupPorts({
            { VarType::Dynamic, "_in" },
        }, {
            { VarType::Float, "_out" }
        });
    }
    virtual std::string GetBody(const Evaluator& eval) const override
    {
        return "float #_out# = length(#_in#);";
    }

    RTTR_ENABLE(Block)

}; // Length

}
}