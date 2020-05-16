#include "systems/FighterActionSystem.h"
#include "kecs/KECS.h"
#include "Components/c_physics.h"
#include "Components/c_fighter.h"
#include "Components/c_render.h"
#include "Time.h"
#include "Timer.h"
#include "MessageManager.h"
#include "Messages/m_fighterStateChanged.h"

void FighterActionSystem::ResolveActions()
{
	std::vector<Entity> entities = EntityManager::GetEntitiesWithComponent<Physics, Fighter>();

	for (Entity entity : entities)
	{
		Physics* phys = EntityManager::GetComponent<Physics>(entity);
		Fighter* fighter = EntityManager::GetComponent<Fighter>(entity);

		//Is this the right place to handle state transitions? Maybe a different system?
		if (fighter->HasAction(Fighter::Action::ReadyAttack))
		{
			TransitionState(entity, *fighter, Fighter::State::Readying);
		}
		else if (fighter->HasAction(Fighter::Action::ReleaseAttack))
		{
			TransitionState(entity, *fighter, Fighter::State::Attacking);
		}

		//handle state specifics (consolidate?)
		Fighter::State fighterState = fighter->GetState();

		//Special cases for each case
		switch(fighterState)
		{
			case Fighter::State::Attacking: {
				uint32_t elapsedAttackTime = fighter->attackTimer.GetTimeElapsedMs();
				if (elapsedAttackTime <= fighter->attackTimeMs / 2.0f)
				{
					fighter->TakeAction(Fighter::Action::MoveRight);
				}
				else if (elapsedAttackTime >= fighter->attackTimeMs)
				{
					//If the attack is over, transition back to block
					TransitionState(entity, *fighter, Fighter::State::Blocking);
				}
				break;
			}
			case Fighter::State::Clashing:
				break;
			default:
				break;
		}

		ResolveMovement(*fighter, *phys);
	}
}

void FighterActionSystem::ResolveMovement(Fighter& fighter, Physics& phys)
{
	float moveSpeed = fighter.moveSpeed;
	float maxSpeed = phys.maxSpeed;
	if (fighter.GetState() == Fighter::State::Attacking)
	{
		maxSpeed += fighter.bigMoveMaxOffset;
		moveSpeed = fighter.bigMoveSpeed;
	}
	
	//Movement is left and right
	if (fighter.HasAction(Fighter::Action::MoveLeft))
	{
		float currentVelocityX = phys.velocity.GetX();
		float newVelocityX = currentVelocityX - moveSpeed * Time::GetDeltaTime();
		if (phys.velocity.GetX() > 0){
			//If we're moving right, give left movement a boost
			newVelocityX -= phys.friction * Time::GetDeltaTime();
		}
		phys.velocity.SetX(newVelocityX);
	}
	else if (fighter.HasAction(Fighter::Action::MoveRight))
	{
		float currentVelocityX = phys.velocity.GetX();
		float newVelocityX = currentVelocityX + moveSpeed * Time::GetDeltaTime();
		if (phys.velocity.GetX() < 0){
			//If we're moving right, give left movement a boost
			newVelocityX += phys.friction * Time::GetDeltaTime();
		}
		phys.velocity.SetX(newVelocityX);
	}
	else{
		//If no input is selected, we apply friction
		if (phys.velocity.GetX() > 0){
			//If we're moving right, apply friction to the left
			phys.velocity.SetX(phys.velocity.GetX() - phys.friction * Time::GetDeltaTime());

			//If after applying friction the velocity direction changed, zero the velocity out
			if (phys.velocity.GetX() < 0){
				phys.velocity.SetX(0);
			}

		}
		else if (phys.velocity.GetX() < 0){
			//If we're moving left, apply friction to the right
			phys.velocity.SetX(phys.velocity.GetX() + phys.friction * Time::GetDeltaTime());

			//If after applying friction the velocity direction changed, zero the velocity out
			if (phys.velocity.GetX() > 0){
				phys.velocity.SetX(0);
			}
		}
	}

	//Clamping speed to max speed
	if (phys.velocity.GetMagnitude() > maxSpeed)
	{
		phys.velocity.SetMagnitude(maxSpeed);
	}
}

void FighterActionSystem::TransitionState(Entity entity, Fighter& fighter, Fighter::State nextState)
{
	Fighter::State oldState = fighter.GetState();
	TransitionFromState(fighter);
	TransitionToState(fighter, nextState);
	fighter.ChangeState(nextState);
	
	FighterStateChangedMessage message(entity, oldState, nextState);
	MessageManager::PushMessage<FighterStateChangedMessage>(message);
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
	switch(nextState)
	{
		case Fighter::State::Blocking:
			break;
		case Fighter::State::Readying:
			break;
		case Fighter::State::Attacking:
			fighter.attackTimer.Restart();
			break;
		case Fighter::State::Clashing:
			break;
		default:
			break;
	}
}
