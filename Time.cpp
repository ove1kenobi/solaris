#include "Time.h"

Time::t_moment Time::m_loopBegin = Time::t_clock::now();

Time::Time() : m_start{ t_clock::now() }
{

}

long double Time::SinceStart()
{
	t_dSec elapsed = t_clock::now() - m_start;
	return elapsed.count();
}

long double Time::DeltaTime()
{
	t_dSec elapsed = t_clock::now() - m_loopBegin;
	return elapsed.count();
}

void Time::Update()
{
	m_loopBegin = t_clock::now();
}