#pragma once
//forward declaration
class Spritesheet;
#include "Animation.h"

struct Render
{
	Render()
	{
		spritesheet = nullptr;
		spriteIndex = 0;
	};
	Spritesheet* spritesheet;
	int spriteIndex;
	AnimationInstance animationIndex;
};