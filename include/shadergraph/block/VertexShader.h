#pragma once

#include "shadergraph/Block.h"
#include "shadergraph/ValueImpl.h"

namespace shadergraph
{
namespace block
{

class VertexShader : public Block
{
public:
    VertexShader()
    {
        SetupPorts({
            { VarType::Float4, "position" }
        }, {
        });
    }

    virtual std::string GetBody(const Evaluator& eval) const override
    {
        std::string ret;
        if (!m_imports[0].conns.empty()) {
            ret = "gl_Position = #position#;";
        }
        return ret;
    }

    RTTR_ENABLE(Block)

}; // VertexShader

}
}