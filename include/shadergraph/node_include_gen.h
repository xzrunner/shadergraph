#define XSTR(s) STR(s)
#define STR(s) #s

#ifndef PARM_NODE_TYPE
#error "You must define PARM_NODE_TYPE macro before include this file"
#endif

#ifndef PARM_FILEPATH_H
#define PARM_FILEPATH_H shadergraph/block/##PARM_NODE_TYPE##.h
#endif

#ifndef NO_FILEPATH_INCLUDE
#include XSTR(PARM_FILEPATH_H)
#endif // NO_FILEPATH_INCLUDE

#undef PARM_NODE_NAME
#undef PARM_NODE_TYPE
#undef PARM_FILEPATH_PARM
