#include "systems/FighterSystem.h"
#include "kecs/KECS.h"
#include "Components/c_input.h"
#include "Components/c_physics.h"
#include "Components/c_fighter.h"
#include "Components/c_render.h"
#include "Time.h"

void FighterSystem::HandleUserInput()
{
	std::vector<Entity> entities = EntityManager::GetEntitiesWithComponent<UserInput, Fighter>();

	for (Entity entity : entities)
	{
		UserInput* uin = EntityManager::GetComponent<UserInput>(entity);
		Fighter* fighter = EntityManager::GetComponent<Fighter>(entity);

		Fighter::State fighterState = fighter->GetState();

		//Clear all actions
		fighter->ClearActions();

		//Make input queue instead?
		switch(fighterState)
		{
			case Fighter::State::Blocking:
				HandleBlockingStateInput(*uin, *fighter);
				break;
			case Fighter::State::Readying:
				HandleReadyingStateInput(*uin, *fighter);
				break;
			case Fighter::State::Attacking:
				HandleAttackingStateInput(*uin, *fighter);
				break;
			case Fighter::State::Clashing:
				HandleClashingStateInput(*uin, *fighter);
				break;
			default:
				break;
		}
	}
}

void FighterSystem::ResolveActions()
{
	std::vector<Entity> entities = EntityManager::GetEntitiesWithComponent<Physics, Fighter>();

	for (Entity entity : entities)
	{
		Physics* phys = EntityManager::GetComponent<Physics>(entity);
		Fighter* fighter = EntityManager::GetComponent<Fighter>(entity);

		float moveSpeed = fighter->moveSpeed;
		
		//Movement is left and right
		if (fighter->HasAction(Fighter::Action::MoveLeft))
		{
			float currentVelocityX = phys->velocity.GetX();
			float newVelocityX = currentVelocityX - moveSpeed * Time::GetDeltaTime();
			if (phys->velocity.GetX() > 0){
				//If we're moving right, give left movement a boost
				newVelocityX -= phys->friction * Time::GetDeltaTime();
			}
			phys->velocity.SetX(newVelocityX);
		}
		else if (fighter->HasAction(Fighter::Action::MoveRight))
		{
			float currentVelocityX = phys->velocity.GetX();
			float newVelocityX = currentVelocityX + moveSpeed * Time::GetDeltaTime();
			if (phys->velocity.GetX() < 0){
				//If we're moving right, give left movement a boost
				newVelocityX += phys->friction * Time::GetDeltaTime();
			}
			phys->velocity.SetX(newVelocityX);
		}
		else{
			//If no input is selected, we apply friction
			if (phys->velocity.GetX() > 0){
				//If we're moving right, apply friction to the left
				phys->velocity.SetX(phys->velocity.GetX() - phys->friction * Time::GetDeltaTime());

				//If after applying friction the velocity direction changed, zero the velocity out
				if (phys->velocity.GetX() < 0){
					phys->velocity.SetX(0);
				}

			}
			else if (phys->velocity.GetX() < 0){
				//If we're moving left, apply friction to the right
				phys->velocity.SetX(phys->velocity.GetX() + phys->friction * Time::GetDeltaTime());

				//If after applying friction the velocity direction changed, zero the velocity out
				if (phys->velocity.GetX() > 0){
					phys->velocity.SetX(0);
				}
			}
		}

		//Clamping speed to max speed
		if (phys->velocity.GetMagnitude() > phys->maxSpeed)
		{
			phys->velocity.SetMagnitude(phys->maxSpeed);
		}

		//Is this the right place to handle state transitions? Maybe a different system?
		if (fighter->HasAction(Fighter::Action::ReadyAttack))
		{
			TransitionState(*fighter, Fighter::State::Readying);
		}
		if (fighter->HasAction(Fighter::Action::ReleaseAttack))
		{
			TransitionState(*fighter, Fighter::State::Attacking);
		}
		/*
		if (uin->keyStates[UserInput::InputType::SPACE] && EntityManager::HasComponent<Cannon>(entity))
		{
			BulletFiredMessage message(entity);
			MessageManager::PushMessage<BulletFiredMessage>(message);
		}
		*/
	}
}

void FighterSystem::ResolveAnimations()
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
}

void FighterSystem::HandleWalkAnimations(Fighter& fighter)
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

void FighterSystem::HandleBlockingAnimations(Fighter& fighter)
{	
	HandleWalkAnimations(fighter);

	bool guardAnimationSet = false;
	std::string animationName;
	int startFrame = 0;
	int fps = 0;
	bool looping = false;

	if (fighter.HasAction(Fighter::Action::SwordCenter))
	{
		animationName = "midGuard";
		guardAnimationSet = true;
		startFrame = 0;
		fps = 0;
		looping = false;
	}
	else if (fighter.HasAction(Fighter::Action::SwordUp))
	{
		animationName = "highGuard";
		guardAnimationSet = true;
		startFrame = 0;
		fps = 0;
		looping = false;
	}
	else if (fighter.HasAction(Fighter::Action::SwordDown))
	{
		animationName = "lowGuard";
		guardAnimationSet = true;
		startFrame = 0;
		fps = 0;
		looping = false;
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
void FighterSystem::HandleReadyingAnimations(Fighter& fighter)
{
	HandleWalkAnimations(fighter);

	bool readyAnimationSet = false;
	std::string animationName;
	int startFrame = 0;
	int fps = 0;
	bool looping = false;

	if (fighter.HasAction(Fighter::Action::SwordCenter))
	{
		animationName = "midReady";
		readyAnimationSet = true;
		startFrame = 0;
		fps = 0;
		looping = false;
	}
	else if (fighter.HasAction(Fighter::Action::SwordUp))
	{
		animationName = "highReady";
		readyAnimationSet = true;
		startFrame = 0;
		fps = 0;
		looping = false;
	}
	else if (fighter.HasAction(Fighter::Action::SwordDown))
	{
		animationName = "lowReady";
		readyAnimationSet = true;
		startFrame = 0;
		fps = 0;
		looping = false;
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
void FighterSystem::HandleAttackingAnimations(Fighter& fighter)
{

}
void FighterSystem::HandleClashingAnimations(Fighter& fighter)
{
	
}

void FighterSystem::HandleBlockingStateInput(UserInput& userInput, Fighter& fighter)
{
	//std::cout<<"Handling blocking state input" <<std::endl;
	HandleMovementInput(userInput, fighter);
	HandleAttackDirectionInput(userInput, fighter);

	//If blocking, we can get into a ready attack state by holding spacebar
	if (userInput.keyPressed[UserInput::InputType::SPACE])
	{
		fighter.TakeAction(Fighter::Action::ReadyAttack);
	}
}
void FighterSystem::HandleReadyingStateInput(UserInput& userInput, Fighter& fighter)
{
	//std::cout<<"Handling readying state input" <<std::endl;
	HandleMovementInput(userInput, fighter);

	if (!(userInput.keyStates[UserInput::InputType::SPACE]))
	{
		fighter.TakeAction(Fighter::Action::ReleaseAttack);
	}
}
void FighterSystem::HandleAttackingStateInput(UserInput& userInput, Fighter& fighter)
{

}
void FighterSystem::HandleClashingStateInput(UserInput& userInput, Fighter& fighter)
{

}


void FighterSystem::HandleMovementInput(UserInput& userInput, Fighter& fighter)
{
	//Movement is left and right
	if (userInput.keyStates[UserInput::InputType::LEFT])
	{
		fighter.TakeAction(Fighter::Action::MoveLeft);
	}
	else if (userInput.keyStates[UserInput::InputType::RIGHT])
	{
		fighter.TakeAction(Fighter::Action::MoveRight);
	}
}

void FighterSystem::HandleAttackDirectionInput(UserInput& userInput, Fighter& fighter)
{
	//Choose sword direction
	if (userInput.keyStates[UserInput::InputType::UP])
	{
		fighter.TakeAction(Fighter::Action::SwordUp);
	}
	else if (userInput.keyStates[UserInput::InputType::DOWN])
	{
		fighter.TakeAction(Fighter::Action::SwordDown);
	}
	else
	{
		//If no input is selected, default to center
		fighter.TakeAction(Fighter::Action::SwordCenter);
	}
}


void FighterSystem::TransitionState(Fighter& fighter, Fighter::State nextState)
{
	TransitionFromState(fighter);
	TransitionToState(fighter, nextState);
	fighter.ChangeState(nextState);
}
void FighterSystem::TransitionFromState(Fighter& fighter)
{
	//Do transition from current fighter state
	Fighter::State previousState = fighter.GetState();
	std::cout<<"Transitioning from fighter state " << previousState << std::endl;
}
void FighterSystem::TransitionToState(Fighter& fighter, Fighter::State nextState)
{
	//Do transition to next state
	std::cout<<"Transitioning to fighter state " << nextState << std::endl;
}
