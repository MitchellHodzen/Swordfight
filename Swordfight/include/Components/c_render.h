#pragma once
//forward declaration
class Spritesheet;
#include "Animation.h"

struct Render
{
	Render()
	{
		spritesheet = nullptr;
		animationInstance = nullptr;
		spriteIndex = 0;
	};
	Spritesheet* spritesheet;
	int spriteIndex;
	AnimationInstance* animationInstance;
};