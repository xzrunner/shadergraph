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
	.method("GetImports", &shadergraph::Block::GetImports)
	.method("GetExports", &shadergraph::Block::GetExports)
;

#define EXE_FILEPATH "shadergraph/node_rttr_gen.h"
#include "shadergraph/node_regist_cfg.h"
#undef EXE_FILEPATH

}

namespace shadergraph
{

void regist_rttr()
{
}

}