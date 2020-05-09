#pragma once

#include "shadergraph/Block.h"

namespace shadergraph
{
namespace block
{

class Combine : public Block
{
public:
    Combine()
    {
        SetupPorts({
            { VarType::Float, "r", "0" },
            { VarType::Float, "g", "0" },
            { VarType::Float, "b", "0" },
            { VarType::Float, "a", "1" }
        }, {
            { VarType::Float4, "rgba" },
            { VarType::Float3, "rgb" },
            { VarType::Float2, "rg" }
        });
    }

    enum class Output
    {
        RGBA,
        RGB,
        RG,
    };

    virtual std::string GetBody() const override
    {
        std::string ret;
        if (!m_exports[static_cast<int>(Output::RGBA)].conns.empty()) {
            ret += "vec4 #rgba# = vec4(#r#, #g#, #b#, #a#);";
        }
        if (!m_exports[static_cast<int>(Output::RGB)].conns.empty()) {
            ret += "vec3 #rgb# = vec3(#r#, #g#, #b#);";
        }
        if (!m_exports[static_cast<int>(Output::RG)].conns.empty()) {
            ret += "vec2 #rg# = vec2(#r#, #g#);";
        }
        return ret;
    }

    RTTR_ENABLE(Block)

}; // Combine

}
}