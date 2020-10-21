#pragma once

#include "shadergraph/Block.h"
#include "shadergraph/ValueImpl.h"

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
            { VarType::Float4, "rgba"},
            { VarType::Float3, "rgb"},
            { VarType::Float2, "rg"},
            { VarType::Float,  "grey"}
        }, {
            { VarType::Float4, "col" }
        });

        m_default_in_vals[0] = Variant(VarType::Float4, "", std::make_shared<Float4Val>(0.0f, 0.0f, 0.0f, 1.0f));
        m_default_in_vals[1] = Variant(VarType::Float3, "", std::make_shared<Float3Val>(0.0f, 0.0f, 0.0f));
        m_default_in_vals[2] = Variant(VarType::Float2, "", std::make_shared<Float2Val>(0.0f, 0.0f));
        m_default_in_vals[3] = Variant(VarType::Float, "", std::make_shared<FloatVal>(0.0f));

        m_default_out_vals[0] = Variant(VarType::String, "", std::make_shared<StringVal>("FragColor"));
    }

    enum class Input
    {
        RGBA,
        RGB,
        RG,
        Grey,
    };

    virtual std::string GetHeader(const Evaluator& eval) const override {
        return "out vec4 #col#;";
    }
    virtual std::string GetBody(const Evaluator& eval) const override
    {
        std::string ret;
        if (!m_imports[static_cast<int>(Input::RGBA)].conns.empty()) {
            ret = "#col# = vec4(#rgba#);";
        } else if (!m_imports[static_cast<int>(Input::RGB)].conns.empty()) {
            ret = "#col# = vec4(#rgb#, 1.0);";
        } else if (!m_imports[static_cast<int>(Input::RG)].conns.empty()) {
            ret = "#col# = vec4(#rg#, 0.0, 1.0);";
        } else if (!m_imports[static_cast<int>(Input::Grey)].conns.empty()) {
            ret = "#col# = vec4(#grey#, #grey#, #grey#, 1.0);";
        }
        return ret;
    }

    RTTR_ENABLE(Block)

}; // FragmentShader

}
}