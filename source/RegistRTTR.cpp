#include "shadergraph/Block.h"
#include "shadergraph/Variant.h"

#define EXE_FILEPATH "shadergraph/node_include_gen.h"
#include "shadergraph/node_regist_cfg.h"
#undef EXE_FILEPATH

#include <rttr/registration>

#define REGIST_NODE_TYPE(type, name)                                            \
	rttr::registration::class_<shadergraph::device::type>("shadergraph::"#name) \
		.constructor<>()                                                        \
	;

#define REGIST_ENUM_ITEM(type, name, label) \
    rttr::value(name, type),                \
    rttr::metadata(type, label)             \

RTTR_REGISTRATION
{

rttr::registration::class_<dag::Node<shadergraph::Variant>::Port>("shadergraph::Node::Port")
	.property("var", &dag::Node<shadergraph::Variant>::Port::var)
;

rttr::registration::class_<shadergraph::Block>("shadergraph::Block")
	.method("GetImports",     &shadergraph::Block::GetImports)
	.method("GetExports",     &shadergraph::Block::GetExports)
    .method("GetVariants",    &shadergraph::Block::GetVariants)
    .method("GetFunctions",   &shadergraph::Block::GetFunctions)
    .method("GetCurrFuncIdx", &shadergraph::Block::GetCurrFuncIdx)
;

#define EXE_FILEPATH "shadergraph/node_rttr_gen.h"
#include "shadergraph/node_regist_cfg.h"
#undef EXE_FILEPATH

rttr::registration::enumeration<shadergraph::VarType>("sg_var_type")
(
    REGIST_ENUM_ITEM(shadergraph::VarType::Invalid,     "invalid",     "Invalid"),
    REGIST_ENUM_ITEM(shadergraph::VarType::Dynamic,     "dynamic",     "Dynamic"),
    REGIST_ENUM_ITEM(shadergraph::VarType::Void,        "void",        "Void"),
    REGIST_ENUM_ITEM(shadergraph::VarType::Bool,        "bool",        "Bool"),
    REGIST_ENUM_ITEM(shadergraph::VarType::Bool2,       "bvec2",       "Bool2"),
    REGIST_ENUM_ITEM(shadergraph::VarType::Bool3,       "bvec3",       "Bool3"),
    REGIST_ENUM_ITEM(shadergraph::VarType::Bool4,       "bvec4",       "Bool4"),
    REGIST_ENUM_ITEM(shadergraph::VarType::UInt,        "uint",        "UInt"),
    REGIST_ENUM_ITEM(shadergraph::VarType::Int,         "int",         "Int"),
    REGIST_ENUM_ITEM(shadergraph::VarType::Int2,        "ivec2",       "Int2"),
    REGIST_ENUM_ITEM(shadergraph::VarType::Int3,        "ivec3",       "Int3"),
    REGIST_ENUM_ITEM(shadergraph::VarType::Int4,        "ivec4",       "Int4"),
    REGIST_ENUM_ITEM(shadergraph::VarType::Float,       "float",       "Float"),
    REGIST_ENUM_ITEM(shadergraph::VarType::Float2,      "vec2",        "Float2"),
    REGIST_ENUM_ITEM(shadergraph::VarType::Float3,      "vec3",        "Float3"),
    REGIST_ENUM_ITEM(shadergraph::VarType::Float4,      "vec4",        "Float4"),
    REGIST_ENUM_ITEM(shadergraph::VarType::Matrix2,     "mat2",        "Matrix2"),
    REGIST_ENUM_ITEM(shadergraph::VarType::Matrix3,     "mat3",        "Matrix3"),
    REGIST_ENUM_ITEM(shadergraph::VarType::Matrix4,     "mat4",        "Matrix4"),
    REGIST_ENUM_ITEM(shadergraph::VarType::Sampler2D,   "sampler2D",   "Sampler2D"),
    REGIST_ENUM_ITEM(shadergraph::VarType::SamplerCube, "samplerCube", "SamplerCube"),
    REGIST_ENUM_ITEM(shadergraph::VarType::Array,       "array",       "Array"),
    REGIST_ENUM_ITEM(shadergraph::VarType::Struct,      "struct",      "Struct"),
    REGIST_ENUM_ITEM(shadergraph::VarType::Uniform,     "uniform",     "Uniform"),
    REGIST_ENUM_ITEM(shadergraph::VarType::Function,    "function",    "Function")
);

rttr::registration::class_<shadergraph::Variant>("sg_variant")
	.property("Type", &shadergraph::Variant::type)
	.property("Name", &shadergraph::Variant::name)
;

rttr::registration::enumeration<shadergraph::block::Blend::Mode>("sg_blend_mode")
(
    REGIST_ENUM_ITEM(shadergraph::block::Blend::Mode::Burn,              "burn",                 "Burn"),
    REGIST_ENUM_ITEM(shadergraph::block::Blend::Mode::Darken,            "darken",               "Darken"),
    REGIST_ENUM_ITEM(shadergraph::block::Blend::Mode::Difference,        "difference",           "Difference"),
    REGIST_ENUM_ITEM(shadergraph::block::Blend::Mode::Dodge,             "dodge",                "Dodge"),
    REGIST_ENUM_ITEM(shadergraph::block::Blend::Mode::Divide,            "divide",               "Divide"),
    REGIST_ENUM_ITEM(shadergraph::block::Blend::Mode::Exclusion,         "exclusion",            "Exclusion"),
    REGIST_ENUM_ITEM(shadergraph::block::Blend::Mode::HardLight,         "hard_light",           "HardLight"),
    REGIST_ENUM_ITEM(shadergraph::block::Blend::Mode::HardMix,           "hard_mix",             "HardMix"),
    REGIST_ENUM_ITEM(shadergraph::block::Blend::Mode::Lighten,           "lighten",              "Lighten"),
    REGIST_ENUM_ITEM(shadergraph::block::Blend::Mode::LinearBurn,        "linear_burn",          "LinearBurn"),
    REGIST_ENUM_ITEM(shadergraph::block::Blend::Mode::LinearDodge,       "linear_dodge",         "LinearDodge"),
    REGIST_ENUM_ITEM(shadergraph::block::Blend::Mode::LinearLight,       "linear_light",         "LinearLight"),
    REGIST_ENUM_ITEM(shadergraph::block::Blend::Mode::LinearLightAddSub, "linear_light_add_sub", "LinearLightAddSub"),
    REGIST_ENUM_ITEM(shadergraph::block::Blend::Mode::Multiply,          "multiply",             "Multiply"),
    REGIST_ENUM_ITEM(shadergraph::block::Blend::Mode::Negation,          "negation",             "Negation"),
    REGIST_ENUM_ITEM(shadergraph::block::Blend::Mode::Overlay,           "overlay",              "Overlay"),
    REGIST_ENUM_ITEM(shadergraph::block::Blend::Mode::PinLight,          "pin_light",            "PinLight"),
    REGIST_ENUM_ITEM(shadergraph::block::Blend::Mode::Screen,            "screen",               "Screen"),
    REGIST_ENUM_ITEM(shadergraph::block::Blend::Mode::SoftLight,         "soft_light",           "SoftLight"),
    REGIST_ENUM_ITEM(shadergraph::block::Blend::Mode::Subtract,          "subtract",             "Subtract"),
    REGIST_ENUM_ITEM(shadergraph::block::Blend::Mode::VividLight,        "vivid_light",          "VividLight"),
    REGIST_ENUM_ITEM(shadergraph::block::Blend::Mode::Overwrite,         "overwrite",            "Overwrite")
);

rttr::registration::enumeration<shadergraph::block::NormalBlend::Mode>("sg_normal_blend_mode")
(
    REGIST_ENUM_ITEM(shadergraph::block::NormalBlend::Mode::Default,    "default",    "Default"),
    REGIST_ENUM_ITEM(shadergraph::block::NormalBlend::Mode::Reoriented, "reoriented", "Reoriented")
);

rttr::registration::enumeration<shadergraph::block::ColorspaceConversion::ColorType>("sg_normal_colorspace_conversion_color_type")
(
    REGIST_ENUM_ITEM(shadergraph::block::ColorspaceConversion::ColorType::RGB,    "rgb",    "RGB"),
    REGIST_ENUM_ITEM(shadergraph::block::ColorspaceConversion::ColorType::Linear, "linear", "Linear"),
    REGIST_ENUM_ITEM(shadergraph::block::ColorspaceConversion::ColorType::HSV,    "hsv",    "HSV")
);

rttr::registration::enumeration<shadergraph::block::Swizzle::Channel>("sg_swizzle_channel")
(
    REGIST_ENUM_ITEM(shadergraph::block::Swizzle::Channel::Red,   "r", "R"),
    REGIST_ENUM_ITEM(shadergraph::block::Swizzle::Channel::Green, "g", "G"),
    REGIST_ENUM_ITEM(shadergraph::block::Swizzle::Channel::Blue,  "b", "B"),
    REGIST_ENUM_ITEM(shadergraph::block::Swizzle::Channel::Alpha, "a", "A")
);

rttr::registration::enumeration<shadergraph::block::Constant::Mode>("sg_normal_constant_mode")
(
    REGIST_ENUM_ITEM(shadergraph::block::Constant::Mode::PI,    "pi",    "PI"),
    REGIST_ENUM_ITEM(shadergraph::block::Constant::Mode::TAU,   "tau",   "TAU"),
    REGIST_ENUM_ITEM(shadergraph::block::Constant::Mode::PHI,   "phi",   "PHI"),
    REGIST_ENUM_ITEM(shadergraph::block::Constant::Mode::E,     "e",     "E"),
    REGIST_ENUM_ITEM(shadergraph::block::Constant::Mode::SQRT2, "sqrt2", "SQRT2")
);

}

namespace shadergraph
{

void regist_rttr()
{
}

}