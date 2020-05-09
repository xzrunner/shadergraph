#include "shadergraph/block/Time.h"
#include "shadergraph/ValueImpl.h"
#include "shadergraph/GlobalClock.h"

namespace shadergraph
{
namespace block
{

void Time::Init()
{
    m_conn_tick = GlobalClock::Instance()->DoOnTick(
        boost::bind(&Time::UpdateTime, this, _1, _2, _3)
    );
}

Time::~Time()
{
    m_conn_tick.disconnect();
}

void Time::UpdateTime(float t, float dt, float smooth_dt)
{
    if (t != m_t)
    {
        m_exports[0].var.type.type = VarType::Float4;
        auto val = std::make_shared<Float4Val>();
        val->xyzw[0] = t / 20;
        val->xyzw[1] = t;
        val->xyzw[2] = t * 2;
        val->xyzw[3] = t * 3;
        m_exports[0].var.type.val = val;

        m_exports[1].var.type.type = VarType::Float4;
        val = std::make_shared<Float4Val>();
        val->xyzw[0] = sin(t / 8);
        val->xyzw[1] = sin(t / 4);
        val->xyzw[2] = sin(t / 2);
        val->xyzw[3] = sin(t);
        m_exports[1].var.type.val = val;

        m_exports[2].var.type.type = VarType::Float4;
        val = std::make_shared<Float4Val>();
        val->xyzw[0] = cos(t / 8);
        val->xyzw[1] = cos(t / 4);
        val->xyzw[2] = cos(t / 2);
        val->xyzw[3] = cos(t);
        m_exports[2].var.type.val = val;

        m_t = t;
    }

    if (dt != m_dt || smooth_dt != m_smooth_dt)
    {
        m_exports[3].var.type.type = VarType::Float4;
        auto val = std::make_shared<Float4Val>();
        val->xyzw[0] = dt;
        val->xyzw[1] = 1 / dt;
        val->xyzw[2] = smooth_dt;
        val->xyzw[3] = 1 / smooth_dt;
        m_exports[3].var.type.val = val;
    }
}

}
}