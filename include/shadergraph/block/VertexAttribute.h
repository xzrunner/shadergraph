#pragma once

#include "shadergraph/Block.h"
#include "shadergraph/ParserUtility.h"

#include <cpputil/StringHelper.h>

namespace shadergraph
{
namespace block
{

class VertexAttribute : public Block
{
public:
    VertexAttribute()
    {
        SetupPorts({
        }, {
            { VarType::Dynamic, "_out" }
        });
    }

    virtual std::string GetHeader(const Evaluator& eval) const override 
    { 
        auto type_str = TypeToString(m_var_type);
        return cpputil::StringHelper::Format(
            "layout (location = %d) in %s %s;", 
            m_index, type_str.c_str(), m_var_name.c_str()
        );
    }

    void SetIndex(int index) { m_index = index; }
    int GetIndex() const { return m_index; }

    void SetVarType(const VarType& type) 
    { 
        m_exports[0].var.type.type = type;
        m_var_type = type; 
    }
    const VarType& GetVarType() const { return m_var_type; }

    void SetVarName(const std::string& name) { m_var_name = name; }
    const std::string& GetVarName() const { return m_var_name; }

    RTTR_ENABLE(Block)

#define NOT_GEN_SETTER_GETTER_FUNC
#define PARM_FILEPATH "shadergraph/block/VertexAttribute.parm.h"
#include <dag/node_parms_gen.h>
#undef PARM_FILEPATH
#undef NOT_GEN_SETTER_GETTER_FUNC

}; // VertexAttribute

}
}