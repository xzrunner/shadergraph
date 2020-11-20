#pragma once

#include "shadergraph/Block.h"

#include <cpputil/StringHelper.h>

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
            { VarType::Dynamic, "_in" }
        }, {
            { VarType::Dynamic, "_out" }
        });
    }

    //virtual std::string GetHeader(const Evaluator& eval) const override
    //{
    //    std::string ret;
    //    if (m_var_type != VarType::Invalid) {
    //        ret = cpputil::StringHelper::Format(
    //            "uniform %s %s;", TypeToString(m_var_type).c_str(), m_var_name.c_str()
    //        );
    //    }
    //    return ret;
    //}

    void SetVarType(const VarType& type);
    const VarType& GetVarType() const { return m_var_type; }

    void SetVarName(const std::string& name);
    const std::string& GetVarName() const { return m_var_name; }

private:
    void ResetVariants();

    RTTR_ENABLE(Block)

#define NOT_GEN_SETTER_GETTER_FUNC
#define PARM_FILEPATH "shadergraph/block/Input.parm.h"
#include <dag/node_parms_gen.h>
#undef PARM_FILEPATH
#undef NOT_GEN_SETTER_GETTER_FUNC

}; // Input

}
}