#include "systems/FighterActionSystem.h"
#include "kecs/KECS.h"
#include "Components/c_physics.h"
#include "Components/c_fighter.h"
#include "Components/c_render.h"
#include "Time.h"

void FighterActionSystem::ResolveActions()
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

void FighterActionSystem::TransitionState(Fighter& fighter, Fighter::State nextState)
{
	TransitionFromState(fighter);
	TransitionToState(fighter, nextState);
	fighter.ChangeState(nextState);
}
void FighterActionSystem::TransitionFromState(Fighter& fighter)
{
	//Do transition from current fighter state
	Fighter::State previousState = fighter.GetState();
	std::cout<<"Transitioning from fighter state " << previousState << std::endl;
}
void FighterActionSystem::TransitionToState(Fighter& fighter, Fighter::State nextState)
{
	//Do transition to next state
	std::cout<<"Transitioning to fighter state " << nextState << std::endl;
}
