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
            { VarType::Float4, "rgba", "vec4(0, 0, 0, 1)" }
        }, {
            { VarType::Float4, "col", "FragColor" }
        });
    }

    virtual std::string GetHeader() const override {
        return "out vec4 #col#;";
    }
    virtual std::string GetBody() const override {
        return "#col# = vec4(#rgba#);";
    }

    RTTR_ENABLE(Block)

}; // FragmentShader

}
}