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

    void SetVarType(const VarType& type) { m_var_type = type; }
    const VarType& GetVarType() const { return m_var_type; }

    void SetVarName(const std::string& name) {
        m_var_name = name;
        m_exports[0].var.type.name = name;
    }
    const std::string& GetVarName() const { return m_var_name; }

    RTTR_ENABLE(Block)

#define NOT_GEN_SETTER_GETTER_FUNC
#define PARM_FILEPATH "shadergraph/block/Input.parm.h"
#include <dag/node_parms_gen.h>
#undef PARM_FILEPATH
#undef NOT_GEN_SETTER_GETTER_FUNC

}; // Input

}
}