#include "shadergraph/GlobalClock.h"

namespace shadergraph
{

CU_SINGLETON_DEFINITION(GlobalClock)

GlobalClock::GlobalClock()
	: m_time(0)
	, m_ticks(0)
{
}

void GlobalClock::Update(float dt)
{
	m_time += dt;
	++m_ticks;

	m_on_tick(m_time, dt, m_time / m_ticks);
}

boost::signals2::connection GlobalClock::DoOnTick(const OnTick::slot_type& slot)
{
	return m_on_tick.connect(slot);
}

}