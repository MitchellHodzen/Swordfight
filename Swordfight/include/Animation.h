#pragma once
#include <string>
#include <vector>


class Animation
{
public:
	Animation();
	~Animation();

	bool GenerateAnimation(const std::string name, const int animationSpriteIndiciesArray[], const unsigned int animationLength, const unsigned int framesPerSecond, const bool looping);

	unsigned int GetAnimationLength();
	unsigned int GetFramesPerSecond();
	bool IsLooping();

	std::string GetName();

private:
	void ClearAnimation();
	
	std::vector<int> animationSpriteIndicies;
	unsigned int animationLength;
	unsigned int framesPerSecond;
	bool looping;
	std::string name;
};

