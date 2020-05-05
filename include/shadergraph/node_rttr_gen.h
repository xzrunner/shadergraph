#define XSTR(s) STR(s)
#define STR(s) #s

#ifndef PARM_NODE_TYPE
#error "You must define PARM_NODE_TYPE macro before include this file"
#endif

#ifndef PARM_NODE_NAME
#error "You must define PARM_NODE_NAME macro before include this file"
#endif

//#ifndef PARM_FILEPATH_PARM
//#define PARM_FILEPATH_PARM shadergraph/block/##PARM_NODE_TYPE##.parm.h
//#endif

#define RTTR_NAME shadergraph::##PARM_NODE_NAME

rttr::registration::class_<shadergraph::block::PARM_NODE_TYPE>(XSTR(RTTR_NAME))
.constructor<>()
//#define PARM_FILEPATH XSTR(PARM_FILEPATH_PARM)
#define PARM_NODE_CLASS shadergraph::node::##PARM_NODE_TYPE
#include <dag/rttr_prop_gen.h>
#undef PARM_FILEPATH
#undef PARM_NODE_CLASS
;

#undef PARM_NODE_NAME
#undef PARM_NODE_TYPE
//#undef PARM_FILEPATH_PARM
