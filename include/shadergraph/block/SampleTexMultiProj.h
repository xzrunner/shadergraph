#pragma once

#include "shadergraph/Block.h"

namespace shadergraph
{
namespace block
{

class SampleTexMultiProj : public Block
{
public:
    SampleTexMultiProj()
    {
        SetupPorts({
            { VarType::Sampler2D, "tex" },
            { VarType::Float3,    "pos" },
            { VarType::Float3,    "weights" },
        }, {
            { VarType::Float4, "rgba" },
        });
    }

    virtual std::string GetBody(const Evaluator& eval) const override
    {
        return R"(
vec4 #rgba# = vec4(#weights#.xxx * texture(#tex#, #pos#.yz).rgb +
	               #weights#.yyy * texture(#tex#, #pos#.zx).rgb +
	               #weights#.zzz * texture(#tex#, #pos#.xy).rgb, 1.0);
)";
    }

    RTTR_ENABLE(Block)

}; // SampleTexMultiProj

}
}