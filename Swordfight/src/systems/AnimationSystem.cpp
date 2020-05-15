#include "systems/AnimationSystem.h"
#include "kecs/KECS.h"
#include "Components/c_render.h"
#include "rendering/Animation.h"
#include "rendering/Spritesheet.h"
#include "Time.h"
#include "Timer.h"

void AnimationSystem::AdvanceAnimations()
{
	std::vector<Entity> entities = EntityManager::GetEntitiesWithComponent<Render>();
	for (Entity entity : entities)
	{
		Render* render = EntityManager::GetComponent<Render>(entity);
		AnimationInstance* animInstance = render->GetAnimationInstance();

		float millisecondsPerFrame = 1000.0f / animInstance->framesPerSecond;

		uint32_t frameTime = animInstance->animationFrameTimer.GetTimeElapsedMs();

		int animationLength = animInstance->animation->GetAnimationLength();

		unsigned int currentAnimationFrame = animInstance->currentAnimationFrame;

		if (frameTime >= millisecondsPerFrame)
		{
			while (frameTime >= millisecondsPerFrame)
			{
				frameTime -= (int)millisecondsPerFrame;
				currentAnimationFrame++;
			}
			//Restart the timer, using the leftover frame time
			animInstance->animationFrameTimer.Restart(frameTime);
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