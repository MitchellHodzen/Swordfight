#include "systems/FighterAnimationSystem.h"
#include "kecs/KECS.h"
#include "Components/c_physics.h"
#include "Components/c_fighter.h"
#include "Components/c_render.h"
#include "KTime.h"
#include "MessageManager.h"
#include "Messages/m_fighterEvent.h"

void FighterAnimationSystem::ResolveAnimations()
{
	//Handle state change messages
	unsigned int messageOffsetIndex = MessageManager::GetNewOffsetIndex<FighterEvent>();
	//while(MessageManager::NotEmpty<FighterEvent>())
	while(MessageManager::HasNext<FighterEvent>(messageOffsetIndex))
	{
		//FighterEvent msg = MessageManager::PopMessage<FighterEvent>();
		FighterEvent* msg = MessageManager::PopMessage<FighterEvent>(messageOffsetIndex);
		if (msg != nullptr)
		{
			Entity entity = msg->baseEntity;

			Fighter* fighter = EntityManager::GetComponent<Fighter>(entity);
			if (fighter != nullptr)
			{
				switch(msg->eventType)
				{
					case FighterEvent::EventType::StartedWalk:
					{
						StartWalkAnimation(*fighter, msg->walkDirection);
						break;
					}
					case FighterEvent::EventType::EndedWalk:
					{
						StopWalkAnimation(*fighter);
						break;
					}
					case FighterEvent::EventType::ChangedStance:
					{
						ChangeSwordPositionAnimation(*fighter, msg->newStance);
						break;
					}
					case FighterEvent::EventType::ChangedState:
					{
						if (msg->newState == Fighter::State::Blocking)
						{
							TransitionToBlockAnimation(*fighter);
						}
						else if (msg->newState == Fighter::State::Readying)
						{
							TransitionToReadyAnimation(*fighter);
						}
						else if (msg->newState == Fighter::State::Attacking)
						{
							TransitionToAttackAnimation(*fighter);
						}
						else if (msg->newState = Fighter::State::Dashing)
						{
							TransitionToDashAnimation(*fighter);
						}
						break;
					}
					default:
						break;
				}
			}
		}
	}
}

void FighterAnimationSystem::StartWalkAnimation(Fighter& fighter, Fighter::Direction direction)
{
	ApplyFighterAnimation(fighter.lowerBody, "feetWalk", 0, 5, true);
}
void FighterAnimationSystem::StopWalkAnimation(Fighter& fighter)
{
	ApplyFighterAnimation(fighter.lowerBody, "feetIdle", 0, 0, false);
}

void FighterAnimationSystem::ChangeSwordPositionAnimation(Fighter& fighter, Fighter::Stance stance)
{
	std::string animationName = "";

	switch(stance)
	{
		case Fighter::Stance::UP:
			animationName = "highGuard";
			break;
		case Fighter::Stance::MIDDLE:
			animationName = "midGuard";
			break;
		case Fighter::Stance::DOWN:
			animationName = "lowGuard";
			break;
		default:
			break;
	}
	ApplyFighterAnimation(fighter.upperBody, animationName, 0, 0, false);
}


void FighterAnimationSystem::TransitionToBlockAnimation(Fighter& fighter)
{
	ChangeSwordPositionAnimation(fighter, fighter.currentStance);
}

void FighterAnimationSystem::TransitionToReadyAnimation(Fighter& fighter)
{
	std::string animationName = "";

	switch(fighter.currentStance)
	{
		case Fighter::Stance::UP:
			animationName = "highReady";
			break;
		case Fighter::Stance::MIDDLE:
			animationName = "midReady";
			break;
		case Fighter::Stance::DOWN:
			animationName = "lowReady";
			break;
		default:
			break;
	}

	ApplyFighterAnimation(fighter.upperBody, animationName, 0, 0, false);
}

void FighterAnimationSystem::TransitionToAttackAnimation(Fighter& fighter)
{
	std::string animationName = "";
	int startFrame = 0;
	int fps = 16;
	bool looping = false;

	switch(fighter.currentStance)
	{
		case Fighter::Stance::UP:
			animationName = "highAttack";
			break;
		case Fighter::Stance::MIDDLE:
			animationName = "midAttack";
			break;
		case Fighter::Stance::DOWN:
			animationName = "lowAttack";
			break;
		default:
			break;
	}
	ApplyFighterAnimation(fighter.upperBody, animationName, 0, 16, false);
	StopWalkAnimation(fighter);
}

void FighterAnimationSystem::TransitionToDashAnimation(Fighter& fighter)
{
	StopWalkAnimation(fighter);
}

void FighterAnimationSystem::ApplyFighterAnimation(Entity fighterPart, std::string animationName, int startFrame, int fps, bool looping)
{
	if (!animationName.empty())
	{
		Render* render = EntityManager::GetComponent<Render>(fighterPart);
		if (render != nullptr && render->GetAnimationInstance()->animationName.compare(animationName) != 0)
		{
			render->SetAnimationInstance(animationName, startFrame, fps, looping);
		}
	}
}