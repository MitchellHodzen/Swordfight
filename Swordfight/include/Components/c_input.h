#pragma once
#include <stack>
#include <vector>
struct UserInput
{
	enum InputType { LEFT, RIGHT, UP, DOWN, SPACE, CLOSE};

	UserInput()
	{
		for (int i = 0; i < 5; ++i)
		{
			keyStates[i] = false;
		}
	}

	bool keyStates[5];

};