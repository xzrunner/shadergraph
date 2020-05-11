#pragma once

#include "shadergraph/Block.h"

#include <string>
#include <vector>

namespace shadergraph
{
namespace block
{

class RoundedRectangle : public Block
{
public:
    RoundedRectangle() : Block(CODE) {}

    virtual std::string GetBody() const override {
        return "float #ret# = rounded_rectangle(#uv#, #width#, #height#, #radius#);";
    }

private:
    static constexpr char* const CODE = R"(

// https://docs.unity3d.com/Packages/com.unity.shadergraph@9.0/manual/Rounded-Rectangle-Node.html
float rounded_rectangle(vec2 uv, float width, float height, float radius)
{
    radius = max(min(min(abs(radius * 2), abs(width)), abs(height)), 1e-5);
    uv = abs(uv * 2 - 1) - vec2(width, height) + radius;
    float d = length(max(vec2(0, 0), uv)) / radius;
    return clamp((1 - d) / fwidth(d), 0.0, 1.0);
}

)";

    RTTR_ENABLE(Block)

}; // RoundedRectangle

}
}