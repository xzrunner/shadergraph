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
            { VarType::Float4, "rgba", "vec4(0, 0, 0, 1)" },
            { VarType::Float3, "rgb",  "vec3(0, 0, 0)" },
            { VarType::Float,  "grey", "0" }
        }, {
            { VarType::Float4, "col", "FragColor" }
        });
    }

    enum class Input
    {
        RGBA,
        RGB,
        Grey,
    };

    virtual std::string GetHeader() const override {
        return "out vec4 #col#;";
    }
    virtual std::string GetBody() const override
    {
        std::string ret;
        if (!m_imports[static_cast<int>(Input::RGBA)].conns.empty()) {
            ret = "#col# = vec4(#rgba#);";
        } else if (!m_imports[static_cast<int>(Input::RGB)].conns.empty()) {
            ret = "#col# = vec4(#rgb#, 1.0);";
        } else if (!m_imports[static_cast<int>(Input::Grey)].conns.empty()) {
            ret = "#col# = vec4(#grey#, #grey#, #grey#, 1.0);";
        }
        return ret;
    }

    RTTR_ENABLE(Block)

}; // FragmentShader

}
}