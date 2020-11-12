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
        if (!m_exports[static_cast<int>(Output::RGBA)].conns.empty()) {
            ret += "vec4 #rgba# = texture(#tex#, #uv#);\n";
        }
        if (!m_exports[static_cast<int>(Output::RGB)].conns.empty()) {
            if (!m_exports[static_cast<int>(Output::RGBA)].conns.empty()) {
                ret += "vec3 #rgb# = #rgba#.rgb;\n";
            } else {
                ret += "vec3 #rgb# = texture(#tex#, #uv#).rgb;\n";
            }
        }
		if (!m_exports[static_cast<int>(Output::R)].conns.empty()) {
            if (!m_exports[static_cast<int>(Output::RGBA)].conns.empty()) {
                ret += "float #r# = #rgba#.r;\n";
            } else {
                ret += "float #r# = texture(#tex#, #uv#).r;\n";
            }
		}
		if (!m_exports[static_cast<int>(Output::G)].conns.empty()) {
            if (!m_exports[static_cast<int>(Output::RGBA)].conns.empty()) {
                ret += "float #g# = #rgba#.g;\n";
            } else {
                ret += "float #g# = texture(#tex#, #uv#).g;\n";
            }
		}
		if (!m_exports[static_cast<int>(Output::B)].conns.empty()) {
            if (!m_exports[static_cast<int>(Output::RGBA)].conns.empty()) {
                ret += "float #b# = #rgba#.b;\n";
            } else {
                ret += "float #b# = texture(#tex#, #uv#).b;\n";
            }
		}
		if (!m_exports[static_cast<int>(Output::A)].conns.empty()) {
            if (!m_exports[static_cast<int>(Output::RGBA)].conns.empty()) {
                ret += "float #a# = #rgba#.a;\n";
            } else {
                ret += "float #a# = texture(#tex#, #uv#).a;\n";
            }
		}
		return ret;
    }

    RTTR_ENABLE(Block)

}; // SampleTexture2D

}
}