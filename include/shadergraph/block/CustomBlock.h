#pragma once

#include "shadergraph/Block.h"

#include <string>

namespace shadergraph
{
namespace block
{

class CustomBlock : public Block
{
public:
    CustomBlock() {}

    void SetCode(const std::string& code) { Parser(code); }
    const std::string& GetCode() const { return m_code; }

    RTTR_ENABLE(Block)

#define NOT_GEN_SETTER_GETTER_FUNC
#define PARM_FILEPATH "shadergraph/block/CustomBlock.parm.h"
#include <dag/node_parms_gen.h>
#undef PARM_FILEPATH
#undef NOT_GEN_SETTER_GETTER_FUNC

}; // CustomBlock

}
}