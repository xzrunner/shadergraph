#pragma once

#include "shadergraph/Block.h"

namespace shadergraph
{
namespace block
{

class ColorspaceConversion : public Block
{
public:
    enum class ColorType
    {
        RGB,
        Linear,
        HSV
    };

public:
    ColorspaceConversion()
    {
        SetupPorts({
            { VarType::Float3, "_in" },
        }, {
            { VarType::Float3, "_out" },
        });
    }

    virtual std::string GetHeader(const Evaluator& eval) const
    {
        return R"(

// https://docs.unity3d.com/Packages/com.unity.shadergraph@9.0/manual/Colorspace-Conversion-Node.html

vec3 colorspace_conversion_rgb2rgb(vec3 _in)
{
    return _in;
}

vec3 colorspace_conversion_rgb2linear(vec3 _in)
{
    vec3 linearRGBLo = _in / 12.92;;
    vec3 linearRGBHi = pow(max(abs((_in + 0.055) / 1.055), 1.192092896e-07), vec3(2.4, 2.4, 2.4));
    //return vec3(_in <= 0.04045) ? linearRGBLo : linearRGBHi;
    vec3 ret;
    ret.x = _in.x <= 0.04045 ? linearRGBLo.x : linearRGBHi.x;
    ret.y = _in.y <= 0.04045 ? linearRGBLo.y : linearRGBHi.y;
    ret.z = _in.z <= 0.04045 ? linearRGBLo.z : linearRGBHi.z;
    return ret;
}

vec3 colorspace_conversion_rgb2hsv(vec3 _in)
{
    vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
    vec4 P = mix(vec4(_in.bg, K.wz), vec4(_in.gb, K.xy), step(_in.b, _in.g));
    vec4 Q = mix(vec4(P.xyw, _in.r), vec4(_in.r, P.yzx), step(P.x, _in.r));
    float D = Q.x - min(Q.w, Q.y);
    float  E = 1e-10;
    return vec3(abs(Q.z + (Q.w - Q.y)/(6.0 * D + E)), D / (Q.x + E), Q.x);
}

vec3 colorspace_conversion_linear2rgb(vec3 _in)
{
    vec3 sRGBLo = _in * 12.92;
    vec3 sRGBHi = (pow(max(abs(_in), 1.192092896e-07), vec3(1.0 / 2.4, 1.0 / 2.4, 1.0 / 2.4)) * 1.055) - 0.055;
    //return vec3(_in <= 0.0031308) ? sRGBLo : sRGBHi;
    vec3 ret;
    ret.x = _in.x <= 0.0031308 ? sRGBLo.x : sRGBHi.x;
    ret.y = _in.y <= 0.0031308 ? sRGBLo.y : sRGBHi.y;
    ret.z = _in.z <= 0.0031308 ? sRGBLo.z : sRGBHi.z;
    return ret;
}

vec3 colorspace_conversion_linear2linear(vec3 _in)
{
    return _in;
}

vec3 colorspace_conversion_linear2hsv(vec3 _in)
{
    vec3 sRGBLo = _in * 12.92;
    vec3 sRGBHi = (pow(max(abs(_in), 1.192092896e-07), vec3(1.0 / 2.4, 1.0 / 2.4, 1.0 / 2.4)) * 1.055) - 0.055;
    //vec3 Linear = vec3(_in <= 0.0031308) ? sRGBLo : sRGBHi;
    vec3 Linear;
    Linear.x = _in.x <= 0.0031308 ? sRGBLo.x : sRGBHi.x;
    Linear.y = _in.y <= 0.0031308 ? sRGBLo.y : sRGBHi.y;
    Linear.z = _in.z <= 0.0031308 ? sRGBLo.z : sRGBHi.z;
    vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
    vec4 P = mix(vec4(Linear.bg, K.wz), vec4(Linear.gb, K.xy), step(Linear.b, Linear.g));
    vec4 Q = mix(vec4(P.xyw, Linear.r), vec4(Linear.r, P.yzx), step(P.x, Linear.r));
    float D = Q.x - min(Q.w, Q.y);
    float  E = 1e-10;
    return vec3(abs(Q.z + (Q.w - Q.y)/(6.0 * D + E)), D / (Q.x + E), Q.x);
}

vec3 colorspace_conversion_hsv2rgb(vec3 _in)
{
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 P = abs(fract(_in.xxx + K.xyz) * 6.0 - K.www);
    return _in.z * mix(K.xxx, clamp(P - K.xxx, 0.0, 1.0), _in.y);
}

vec3 colorspace_conversion_hsv2linear(vec3 _in)
{
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 P = abs(fract(_in.xxx + K.xyz) * 6.0 - K.www);
    vec3 RGB = _in.z * mix(K.xxx, clamp(P - K.xxx, 0.0, 1.0), _in.y);
    vec3 linearRGBLo = RGB / 12.92;
    vec3 linearRGBHi = pow(max(abs((RGB + 0.055) / 1.055), 1.192092896e-07), vec3(2.4, 2.4, 2.4));
    //return vec3(RGB <= 0.04045) ? linearRGBLo : linearRGBHi;
    vec3 ret;
    ret.x = RGB.x <= 0.04045 ? linearRGBLo.x : linearRGBHi.x;
    ret.y = RGB.y <= 0.04045 ? linearRGBLo.y : linearRGBHi.y;
    ret.z = RGB.z <= 0.04045 ? linearRGBLo.z : linearRGBHi.z;
    return ret;
}

vec3 colorspace_conversion_hsv2hsv(vec3 _in)
{
    return _in;
}

)";
    }

    virtual std::string GetBody(const Evaluator& eval) const override
    {
        auto get_type_str = [](ColorType type) -> std::string {
            switch (type)
            {
            case ColorType::RGB:
                return "rgb";
            case ColorType::Linear:
                return "linear";
            case ColorType::HSV:
                return "hsv";
            default:
                assert(0);
                return "";
            }
        };
        auto f_str = get_type_str(m_from);
        auto t_str = get_type_str(m_to);
        return "vec3 #_out# = colorspace_conversion_" + f_str + "2" + t_str + "(#_in#);";
    }

    RTTR_ENABLE(Block)

#define PARM_FILEPATH "shadergraph/block/ColorspaceConversion.parm.h"
#include <dag/node_parms_gen.h>
#undef PARM_FILEPATH

}; // ColorspaceConversion

}
}