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
            { VarType::Float4, "in0" },
        }, {
            { VarType::Float, "r" },
            { VarType::Float, "g" },
            { VarType::Float, "b" },
            { VarType::Float, "a" }
        });
    }

    enum class Output
    {
        R,
        G,
        B,
        A,
    };

    virtual std::string GetBody() const override
    {
		std::string ret;
		if (!m_exports[static_cast<int>(Output::R)].conns.empty()) {
			ret += "float #r# = #in0#.r;\n";
		}
		if (!m_exports[static_cast<int>(Output::G)].conns.empty()) {
			ret += "float #g# = #in0#.g;\n";
		}
		if (!m_exports[static_cast<int>(Output::B)].conns.empty()) {
			ret += "float #b# = #in0#.b;\n";
		}
		if (!m_exports[static_cast<int>(Output::A)].conns.empty()) {
			ret += "float #a# = #in0#.a;\n";
		}
		return ret;
    }

    RTTR_ENABLE(Block)

}; // Split

}
}