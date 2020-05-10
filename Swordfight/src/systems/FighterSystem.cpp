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

		//Movement is left and right
		if (uin->keyStates[UserInput::InputType::LEFT])
		{
			fighter->TakeAction(Fighter::Action::MoveLeft);
		}
		else if (uin->keyStates[UserInput::InputType::RIGHT])
		{
			fighter->TakeAction(Fighter::Action::MoveRight);
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