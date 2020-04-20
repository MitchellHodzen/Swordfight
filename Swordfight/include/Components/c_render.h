#pragma once
#include "rendering/Animation.h"
#include "rendering/Spritesheet.h"

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

	void SetAnimationInstance(std::string animationName, unsigned int startingAnimationFrame, unsigned int framesPerSecond, bool looping)
	{
		AnimationInstance* newAnimationInstance = new AnimationInstance(*spritesheet->GetAnimation(animationName), startingAnimationFrame, framesPerSecond, looping);
		delete animationInstance;
		animationInstance = newAnimationInstance;
	}

	AnimationInstance* GetAnimationInstance()
	{
		return animationInstance;
	}

private:
	AnimationInstance* animationInstance;
};