#pragma once

#include "shadergraph/Block.h"

namespace shadergraph
{
namespace block
{

class ViewMatrix : public Block
{
public:
    ViewMatrix()
    {
        SetupPorts({
        }, {
            { VarType::Matrix4, "_out" }
        });

        m_default_out_vals[0] = Variant(VarType::String, "", std::make_shared<StringVal>("view"));

        auto unif = std::make_shared<UniformVal>();
        unif->var.type = VarType::Matrix4;
        unif->var.name = "view";

        Variant var;
        var.type = VarType::Uniform;
        var.name = unif->var.name;
        var.val = unif;

        m_global_vars.push_back(var);
    }

    RTTR_ENABLE(Block)

}; // ViewMatrix

}
}