#pragma once

#include "shadergraph/Block.h"
#include "shadergraph/ValueImpl.h"

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
            { VarType::Float, "r" },
            { VarType::Float, "g" },
            { VarType::Float, "b" },
            { VarType::Float, "a" }
        }, {
            { VarType::Float4, "rgba" },
            { VarType::Float3, "rgb" },
            { VarType::Float2, "rg" }
        });

        m_default_in_vals[0] = Variant(VarType::Float, "", std::make_shared<FloatVal>(0.0f));
        m_default_in_vals[1] = Variant(VarType::Float, "", std::make_shared<FloatVal>(0.0f));
        m_default_in_vals[2] = Variant(VarType::Float, "", std::make_shared<FloatVal>(0.0f));
        m_default_in_vals[3] = Variant(VarType::Float, "", std::make_shared<FloatVal>(1.0f));
    }

    enum class Output
    {
        RGBA,
        RGB,
        RG,
    };

    virtual std::string GetBody(const Evaluator& eval) const override
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