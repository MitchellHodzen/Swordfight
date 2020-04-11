#pragma once
#include <SDL.h>
#include "Time.h"

class Timer
{
public:
	Timer(uint32_t timeLimit)
	{
		this->timeLimit = timeLimit;
		timerStarted = false;
	}

	void Start()
	{
		timerStarted = true;
		timeCounter = 0;
	}

	bool IsOverTime() 
	{
		if (timeCounter >= timeLimit)
		{

		}
	}


private:
	void IncrementTimer()
	{
		if (timerStarted)
		{
			timeCounter += Time::GetDeltaTime();
		}
	}
	bool timerStarted;
	uint32_t timeLimit;
	uint32_t timeCounter;
};