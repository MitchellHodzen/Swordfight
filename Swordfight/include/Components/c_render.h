#pragma once
#include "rendering/Animation.h"
//forward declaration
class Spritesheet;

struct Render
{
	Render()
	{
		spritesheet = nullptr;
		animationInstance = nullptr;
	};

	~Render()
	{
		spritesheet = nullptr;
		delete animationInstance;
		animationInstance = nullptr;
	};
	Spritesheet* spritesheet;
	AnimationInstance* animationInstance;
};