#pragma once
#include <iostream>
#include "Timer.h"
#include <cstdint>

struct Lifetime
{
	Timer lifetimeTimer;
	uint32_t timeToLiveMs = 0;
};