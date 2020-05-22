#pragma once

#include "shadergraph/Block.h"
#include "shadergraph/ParserUtility.h"
#include "shadergraph/Evaluator.h"

#include <cpputil/StringHelper.h>
#include <SM_Vector.h>

namespace shadergraph
{
namespace block
{

class Constant : public Block
{
public:
    enum class Mode
    {
        PI,
        TAU,
        PHI,
        E,
        SQRT2
    };

public:
    Constant()
    {
        SetupPorts({
        }, {
            { VarType::Float, "_out" },
        });
    }

    virtual std::string GetBody(const Evaluator& eval) const override
    {
        const float values[] = {
            3.1415926f,
            6.28318530f,
            1.618034f,
            2.718282f,
            1.414214f,
        };
        return cpputil::StringHelper::Format("float #_out# = %f;", values[static_cast<int>(m_mode)]);
    }

    RTTR_ENABLE(Block)

#define PARM_FILEPATH "shadergraph/block/Constant.parm.h"
#include <dag/node_parms_gen.h>
#undef PARM_FILEPATH

}; // Constant

}
}