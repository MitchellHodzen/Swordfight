#pragma once
#include <string>
#include <vector>


class Animation
{
public:
	~Animation();

	bool GenerateAnimation(const std::string name, const std::vector<int> animationSpriteIndicies);

	unsigned int GetAnimationLength();

	std::string GetName();

private:
	void ClearAnimation();
	
	std::vector<int> animationSpriteIndicies;
	unsigned int animationLength = 0;
	std::string name;
};

struct AnimationInstance{
	std::string animationName;
	unsigned int currentAnimationFrame = 0;
	unsigned int framesPerSecond = 0;
	bool looping = false;
};