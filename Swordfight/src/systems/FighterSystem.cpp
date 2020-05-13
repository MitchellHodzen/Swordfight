#include "systems/FighterSystem.h"
#include "kecs/KECS.h"
#include "Components/c_input.h"
#include "Components/c_physics.h"
#include "Components/c_fighter.h"
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

		/*
		if (uin->keyStates[UserInput::InputType::SPACE] && EntityManager::HasComponent<Cannon>(entity))
		{
			BulletFiredMessage message(entity);
			MessageManager::PushMessage<BulletFiredMessage>(message);
		}
		*/
	}
}

void FighterSystem::HandleBlockingStateInput(UserInput& userInput, Fighter& fighter)
{
	HandleMovementInput(userInput, fighter);
	HandleAttackDirectionInput(userInput, fighter);
}
void FighterSystem::HandleReadyingStateInput(UserInput& userInput, Fighter& fighter)
{
	HandleMovementInput(userInput, fighter);
	HandleAttackDirectionInput(userInput, fighter);
}
void FighterSystem::HandleAttackingStateInput(UserInput& userInput, Fighter& fighter)
{
	HandleAttackDirectionInput(userInput, fighter);
}
void FighterSystem::HandleClashingStateInput(UserInput& userInput, Fighter& fighter)
{
	HandleAttackDirectionInput(userInput, fighter);
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
}
void FighterSystem::TransitionToState(Fighter& fighter, Fighter::State nextState)
{
	//Do transition to next state
}
