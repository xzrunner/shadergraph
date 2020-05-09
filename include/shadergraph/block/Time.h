#pragma once

#include "shadergraph/Block.h"
#include "shadergraph/BlockHelper.h"
#include "shadergraph/ParserUtility.h"

namespace shadergraph
{
namespace block
{

class Time : public Block
{
public:
    static constexpr char* const TIME_STR       = "u_time";
    static constexpr char* const SIN_TIME_STR   = "u_sin_time";
    static constexpr char* const COS_TIME_STR   = "u_cos_time";
    static constexpr char* const DELTA_TIME_STR = "u_delta_time";

public:
    Time()
    {
        SetupPorts({
        }, {
            { VarType::Float4, TIME_STR },
            { VarType::Float4, SIN_TIME_STR },
            { VarType::Float4, COS_TIME_STR },
            { VarType::Float4, DELTA_TIME_STR }
        });
    }

    virtual std::string GetHeader() const
    {
        return BlockHelper::StringFormat(R"(
uniform vec4 #%s#;
uniform vec4 #%s#;
uniform vec4 #%s#;
uniform vec4 #%s#;
)", TIME_STR, SIN_TIME_STR, COS_TIME_STR, DELTA_TIME_STR);
    }

    RTTR_ENABLE(Block)

}; // Time

}
}