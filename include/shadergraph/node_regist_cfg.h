#ifndef EXE_FILEPATH
#error "You must define EXE_FILEPATH macro before include this file"
#endif

// channel

#define PARM_NODE_TYPE Combine
#define PARM_NODE_NAME combine
#include EXE_FILEPATH

// input

#define PARM_NODE_TYPE TexCoords
#define PARM_NODE_NAME tex_coords
#include EXE_FILEPATH

#define PARM_NODE_TYPE UV
#define PARM_NODE_NAME uv
#include EXE_FILEPATH

// noise

#define PARM_NODE_TYPE fBm
#define PARM_NODE_NAME fbm
#include EXE_FILEPATH

#define PARM_NODE_TYPE Worley
#define PARM_NODE_NAME worley
#include EXE_FILEPATH

// utility

#define PARM_NODE_TYPE FragmentShader
#define PARM_NODE_NAME frag_shader
#include EXE_FILEPATH
