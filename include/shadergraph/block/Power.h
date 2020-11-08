#pragma once

#include "shadergraph/Block.h"
#include "shadergraph/Evaluator.h"
#include "shadergraph/ParserUtility.h"

#include <cpputil/StringHelper.h>

namespace shadergraph
{
namespace block
{

class Power : public Block
{
public:
    Power()
    {
        SetupPorts({
            { VarType::Dynamic, "a" },
            { VarType::Dynamic, "b" }
        }, {
            { VarType::Dynamic, "_out" }
        });
    }
    virtual std::string GetBody(const Evaluator& eval) const override
    {
        auto type = eval.QueryRealType(&m_exports[0].var.type);
        return cpputil::StringHelper::Format("%s #_out# = pow(#a#, #b#);",
            TypeToString(type).c_str()
        );
    }

    RTTR_ENABLE(Block)

}; // Power

}
}