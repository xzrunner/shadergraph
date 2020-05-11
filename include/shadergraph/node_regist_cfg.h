#ifndef EXE_FILEPATH
#error "You must define EXE_FILEPATH macro before include this file"
#endif

//////////////////////////////////////////////////////////////////////////
// channel
//////////////////////////////////////////////////////////////////////////

#define NO_PARM_FILEPATH
#define PARM_NODE_TYPE Combine
#define PARM_NODE_NAME combine
#include EXE_FILEPATH

#define NO_PARM_FILEPATH
#define PARM_NODE_TYPE Split
#define PARM_NODE_NAME split
#include EXE_FILEPATH

//////////////////////////////////////////////////////////////////////////
// input
//////////////////////////////////////////////////////////////////////////

// basic

#define PARM_NODE_TYPE Float
#define PARM_NODE_NAME float1
#include EXE_FILEPATH

#define PARM_NODE_TYPE Float2
#define PARM_NODE_NAME float2
#include EXE_FILEPATH

#define PARM_NODE_TYPE Float3
#define PARM_NODE_NAME float3
#include EXE_FILEPATH

#define PARM_NODE_TYPE Float4
#define PARM_NODE_NAME float4
#include EXE_FILEPATH

#define PARM_NODE_TYPE Int
#define PARM_NODE_NAME int1
#include EXE_FILEPATH

#define NO_PARM_FILEPATH
#define PARM_NODE_TYPE Time
#define PARM_NODE_NAME time
#include EXE_FILEPATH

// geometry

#define NO_PARM_FILEPATH
#define PARM_NODE_TYPE TexCoords
#define PARM_NODE_NAME tex_coords
#include EXE_FILEPATH

#define NO_PARM_FILEPATH
#define PARM_NODE_TYPE UV
#define PARM_NODE_NAME uv
#include EXE_FILEPATH

//////////////////////////////////////////////////////////////////////////
// math
//////////////////////////////////////////////////////////////////////////

#define NO_PARM_FILEPATH
#define PARM_NODE_TYPE Add
#define PARM_NODE_NAME add
#include EXE_FILEPATH

#define NO_PARM_FILEPATH
#define PARM_NODE_TYPE Subtract
#define PARM_NODE_NAME sub
#include EXE_FILEPATH

#define NO_PARM_FILEPATH
#define PARM_NODE_TYPE Multiply
#define PARM_NODE_NAME mul
#include EXE_FILEPATH

#define NO_PARM_FILEPATH
#define PARM_NODE_TYPE Divide
#define PARM_NODE_NAME div
#include EXE_FILEPATH

//////////////////////////////////////////////////////////////////////////
// procedural
//////////////////////////////////////////////////////////////////////////

// noise

#define NO_PARM_FILEPATH
#define PARM_NODE_TYPE fBm
#define PARM_NODE_NAME fbm
#include EXE_FILEPATH

#define NO_PARM_FILEPATH
#define PARM_NODE_TYPE GradientNoise
#define PARM_NODE_NAME gradient_noise
#include EXE_FILEPATH

#define NO_PARM_FILEPATH
#define PARM_NODE_TYPE Worley
#define PARM_NODE_NAME worley
#include EXE_FILEPATH

// shapes

#define NO_PARM_FILEPATH
#define PARM_NODE_TYPE Checkerboard
#define PARM_NODE_NAME checkerboard
#include EXE_FILEPATH

//////////////////////////////////////////////////////////////////////////
// utility
//////////////////////////////////////////////////////////////////////////

#define NO_PARM_FILEPATH
#define PARM_NODE_TYPE FragmentShader
#define PARM_NODE_NAME frag_shader
#include EXE_FILEPATH
