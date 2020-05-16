#pragma once
#include <stack>
#include <vector>
struct UserInput
{
	enum InputType { LEFT, RIGHT, UP, DOWN, SPACE, CLOSE, SHIFT};

	UserInput()
	{
		for (int i = 0; i < inputTypesCount; ++i)
		{
			keyStates[i] = false;
			keyPressed[i] = false;
			keyReleased[i] = false;
		}
	}

	const static int inputTypesCount = 7;
	bool keyStates[inputTypesCount];
	bool keyPressed[inputTypesCount];
	bool keyReleased[inputTypesCount];

};