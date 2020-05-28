#include "shadergraph/ShaderGraph.h"
#include "shadergraph/GlobalClock.h"

#include <cslang/Type.h>
#include <cslang/BuildInFuncs.h>

namespace shadergraph
{

CU_SINGLETON_DEFINITION(ShaderGraph);

extern void regist_rttr();

ShaderGraph::ShaderGraph()
{
	regist_rttr();

    cslang::SetupTypeSystem();
    cslang::SetupBuildInFuncs();
}

void ShaderGraph::Update(float dt)
{
    GlobalClock::Instance()->Update(dt);
}

}