#pragma once

#include "shadergraph/Block.h"

namespace shadergraph
{
namespace block
{

class FragmentShader : public Block
{
public:
    FragmentShader()
    {
        SetupPorts({
            { VarType::Float, "r", "0.0" },
            { VarType::Float, "g", "0.0" },
            { VarType::Float, "b", "0.0" },
            { VarType::Float, "a", "1.0" }
        }, {
            { VarType::Float4, "col", "FragColor" }
        });
    }

    virtual std::string GetHeader() const override {
        return "out vec4 #col#;";
    }
    virtual std::string GetBody() const override {
        return "#col# = vec4(#r#, #g#, #b#, #a#);";
    }

    RTTR_ENABLE(Block)

}; // FragmentShader

}
}