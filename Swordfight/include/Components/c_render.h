#pragma once
//forward declaration
class Spritesheet;

struct Render
{
	Render()
	{
		spritesheet = nullptr;
		spriteIndex = 0;
	};
	Spritesheet* spritesheet;
	int spriteIndex;
};