#pragma once
#include <string>
#include <vector>
#include <cstdint>

class Animation
{
public:
	~Animation();

	bool GenerateAnimation(const std::string name, const std::vector<int> animationSpriteIndicies);

	unsigned int GetAnimationLength();
	int GetSpriteAtIndex(int spriteIndex);

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
	uint32_t lastFrameTime = 0;
	uint32_t frameTime = 0;
};