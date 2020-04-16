#include "Animation.h"
#include <iostream>

Animation::Animation()
{
	unsigned int animationLength = 0;
	unsigned int framesPerSecond = 0;
	bool looping = false;
}

Animation::~Animation()
{
	animationSpriteIndicies.clear();
	animationLength = 0;
	name.clear();
}

bool Animation::GenerateAnimation(const std::string name, const int animationSpriteIndiciesArray[], const unsigned int animationLength, const unsigned int framesPerSecond, const bool looping)
{
	bool success = true;
	if (!name.empty())
	{
		this->name = name;
		this->animationLength = animationLength;
		this->framesPerSecond = framesPerSecond;
		this->looping = looping;
		if (animationLength > 0 && animationSpriteIndiciesArray != nullptr)
		{
			for(int i = 0; i < animationLength; ++i)
			{
				this->animationSpriteIndicies.push_back(animationSpriteIndiciesArray[i]);
			}
		}
		else
		{
			std::cout<<"Empty or null animation sprite index array supplied"<<std::endl;
			success = false;
		}
	}
	else
	{
		std::cout<<"No name supplied to animation"<<std::endl;
		success = false;
	}

	return success;

}

unsigned int Animation::GetAnimationLength()
{
	return this->animationLength;
}

unsigned int Animation::GetFramesPerSecond()
{
	return this->framesPerSecond;
}

bool Animation::IsLooping()
{
	return this->looping;
}

std::string Animation::GetName()
{
	return this->name;
}
