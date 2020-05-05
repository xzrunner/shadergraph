#include "shadergraph/ShaderGraph.h"

namespace shadergraph
{

CU_SINGLETON_DEFINITION(ShaderGraph);

extern void regist_rttr();

ShaderGraph::ShaderGraph()
{
	regist_rttr();
}

}