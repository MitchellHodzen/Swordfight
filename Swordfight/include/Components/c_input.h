#pragma once
#include <stack>
#include <vector>
#include "InputCommandMapper.h"
struct UserInput
{

	UserInput()
	{
		for (int i = 0; i < InputCommandMapper::commandSize; ++i)
		{
			keyStates[i] = false;
			keyPressed[i] = false;
			keyReleased[i] = false;
		}
	}

	bool keyStates[InputCommandMapper::commandSize];
	bool keyPressed[InputCommandMapper::commandSize];
	bool keyReleased[InputCommandMapper::commandSize];

};