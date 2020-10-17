#pragma once

#include "shadergraph/Block.h"

namespace shadergraph
{
namespace block
{

class SampleTexture2D : public Block
{
public:
    SampleTexture2D()
    {
        SetupPorts({
            { VarType::Sampler2D, "tex" },
            { VarType::Float2,    "uv" },
        }, {
            { VarType::Float4, "rgba" },
            { VarType::Float3, "rgb" },
            { VarType::Float,  "r" },
            { VarType::Float,  "g" },
            { VarType::Float,  "b" },
            { VarType::Float,  "a" },
        });
    }

    enum class Output
    {
        RGBA,
        RGB,
        R,
        G,
        B,
        A,
    };

    virtual std::string GetBody(const Evaluator& eval) const override
    {
		std::string ret;

		ret += "vec4 #rgba# = texture(#tex#, #uv#);\n";
        ret += "vec3 #rgb# = #rgba#.rgb;\n";

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

		return ret;
    }

    RTTR_ENABLE(Block)

}; // SampleTexture2D

}
}