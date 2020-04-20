#pragma once
//forward declaration
class Spritesheet;
struct AnimationInstance;

struct Render
{
	Render()
	{
		spritesheet = nullptr;
		animationInstance = nullptr;
	};
	Spritesheet* spritesheet;
	AnimationInstance* animationInstance;
};