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
				int fps = 0;
				bool looping = false;

				switch(fighter->currentStance)
				{
					case Fighter::Stance::UP:
						attackAnimationFired = true;
						animationName = "highAttack";
						startFrame = 0;
						fps = 16;
						looping = false;
						break;
					case Fighter::Stance::MIDDLE:
						attackAnimationFired = true;
						animationName = "midAttack";
						startFrame = 0;
						fps = 16;
						looping = false;
						break;
					case Fighter::Stance::DOWN:
						attackAnimationFired = true;
						animationName = "lowAttack";
						startFrame = 0;
						fps = 16;
						looping = false;
						break;
					default:
						break;
				}
				if (attackAnimationFired)
				{
					Entity fighterUpperBody = fighter->upperBody;
					Render* upperBodyRender = EntityManager::GetComponent<Render>(fighterUpperBody);
					if (upperBodyRender != nullptr)
					{
						upperBodyRender->SetAnimationInstance(animationName, startFrame, fps, looping);
					}
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
		startFrame = 0;
		fps = 5;
		looping = true;
	}
	else
	{
		animationName = "feetIdle";
		setWalkAnimation = true;
		startFrame = 0;
		fps = 0;
		looping = false;
	}

	if (setWalkAnimation)
	{
		Entity fighterLowerBody = fighter.lowerBody;
		Render* lowerBodyRender = EntityManager::GetComponent<Render>(fighterLowerBody);
		if (lowerBodyRender != nullptr)
		{
			if (lowerBodyRender->GetAnimationInstance()->animationName.compare(animationName) != 0)
			{
				std::cout<<"Different animations. Animation Instance: " << lowerBodyRender->GetAnimationInstance()->animationName << ". New: " << animationName <<std::endl;
				lowerBodyRender->SetAnimationInstance(animationName, startFrame, fps, looping);
			}
		}
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
			startFrame = 0;
			fps = 0;
			looping = false;
			break;
		case Fighter::Stance::MIDDLE:
			animationName = "midGuard";
			guardAnimationSet = true;
			startFrame = 0;
			fps = 0;
			looping = false;
			break;
		case Fighter::Stance::DOWN:
			animationName = "lowGuard";
			guardAnimationSet = true;
			startFrame = 0;
			fps = 0;
			looping = false;
			break;
		default:
			break;
	}
	if (guardAnimationSet)
	{
		Entity fighterUpperBody = fighter.upperBody;
		Render* upperBodyRender = EntityManager::GetComponent<Render>(fighterUpperBody);
		if (upperBodyRender != nullptr)
		{
			upperBodyRender->SetAnimationInstance(animationName, startFrame, fps, looping);
		}
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
			startFrame = 0;
			fps = 0;
			looping = false;
			break;
		case Fighter::Stance::MIDDLE:
			animationName = "midReady";
			readyAnimationSet = true;
			startFrame = 0;
			fps = 0;
			looping = false;
			break;
		case Fighter::Stance::DOWN:
			animationName = "lowReady";
			readyAnimationSet = true;
			startFrame = 0;
			fps = 0;
			looping = false;
			break;
		default:
			break;
	}
	
	if (readyAnimationSet)
	{
		Entity fighterUpperBody = fighter.upperBody;
		Render* upperBodyRender = EntityManager::GetComponent<Render>(fighterUpperBody);
		if (upperBodyRender != nullptr)
		{
			upperBodyRender->SetAnimationInstance(animationName, startFrame, fps, looping);
		}
	}
}
void FighterAnimationSystem::HandleAttackingAnimations(Fighter& fighter)
{

}
void FighterAnimationSystem::HandleClashingAnimations(Fighter& fighter)
{
	
}