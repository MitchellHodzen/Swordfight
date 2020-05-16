#include "systems/FighterAnimationSystem.h"
#include "kecs/KECS.h"
#include "Components/c_physics.h"
#include "Components/c_fighter.h"
#include "Components/c_render.h"
#include "Time.h"
#include "MessageManager.h"
#include "Messages/m_fighterStateChanged.h"

void FighterAnimationSystem::ResolveAnimations()
{
	std::vector<Entity> entities = EntityManager::GetEntitiesWithComponent<Physics, Fighter>();

	for (Entity entity : entities)
	{
		Fighter* fighter = EntityManager::GetComponent<Fighter>(entity);

		Fighter::State fighterState = fighter->GetState();

		switch(fighterState)
		{
			case Fighter::State::Blocking:
				HandleBlockingAnimations(*fighter);
				break;
			case Fighter::State::Readying:
				HandleReadyingAnimations(*fighter);
				break;
			case Fighter::State::Attacking:
				HandleAttackingAnimations(*fighter);
				break;
			case Fighter::State::Clashing:
				HandleClashingAnimations(*fighter);
				break;
			default:
				break;
		}
	}

	//Handle state change messages
	while(MessageManager::NotEmpty<FighterStateChangedMessage>())
	{
		FighterStateChangedMessage msg = MessageManager::PopMessage<FighterStateChangedMessage>();
		Entity fighterEntity = msg.fighterEntity;

		if (msg.newState == Fighter::State::Attacking)
		{
			Fighter* fighter = EntityManager::GetComponent<Fighter>(fighterEntity);

			if (fighter != nullptr)
			{
				bool attackAnimationFired = false;
				std::string animationName;
				int startFrame = 0;
				int fps = 16;
				bool looping = false;

				switch(fighter->currentStance)
				{
					case Fighter::Stance::UP:
						attackAnimationFired = true;
						animationName = "highAttack";
						break;
					case Fighter::Stance::MIDDLE:
						attackAnimationFired = true;
						animationName = "midAttack";
						break;
					case Fighter::Stance::DOWN:
						attackAnimationFired = true;
						animationName = "lowAttack";
						break;
					default:
						break;
				}
				if (attackAnimationFired)
				{
					ApplyFighterAnimation(fighter->upperBody, animationName, startFrame, fps, looping);
					ApplyFighterAnimation(fighter->lowerBody, "feetIdle", 0, 0, false);
				}
			}
		}
	}
}

void FighterAnimationSystem::HandleWalkAnimations(Fighter& fighter)
{
	bool setWalkAnimation = false;
	std::string animationName;
	int startFrame = 0;
	int fps = 0;
	bool looping = false;

	if (fighter.HasAction(Fighter::Action::MoveLeft) || fighter.HasAction(Fighter::Action::MoveRight))
	{
		animationName = "feetWalk";
		setWalkAnimation = true;
		fps = 5;
		looping = true;
	}
	else
	{
		animationName = "feetIdle";
		setWalkAnimation = true;
		fps = 0;
		looping = false;
	}

	if (setWalkAnimation)
	{
		ApplyFighterAnimation(fighter.lowerBody, animationName, startFrame, fps, looping);
	}
}

void FighterAnimationSystem::HandleBlockingAnimations(Fighter& fighter)
{	
	HandleWalkAnimations(fighter);

	bool guardAnimationSet = false;
	std::string animationName;
	int startFrame = 0;
	int fps = 0;
	bool looping = false;

	switch(fighter.currentStance)
	{
		case Fighter::Stance::UP:
			animationName = "highGuard";
			guardAnimationSet = true;
			break;
		case Fighter::Stance::MIDDLE:
			animationName = "midGuard";
			guardAnimationSet = true;
			break;
		case Fighter::Stance::DOWN:
			animationName = "lowGuard";
			guardAnimationSet = true;
			break;
		default:
			break;
	}
	if (guardAnimationSet)
	{
		ApplyFighterAnimation(fighter.upperBody, animationName, startFrame, fps, looping);
	}
}

void FighterAnimationSystem::HandleReadyingAnimations(Fighter& fighter)
{
	HandleWalkAnimations(fighter);

	bool readyAnimationSet = false;
	std::string animationName;
	int startFrame = 0;
	int fps = 0;
	bool looping = false;
	switch(fighter.currentStance)
	{
		case Fighter::Stance::UP:
			animationName = "highReady";
			readyAnimationSet = true;
			break;
		case Fighter::Stance::MIDDLE:
			animationName = "midReady";
			readyAnimationSet = true;
			break;
		case Fighter::Stance::DOWN:
			animationName = "lowReady";
			readyAnimationSet = true;
			break;
		default:
			break;
	}

	if (readyAnimationSet)
	{
		ApplyFighterAnimation(fighter.upperBody, animationName, startFrame, fps, looping);
	}
}
void FighterAnimationSystem::HandleAttackingAnimations(Fighter& fighter)
{

}
void FighterAnimationSystem::HandleClashingAnimations(Fighter& fighter)
{
	
}

void FighterAnimationSystem::ApplyFighterAnimation(Entity fighterPart, std::string animationName, int startFrame, int fps, bool looping)
{
	Render* render = EntityManager::GetComponent<Render>(fighterPart);
	if (render != nullptr && render->GetAnimationInstance()->animationName.compare(animationName) != 0)
	{
		render->SetAnimationInstance(animationName, startFrame, fps, looping);
	}
}