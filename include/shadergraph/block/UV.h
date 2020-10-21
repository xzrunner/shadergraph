#pragma once

#include "shadergraph/Block.h"
#include "shadergraph/ValueImpl.h"

namespace shadergraph
{
namespace block
{

class UV : public Block
{
public:
    UV()
    {
        SetupPorts({
        }, {
            { VarType::Float2, "uv"}
        });

        m_default_out_vals[0] = Variant(VarType::String, "", std::make_shared<StringVal>("TexCoord"));
    }

    virtual std::string GetHeader(const Evaluator& eval) const override {
        return "in vec2 #uv#;";
    }

    RTTR_ENABLE(Block)

}; // UV

}
}