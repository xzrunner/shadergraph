#pragma once

#include "shadergraph/Block.h"

namespace shadergraph
{
namespace block
{

class TexCoords : public Block
{
public:
    TexCoords()
    {
        SetupPorts({
            { VarType::Sampler2D, "tex" }
        }, {
            { VarType::Float2, "uv" }
        });
    }

    RTTR_ENABLE(Block)

}; // TexCoords

}
}