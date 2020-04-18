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

	AnimationInstance(Animation& animation, unsigned int currentAnimationFrame, unsigned int framesPerSecond, bool looping)
	{
		this->animation = &animation;
		this->animationName = animation.GetName();
		this->currentAnimationFrame = currentAnimationFrame;
		this->framesPerSecond = framesPerSecond;
		this->looping = looping;
	}

	~AnimationInstance()
	{
		animationName.clear();
		animation = nullptr;
		currentAnimationFrame = 0;
		framesPerSecond = 0;
		looping = false;
		lastFrameTime = 0;
		frameTime = 0;
	}

	int GetCurrentAnimSprite()
	{
		return animation->GetSpriteAtIndex(currentAnimationFrame);
	}

	std::string animationName;
	Animation* animation;
	unsigned int currentAnimationFrame = 0;
	unsigned int framesPerSecond = 0;
	bool looping = false;
	uint32_t lastFrameTime = 0;
	uint32_t frameTime = 0;
};