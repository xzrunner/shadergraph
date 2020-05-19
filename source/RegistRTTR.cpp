#include "shadergraph/Block.h"

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
    .method("GetUniforms",    &shadergraph::Block::GetUniforms)
    .method("GetFunctions",   &shadergraph::Block::GetFunctions)
    .method("GetCurrFuncIdx", &shadergraph::Block::GetCurrFuncIdx)
;

#define EXE_FILEPATH "shadergraph/node_rttr_gen.h"
#include "shadergraph/node_regist_cfg.h"
#undef EXE_FILEPATH

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

}

namespace shadergraph
{

void regist_rttr()
{
}

}