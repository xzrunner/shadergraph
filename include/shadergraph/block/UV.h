#pragma once

#include "shadergraph/Block.h"

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
            { VarType::Float2, "uv", "TexCoord" }
        });
    }

    virtual std::string GetHeader(const Evaluator& eval) const override {
        return "in vec2 #uv#;";
    }

    RTTR_ENABLE(Block)

}; // UV

}
}