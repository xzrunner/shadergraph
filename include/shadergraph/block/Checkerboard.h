#pragma once

#include "shadergraph/Block.h"

#include <string>
#include <vector>

namespace shadergraph
{
namespace block
{

class Checkerboard : public Block
{
public:
    Checkerboard() : Block(CODE) {}

    virtual std::string GetBody() const override {
        return "vec3 #ret# = checkerboard(#st#, #col_a#, #col_b#, #freq#);";
    }

private:
    static constexpr char* const CODE = R"(

// https://docs.unity3d.com/Packages/com.unity.shadergraph@9.0/manual/Checkerboard-Node.html
vec3 checkerboard(vec2 st, vec3 col_a, vec3 col_b, vec2 freq)
{
    st = (st.xy + 0.5) * freq;
    vec4 derivatives = vec4(dFdx(st), dFdy(st));
    vec2 duv_length = sqrt(vec2(dot(derivatives.xz, derivatives.xz), dot(derivatives.yw, derivatives.yw)));
    float width = 1.0;
    vec2 distance3 = 4.0 * abs(fract(st + 0.25) - 0.5) - width;
    vec2 scale = 0.35 / duv_length.xy;
    float freq_limiter = sqrt(clamp(1.1f - max(duv_length.x, duv_length.y), 0.0, 1.0));
    vec2 vector_alpha = clamp(distance3 * scale.xy, -1.0, 1.0);
    float alpha = clamp(0.5f + 0.5f * vector_alpha.x * vector_alpha.y * freq_limiter, 0.0, 1.0);
    return mix(col_a, col_b, alpha);
}

)";

    RTTR_ENABLE(Block)

}; // Checkerboard

}
}