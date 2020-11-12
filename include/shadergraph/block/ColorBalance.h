// code from PD80_04_Color_Balance.fx

#pragma once

#include "shadergraph/Block.h"

namespace shadergraph
{
namespace block
{

class ColorBalance : public Block
{
public:
	ColorBalance() : Block(R"(

/////////////////////////////////////////////////
/// <color_balance> @export
/////////////////////////////////////////////////

uniform int preserve_luma;
uniform int separation_mode;    // 0 Harsh Separation, 1 Smooth Separation

uniform float s_RedShift;       // Shadows: Cyan <--> Red
uniform float s_GreenShift;     // Shadows: Magenta <--> Green
uniform float s_BlueShift;      // Shadows: Yellow <--> Blue

uniform float m_RedShift;       // Midtones: Cyan <--> Red
uniform float m_GreenShift;     // Midtones: Magenta <--> Green
uniform float m_BlueShift;      // Midtones: Yellow <--> Blue

uniform float h_RedShift;       // Highlights: Cyan <--> Red
uniform float h_GreenShift;     // Highlights: Magenta <--> Green
uniform float h_BlueShift;      // Highlights: Yellow <--> Blue

// todo
//#define ES_RGB vec3(1.0 - vec3(0.299, 0.587, 0.114))
//#define ES_CMY vec3(dot(ES_RGB.yz, 0.5), dot(ES_RGB.xz, 0.5), dot(ES_RGB.xy, 0.5))

vec3 curve(vec3 x)
{
    return x * x * (3.0f - 2.0f * x);
}

vec3 color_balance_impl(vec3 c, vec3 shadows, vec3 midtones, vec3 highlights)
{
    // For highlights
    float luma = dot(c.xyz, vec3(0.333333f, 0.333334f, 0.333333f));
        
    // Determine the distribution curves between shadows, midtones, and highlights
    vec3 dist_s; vec3 dist_h;

    switch (separation_mode)
    {
        case 0:
            dist_s.xyz = curve(max(1.0f - c.xyz * 2.0f, 0.0f));
            dist_h.xyz = curve(max((c.xyz - 0.5f) * 2.0f, 0.0f));
            break;

        case 1:
            dist_s.xyz = pow(vec3(1.0f) - c.xyz, vec3(4.0f));
            dist_h.xyz = pow(c.xyz, vec3(4.0f));
            break;
    }

    // Get luminosity offsets
    // One could omit this whole code part in case no luma should be preserved
    vec3 s_rgb = vec3(1.0f);
    vec3 m_rgb = vec3(1.0f);
    vec3 h_rgb = vec3(1.0f);

    if (preserve_luma > 0)
    {
        s_rgb.r = shadows.r > 0.0f     ? ES_RGB.r * shadows.r    : ES_CMY.r * abs(shadows.r);
        s_rgb.g = shadows.g > 0.0f     ? ES_RGB.g * shadows.g    : ES_CMY.g * abs(shadows.g);
        s_rgb.b = shadows.b > 0.0f     ? ES_RGB.b * shadows.b    : ES_CMY.b * abs(shadows.b);
        m_rgb.r = midtones.r > 0.0f    ? ES_RGB.r * midtones.r   : ES_CMY.r * abs(midtones.r);
        m_rgb.g = midtones.g > 0.0f    ? ES_RGB.g * midtones.g   : ES_CMY.g * abs(midtones.g);
        m_rgb.b = midtones.b > 0.0f    ? ES_RGB.b * midtones.b   : ES_CMY.b * abs(midtones.b);
        h_rgb.r = highlights.r > 0.0f  ? ES_RGB.r * highlights.r : ES_CMY.r * abs(highlights.r);
        h_rgb.g = highlights.g > 0.0f  ? ES_RGB.g * highlights.g : ES_CMY.g * abs(highlights.g);
        h_rgb.b = highlights.b > 0.0f  ? ES_RGB.b * highlights.b : ES_CMY.b * abs(highlights.b);
    }
    vec3 mids = clamp(1.0f - dist_s.xyz - dist_h.xyz, 0.0, 1.0);
    vec3 highs = dist_h.xyz * (highlights.xyz * h_rgb.xyz * (1.0f - luma));
    vec3 newc = c.xyz * (dist_s.xyz * shadows.xyz * s_rgb.xyz + mids.xyz * midtones.xyz * m_rgb.xyz) * (1.0f - c.xyz) + highs.xyz;
    return clamp(c.xyz + newc.xyz, 0.0, 1.0);
}

vec4 color_balance(vec4 color)
{
    color.xyz = clamp(color.xyz, 0.0, 1.0);
    color.xyz = color_balance_impl(color.xyz, vec3(s_RedShift, s_GreenShift, s_BlueShift), 
                                              vec3(m_RedShift, m_GreenShift, m_BlueShift),
                                              vec3(h_RedShift, h_GreenShift, h_BlueShift));
    return color;
}

)") {}

    virtual std::string GetHeader(const Evaluator& eval) const
    {
        return R"(
#define ES_RGB vec3(1.0 - vec3(0.299, 0.587, 0.114))
#define ES_CMY vec3(dot(ES_RGB.yz, vec2(0.5)), dot(ES_RGB.xz, vec2(0.5)), dot(ES_RGB.xy, vec2(0.5)))
)";
    }

    RTTR_ENABLE(Block)

}; // ColorBalance

}
}
