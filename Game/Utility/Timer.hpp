#ifndef TIMER_HPP_INCLUDED
#define TIMER_HPP_INCLUDED

#include <chrono>

class Timer
{
public:

	void reset() 
	{
		_milleseconds = 0;
	}

	void start() 
	{
		_start = std::chrono::system_clock::now();
	}

	void stop()
	{
		auto end = std::chrono::system_clock::now();

		auto duration = end - _start;

		auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(duration).count();

		_milleseconds += (float)microseconds / 1000.0f;
	}

	float milleseconds() const
	{
		return _milleseconds;
	}

private:
	float _milleseconds = 0;

	std::chrono::system_clock::time_point _start;
};

#endif//TIMER_HPP_INCLUDED
