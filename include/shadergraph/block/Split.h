#pragma once

#include "shadergraph/Block.h"

namespace shadergraph
{
namespace block
{

class Split : public Block
{
public:
    Split()
    {
        SetupPorts({
            { VarType::Float2, "rg" },
            { VarType::Float3, "rgb" },
            { VarType::Float4, "rgba" },
        }, {
            { VarType::Float, "r" },
            { VarType::Float, "g" },
            { VarType::Float, "b" },
            { VarType::Float, "a" }
        });
    }

    enum class Input
    {
        RG,
        RGB,
        RGBA,
    };

    enum class Output
    {
        R,
        G,
        B,
        A,
    };

    virtual std::string GetBody(const Evaluator& eval) const override
    {
		std::string ret;
        if (!m_imports[static_cast<int>(Input::RGBA)].conns.empty())
        {
		    if (!m_exports[static_cast<int>(Output::R)].conns.empty()) {
			    ret += "float #r# = #rgba#.r;\n";
		    }
		    if (!m_exports[static_cast<int>(Output::G)].conns.empty()) {
			    ret += "float #g# = #rgba#.g;\n";
		    }
		    if (!m_exports[static_cast<int>(Output::B)].conns.empty()) {
			    ret += "float #b# = #rgba#.b;\n";
		    }
		    if (!m_exports[static_cast<int>(Output::A)].conns.empty()) {
			    ret += "float #a# = #rgba#.a;\n";
		    }
        }
        else if (!m_imports[static_cast<int>(Input::RGB)].conns.empty())
        {
            if (!m_exports[static_cast<int>(Output::R)].conns.empty()) {
                ret += "float #r# = #rgb#.r;\n";
            }
            if (!m_exports[static_cast<int>(Output::G)].conns.empty()) {
                ret += "float #g# = #rgb#.g;\n";
            }
            if (!m_exports[static_cast<int>(Output::B)].conns.empty()) {
                ret += "float #b# = #rgb#.b;\n";
            }
        }
        else if (!m_imports[static_cast<int>(Input::RG)].conns.empty())
        {
            if (!m_exports[static_cast<int>(Output::R)].conns.empty()) {
                ret += "float #r# = #rg#.r;\n";
            }
            if (!m_exports[static_cast<int>(Output::G)].conns.empty()) {
                ret += "float #g# = #rg#.g;\n";
            }
        }
		return ret;
    }

    RTTR_ENABLE(Block)

}; // Split

}
}