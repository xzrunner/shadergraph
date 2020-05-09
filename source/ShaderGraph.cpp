#include "shadergraph/ShaderGraph.h"
#include "shadergraph/GlobalClock.h"

namespace shadergraph
{

CU_SINGLETON_DEFINITION(ShaderGraph);

extern void regist_rttr();

ShaderGraph::ShaderGraph()
{
	regist_rttr();
}

void ShaderGraph::Update(float dt)
{
    GlobalClock::Instance()->Update(dt);
}

}