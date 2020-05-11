#pragma once

#include "shadergraph/Block.h"

#include <string>
#include <vector>

namespace shadergraph
{
namespace block
{

class Ellipse : public Block
{
public:
    Ellipse() : Block(CODE) {}

    virtual std::string GetBody() const override {
        return "float #ret# = ellipse(#uv#, #width#, #height#);";
    }

private:
    static constexpr char* const CODE = R"(

// https://docs.unity3d.com/Packages/com.unity.shadergraph@9.0/manual/Ellipse-Node.html
float ellipse(vec2 uv, float width, float height)
{
    float d = length((uv * 2 - 1) / vec2(width, height));
    return clamp((1 - d) / fwidth(d), 0.0, 1.0);
}

)";

    RTTR_ENABLE(Block)

}; // Ellipse

}
}