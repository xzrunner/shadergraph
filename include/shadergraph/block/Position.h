#pragma once

#include "shadergraph/Block.h"
#include "shadergraph/ValueImpl.h"

namespace shadergraph
{
namespace block
{

class Position : public Block
{
public:
    Position()
    {
        SetupPorts({
        }, {
            { VarType::Float3, "pos"}
        });

        m_default_out_vals[0] = Variant(VarType::String, "", std::make_shared<StringVal>("frag_pos"));
    }

    virtual std::string GetHeader(const Evaluator& eval) const override {
        return "in vec3 #pos#;";
    }

    RTTR_ENABLE(Block)

}; // Position

}
}