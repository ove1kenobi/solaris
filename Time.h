#pragma once
class Time
{
	using t_clock = std::chrono::high_resolution_clock;
	using t_dSec = std::chrono::duration<long double, std::ratio<1, 1>>;
	using t_dNano = std::chrono::duration<long double, std::ratio<1, 1000000000>>;
	using t_moment = std::chrono::time_point<t_clock, t_dNano>;
private:
	t_moment m_start;
	static t_moment m_loopBegin;
	static t_dSec m_deltaTime;
public:
	Time();
	~Time() = default;
	long double SinceStart();
	long double DeltaTime();
	void Update();
};