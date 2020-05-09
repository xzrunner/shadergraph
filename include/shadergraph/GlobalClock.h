#pragma once

#include <cu/cu_macro.h>

#include <boost/signals2.hpp>

namespace shadergraph
{

class GlobalClock
{
public:
	typedef boost::signals2::signal<void(float t, float dt, float smooth_dt)> OnTick;

public:
	void Update(float dt);

	boost::signals2::connection DoOnTick(const OnTick::slot_type& slot);

private:
	float m_time;
	float m_ticks;

	OnTick m_on_tick;

	CU_SINGLETON_DECLARATION(GlobalClock)

}; // GlobalClock

}