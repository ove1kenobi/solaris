#include "pch.h"
#include "Time.h"

Time::t_moment Time::m_loopBegin = Time::t_clock::now();
Time::t_dSec Time::m_deltaTime = Time::m_loopBegin - Time::m_loopBegin;

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
	return m_deltaTime.count();
}

void Time::Update()
{
	m_deltaTime = t_clock::now() - m_loopBegin;
	m_loopBegin = t_clock::now();
}