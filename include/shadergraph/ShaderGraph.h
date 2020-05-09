#pragma once

#include <cu/cu_macro.h>

namespace shadergraph
{

class ShaderGraph
{
public:
    void Update(float dt);

    CU_SINGLETON_DECLARATION(ShaderGraph)

}; // ShaderGraph

}