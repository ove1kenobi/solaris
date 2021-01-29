#pragma once
#include <chrono>

class Time
{
	using t_clock = std::chrono::high_resolution_clock;
	using t_dSec = std::chrono::duration<long double, std::ratio<1, 1>>;
	using t_moment = std::chrono::time_point<t_clock, t_dSec>;
private:
	t_moment m_start;
	static t_moment m_loopBegin;
public:
	Time();
	long double SinceStart();
	long double DeltaTime();
	void Update();
};