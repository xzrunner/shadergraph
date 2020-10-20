#pragma once

#include "shadergraph/Block.h"
#include "shadergraph/BlockHelper.h"
#include "shadergraph/ParserUtility.h"

#include <boost/signals2.hpp>

namespace shadergraph
{
namespace block
{

class Time : public Block
{
public:
    static constexpr char* const TIME_STR       = "time";
    static constexpr char* const SIN_TIME_STR   = "sin_time";
    static constexpr char* const COS_TIME_STR   = "cos_time";
    static constexpr char* const DELTA_TIME_STR = "delta_time";

public:
    Time() : Block(BlockHelper::StringFormat(R"(
uniform vec4 %s;
uniform vec4 %s;
uniform vec4 %s;
uniform vec4 %s;
)", TIME_STR, SIN_TIME_STR, COS_TIME_STR, DELTA_TIME_STR)
)
    {
        SetupPorts({
        }, {
            { VarType::Float4, TIME_STR },
            { VarType::Float4, SIN_TIME_STR },
            { VarType::Float4, COS_TIME_STR },
            { VarType::Float4, DELTA_TIME_STR }
        });

        Init();
    }
    virtual ~Time();

private:
    void Init();

    void UpdateTime(float t, float dt, float smooth_dt);

private:
    boost::signals2::connection m_conn_tick;

    float m_t         = 0;
    float m_dt        = 0;
    float m_smooth_dt = 0;

    RTTR_ENABLE(Block)

}; // Time

}
}