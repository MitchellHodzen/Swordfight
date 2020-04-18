#include "AnimationSystem.h"
#include "kecs/KECS.h"
#include "Components/c_render.h"
#include "Animation.h"
#include "Spritesheet.h"
#include "Time.h"

void AnimationSystem::AdvanceAnimations()
{
	std::vector<Entity> entities = EntityManager::GetEntitiesWithComponent<Render>();
	for (Entity entity : entities)
	{
		Render* render = EntityManager::GetComponent<Render>(entity);
		AnimationInstance* animInstance = render->animationInstance;

		float millisecondsPerFrame = 1000.0f / animInstance->framesPerSecond;

		uint32_t lastFrameTime = animInstance->lastFrameTime;
		if (lastFrameTime == 0)
		{
			lastFrameTime = Time::GetCurrentFrameTime();
		}
		uint32_t currentFrameTime = Time::GetCurrentFrameTime();
		animInstance->lastFrameTime = currentFrameTime;

		animInstance->frameTime += currentFrameTime - lastFrameTime;

		int animationLength = animInstance->animation->GetAnimationLength();

		unsigned int currentAnimationFrame = animInstance->currentAnimationFrame;

		if (animInstance->frameTime >= millisecondsPerFrame)
		{
			while (animInstance->frameTime >= millisecondsPerFrame)
			{
				animInstance->frameTime -= (int)millisecondsPerFrame;
				currentAnimationFrame++;
			}
			if (currentAnimationFrame >= animationLength)
			{
				if (animInstance->looping == true)
				{
					currentAnimationFrame = 0;
				}
				else
				{
					currentAnimationFrame = animationLength - 1;
				}
			}

		}

		animInstance->currentAnimationFrame = currentAnimationFrame;

	}
}