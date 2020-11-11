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

    RTTR_ENABLE(Block)

#define PARM_FILEPATH "shadergraph/block/VertexAttribute.parm.h"
#include <dag/node_parms_gen.h>
#undef PARM_FILEPATH

}; // VertexAttribute

}
}