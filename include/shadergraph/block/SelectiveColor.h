// code from PD80_04_Selective_Color.fx

#pragma once

#include "shadergraph/Block.h"

namespace shadergraph
{
namespace block
{

class SelectiveColor : public Block
{
public:
	SelectiveColor() : Block(R"(

/////////////////////////////////////////////////
/// <selective_color> @export
/////////////////////////////////////////////////

uniform int corr_method;    // 0 Absolute, 1 Relative
uniform int corr_method2;   // 0 Absolute, 1 Relative

// Reds
uniform float r_adj_cya;
uniform float r_adj_mag;
uniform float r_adj_yel;
uniform float r_adj_bla;
uniform float r_adj_sat;
uniform float r_adj_vib;

// Yellows
uniform float y_adj_cya;
uniform float y_adj_mag;
uniform float y_adj_yel;
uniform float y_adj_bla;
uniform float y_adj_sat;
uniform float y_adj_vib;

// Greens
uniform float g_adj_cya;
uniform float g_adj_mag;
uniform float g_adj_yel;
uniform float g_adj_bla;
uniform float g_adj_sat;
uniform float g_adj_vib;

// Cyans
uniform float c_adj_cya;
uniform float c_adj_mag;
uniform float c_adj_yel;
uniform float c_adj_bla;
uniform float c_adj_sat;
uniform float c_adj_vib;

// Blues
uniform float b_adj_cya;
uniform float b_adj_mag;
uniform float b_adj_yel;
uniform float b_adj_bla;
uniform float b_adj_sat;
uniform float b_adj_vib;

// Magentas
uniform float m_adj_cya;
uniform float m_adj_mag;
uniform float m_adj_yel;
uniform float m_adj_bla;
uniform float m_adj_sat;
uniform float m_adj_vib;

// Whites
uniform float w_adj_cya;
uniform float w_adj_mag;
uniform float w_adj_yel;
uniform float w_adj_bla;
uniform float w_adj_sat;
uniform float w_adj_vib;

// Neutrals
uniform float n_adj_cya;
uniform float n_adj_mag;
uniform float n_adj_yel;
uniform float n_adj_bla;
uniform float n_adj_sat;
uniform float n_adj_vib;

// Blacks
uniform float bk_adj_cya;
uniform float bk_adj_mag;
uniform float bk_adj_yel;
uniform float bk_adj_bla;
uniform float bk_adj_sat;
uniform float bk_adj_vib;

float getLum(vec3 x)
{
    return dot(x, vec3(0.212656, 0.715158, 0.072186));
}

vec3 sat(vec3 res, float x)
{
    return clamp(mix(vec3(getLum(res.xyz)), res.xyz, x + 1.0f), 0.0, 1.0);
}

vec3 vib(vec3 res, float x)
{
    vec4 sat = vec4(0.0f);
    sat.xy = vec2(min(min(res.x, res.y), res.z), max(max(res.x, res.y), res.z));
    sat.z = sat.y - sat.x;
    sat.w = getLum(res.xyz);
    return clamp(mix(vec3(sat.w), res.xyz, 1.0f + (x * (1.0f - sat.z))), 0.0, 1.0);
}

float mid(vec3 c)
{
    float sum = c.x + c.y + c.z;
    float mn = min(min(c.x, c.y), c.z);
    float mx = max(max(c.x, c.y), c.z);
    return sum - mn - mx;
}

float adjust_color(float scale, float colorvalue, float adjust, float bk, int method)
{
    /* 
    y(value, adjustment) = clamp(((-1 - adjustment) * bk - adjustment) * method, -value, 1 - value) * scale
    absolute: method = 1.0f - colorvalue * 0
    relative: method = 1.0f - colorvalue * 1
    */
    return clamp(((-1.0f - adjust) * bk - adjust) * (1.0f - colorvalue * method), -colorvalue, 1.0f - colorvalue) * scale;
}

vec4 selective_color(vec4 color)
{
//    vec4 color      = tex2D(ReShade::BackBuffer, texcoord);

    // Clamp 0..1
    color.xyz         = clamp(color.xyz, 0.0, 1.0);

    // Need these a lot
    float min_value   = min(min(color.x, color.y), color.z);
    float max_value   = max(max(color.x, color.y), color.z);
    float mid_value   = mid(color.xyz);
        
    // Used for determining which pixels to adjust regardless of prior changes to color
    vec3 orig         = color.xyz;

    // Scales
    float sRGB        = max_value - mid_value;
    float sCMY        = mid_value - min_value;
    float sNeutrals   = 1.0f - (abs(max_value - 0.5f) + abs(min_value - 0.5f));
    float sWhites     = (min_value - 0.5f) * 2.0f;
    float sBlacks     = (0.5f - max_value) * 2.0f;

    /*
    Create relative saturation levels.
    For example when saturating red channel you will manipulate yellow and magenta channels.
    So, to ensure there are no bugs and transitions are smooth, need to scale saturation with
    relative saturation of nearest colors. If difference between red and green is low (color nearly yellow)
    you use this info to scale back red saturation on those pixels.
    This solution is not fool proof, but gives acceptable results almost always.
    */
        
    // Red is when maxvalue = x
    float r_d_m       = orig.x - orig.z;
    float r_d_y       = orig.x - orig.y;
    // Yellow is when minvalue = z
    float y_d         = mid_value - orig.z;
    // Green is when maxvalue = y
    float g_d_y       = orig.y - orig.x;
    float g_d_c       = orig.y - orig.z;
    // Cyan is when minvalue = x
    float c_d         = mid_value - orig.x;
    // Blue is when maxvalue = z
    float b_d_c       = orig.z - orig.y;
    float b_d_m       = orig.z - orig.x;
    // Magenta is when minvalue = y
    float m_d         = mid_value - orig.y;
        
    float r_delta     = 1.0f;
    float y_delta     = 1.0f;
    float g_delta     = 1.0f;
    float c_delta     = 1.0f;
    float b_delta     = 1.0f;
    float m_delta     = 1.0f;

    if (corr_method2 != 0) // Relative saturation
    {
        r_delta       = min(r_d_m, r_d_y);
        y_delta       = y_d;
        g_delta       = min(g_d_y, g_d_c);
        c_delta       = c_d;
        b_delta       = min(b_d_c, b_d_m);
        m_delta       = m_d;
    } 

    // Selective Color
    if (max_value == orig.x)
    {
        color.x       = color.x + adjust_color(sRGB, color.x, r_adj_cya, r_adj_bla, corr_method);
        color.y       = color.y + adjust_color(sRGB, color.y, r_adj_mag, r_adj_bla, corr_method);
        color.z       = color.z + adjust_color(sRGB, color.z, r_adj_yel, r_adj_bla, corr_method);
        color.xyz     = sat(color.xyz, r_adj_sat * r_delta);
        color.xyz     = vib(color.xyz, r_adj_vib * r_delta);
    }

    if (min_value == orig.z)
    {
        color.x       = color.x + adjust_color(sCMY, color.x, y_adj_cya, y_adj_bla, corr_method);
        color.y       = color.y + adjust_color(sCMY, color.y, y_adj_mag, y_adj_bla, corr_method);
        color.z       = color.z + adjust_color(sCMY, color.z, y_adj_yel, y_adj_bla, corr_method);
        color.xyz     = sat(color.xyz, y_adj_sat * y_delta);
        color.xyz     = vib(color.xyz, y_adj_vib * y_delta);
    }

    if (max_value == orig.y)
    {
        color.x       = color.x + adjust_color(sRGB, color.x, g_adj_cya, g_adj_bla, corr_method);
        color.y       = color.y + adjust_color(sRGB, color.y, g_adj_mag, g_adj_bla, corr_method);
        color.z       = color.z + adjust_color(sRGB, color.z, g_adj_yel, g_adj_bla, corr_method);
        color.xyz     = sat(color.xyz, g_adj_sat * g_delta);
        color.xyz     = vib(color.xyz, g_adj_vib * g_delta);
    }

    if (min_value == orig.x)
    {
        color.x       = color.x + adjust_color(sCMY, color.x, c_adj_cya, c_adj_bla, corr_method);
        color.y       = color.y + adjust_color(sCMY, color.y, c_adj_mag, c_adj_bla, corr_method);
        color.z       = color.z + adjust_color(sCMY, color.z, c_adj_yel, c_adj_bla, corr_method);
        color.xyz     = sat(color.xyz, c_adj_sat * c_delta);
        color.xyz     = vib(color.xyz, c_adj_vib * c_delta);
    }

    if (max_value == orig.z)
    {
        color.x       = color.x + adjust_color(sRGB, color.x, b_adj_cya, b_adj_bla, corr_method);
        color.y       = color.y + adjust_color(sRGB, color.y, b_adj_mag, b_adj_bla, corr_method);
        color.z       = color.z + adjust_color(sRGB, color.z, b_adj_yel, b_adj_bla, corr_method);
        color.xyz     = sat(color.xyz, b_adj_sat * b_delta);
        color.xyz     = vib(color.xyz, b_adj_vib * b_delta);
    }

    if (min_value == orig.y)
    {
        color.x       = color.x + adjust_color(sCMY, color.x, m_adj_cya, m_adj_bla, corr_method);
        color.y       = color.y + adjust_color(sCMY, color.y, m_adj_mag, m_adj_bla, corr_method);
        color.z       = color.z + adjust_color(sCMY, color.z, m_adj_yel, m_adj_bla, corr_method);
        color.xyz     = sat(color.xyz, m_adj_sat * m_delta);
        color.xyz     = vib(color.xyz, m_adj_vib * m_delta);
    }

    if (min_value >= 0.5f)
    {
        color.x       = color.x + adjust_color(sWhites, color.x, w_adj_cya, w_adj_bla, corr_method);
        color.y       = color.y + adjust_color(sWhites, color.y, w_adj_mag, w_adj_bla, corr_method);
        color.z       = color.z + adjust_color(sWhites, color.z, w_adj_yel, w_adj_bla, corr_method);
        color.xyz     = sat(color.xyz, w_adj_sat * smoothstep(0.5f, 1.0f, min_value));
        color.xyz     = vib(color.xyz, w_adj_vib * smoothstep(0.5f, 1.0f, min_value));
    }

    if (max_value > 0.0f && min_value < 1.0f)
    {
        color.x       = color.x + adjust_color(sNeutrals, color.x, n_adj_cya, n_adj_bla, corr_method);
        color.y       = color.y + adjust_color(sNeutrals, color.y, n_adj_mag, n_adj_bla, corr_method);
        color.z       = color.z + adjust_color(sNeutrals, color.z, n_adj_yel, n_adj_bla, corr_method);
        color.xyz     = sat(color.xyz, n_adj_sat);
        color.xyz     = vib(color.xyz, n_adj_vib);
    }

    if (max_value < 0.5f)
    {
        color.x       = color.x + adjust_color(sBlacks, color.x, bk_adj_cya, bk_adj_bla, corr_method);
        color.y       = color.y + adjust_color(sBlacks, color.y, bk_adj_mag, bk_adj_bla, corr_method);
        color.z       = color.z + adjust_color(sBlacks, color.z, bk_adj_yel, bk_adj_bla, corr_method);
        color.xyz     = sat(color.xyz, bk_adj_sat * smoothstep(0.5f, 0.0f, max_value));
        color.xyz     = vib(color.xyz, bk_adj_vib * smoothstep(0.5f, 0.0f, max_value));
    }

    return color;
}

)") {}

    RTTR_ENABLE(Block)

}; // SelectiveColor

}
}
