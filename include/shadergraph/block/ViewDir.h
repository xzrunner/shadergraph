#pragma once

#include "shadergraph/Block.h"
#include "shadergraph/VarNames.h"

#include <cpputil/StringHelper.h>

namespace shadergraph
{
namespace block
{

class ViewDir : public Block
{
public:
    ViewDir() : Block(cpputil::StringHelper::Format(R"(
in vec4 %s;
uniform vec3 %s;
)", VarNames::FragInputs::frag_pos, VarNames::PositionUniforms::cam_pos))
    {
        SetupPorts({
        }, {
            { VarType::Float3, "view_dir" }
        });
    }

    virtual std::string GetBody(const Evaluator& eval) const override
    {
        return cpputil::StringHelper::Format(
            "vec3 #view_dir# = normalize(%s - %s.xyz);\n", 
            VarNames::PositionUniforms::cam_pos, 
            VarNames::FragInputs::frag_pos
        );
    }

    RTTR_ENABLE(Block)

}; // ViewDir

}
}