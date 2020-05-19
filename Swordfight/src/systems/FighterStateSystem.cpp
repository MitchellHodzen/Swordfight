#include "systems/FighterStateSystem.h"
#include "kecs/KECS.h"
#include "Components/c_input.h"
#include "Components/c_fighter.h"
#include "Components/c_physics.h"
#include "KTime.h"
#include "Timer.h"
#include "MessageManager.h"
#include "InputCommandMapper.h"
#include "Messages/m_fighterStateChanged.h"
#include "Factories/SwordMaskFactory.h"


void FighterStateSystem::UpdateFighterState()
{
	std::vector<Entity> entities = EntityManager::GetEntitiesWithComponent<UserInput, Fighter, Physics>();

	for (Entity entity : entities)
	{
		UserInput* uin = EntityManager::GetComponent<UserInput>(entity);
		Fighter* fighter = EntityManager::GetComponent<Fighter>(entity);
		Physics* physics = EntityManager::GetComponent<Physics>(entity);
		Fighter::State fighterState = fighter->GetState();

		//Clear all actions
		fighter->ClearActions();

		//Make input queue instead?
		switch(fighterState)
		{
			case Fighter::State::Blocking:
				UpdateBlockingState(entity, *uin, *fighter);
				break;
			case Fighter::State::Readying:
				UpdateReadyingState(entity, *uin, *fighter);
				break;
			case Fighter::State::Attacking:
				UpdateAttackingState(entity, *fighter);
				break;
			case Fighter::State::Clashing:
				UpdateClashingState(entity, *fighter);
				break;
			case Fighter::State::Dashing:
				UpdateDashingState(entity, *fighter);
			default:
				break;
		}

		ResolveMovement(*fighter, *physics);
	}
}

void FighterStateSystem::UpdateBlockingState(Entity entity, UserInput& userInput, Fighter& fighter)
{
	//std::cout<<"Handling blocking state input" <<std::endl;
	HandleMovementInput(userInput, fighter);
	HandleAttackDirectionInput(userInput, fighter);

	//Dash overrides attack
	if (userInput.keyPressed[InputCommandMapper::Command::Dash])
	{
		fighter.TakeAction(Fighter::Action::Dash);
		TransitionState(entity, fighter, Fighter::State::Dashing);
	}
	else if (userInput.keyStates[InputCommandMapper::Command::Attack])
	{
		fighter.TakeAction(Fighter::Action::ReadyAttack);
		TransitionState(entity, fighter, Fighter::State::Readying);
	}
}
void FighterStateSystem::UpdateReadyingState(Entity entity, UserInput& userInput, Fighter& fighter)
{
	//std::cout<<"Handling readying state input" <<std::endl;
	HandleMovementInput(userInput, fighter);

	//Dash overrides attack
	if (userInput.keyPressed[InputCommandMapper::Command::Dash])
	{
		fighter.TakeAction(Fighter::Action::Dash);
		TransitionState(entity, fighter, Fighter::State::Dashing);
	}
	else if (!(userInput.keyStates[InputCommandMapper::Command::Attack]))
	{
		fighter.TakeAction(Fighter::Action::ReleaseAttack);
		TransitionState(entity, fighter, Fighter::State::Attacking);
	}
}
void FighterStateSystem::UpdateAttackingState(Entity entity, Fighter& fighter)
{
	uint32_t elapsedAttackTime = fighter.attackTimer.GetTimeElapsedMs();
	if (elapsedAttackTime >= fighter.attackMaskSpawnMs && !fighter.hasSpawnedMask)
	{
		//Spawn the attack mask
		Entity attackMask = SwordMaskFactory::ConstructSwordMask(fighter.isPlayerOne, entity);
		fighter.hasSpawnedMask = true;
	}
	if (elapsedAttackTime <= fighter.attackTimeMs / 2.0f)
	{
		if (fighter.primaryDirection == Fighter::Direction::RIGHT)
		{
			fighter.TakeAction(Fighter::Action::MoveRight);
		}
		else
		{
			fighter.TakeAction(Fighter::Action::MoveLeft);
		}
		
		fighter.TakeAction(Fighter::Action::MoveRight);
	}
	else if (elapsedAttackTime >= fighter.attackTimeMs)
	{
		//If the attack is over, transition back to block
		fighter.hasSpawnedMask = false;
		TransitionState(entity, fighter, Fighter::State::Blocking);
	}
}
void FighterStateSystem::UpdateClashingState(Entity entity, Fighter& fighter)
{
	//Nothing yet
}
void FighterStateSystem::UpdateDashingState(Entity entity, Fighter& fighter)
{
	uint32_t elapsedDashTimeMs = fighter.dashTimer.GetTimeElapsedMs();
	if (elapsedDashTimeMs <= fighter.dashTimeMs / 2.0f)
	{			
		if (fighter.currentDashDirection == Fighter::Direction::LEFT)
		{
			fighter.TakeAction(Fighter::Action::MoveLeft);
		}
		else
		{
			fighter.TakeAction(Fighter::Action::MoveRight);
		}
	}
	else if (elapsedDashTimeMs >= fighter.dashTimeMs)
	{
		//If the dash is over, transition back to previous state
		TransitionState(entity, fighter, fighter.GetPreviousState());
	}
}


void FighterStateSystem::HandleMovementInput(UserInput& userInput, Fighter& fighter)
{
	//Check if dash is clicked
	//if (userInput.keyPressed[InputCommandMapper::Command::Dash])
	//{
	//	fighter.TakeAction(Fighter::Action::Dash);
	//}
	//Movement is left and right
	if (userInput.keyStates[InputCommandMapper::Command::MoveLeft])
	{
		fighter.TakeAction(Fighter::Action::MoveLeft);
	}
	else if (userInput.keyStates[InputCommandMapper::Command::MoveRight])
	{
		fighter.TakeAction(Fighter::Action::MoveRight);
	}
}

void FighterStateSystem::HandleAttackDirectionInput(UserInput& userInput, Fighter& fighter)
{
	//Choose sword direction
	if (userInput.keyStates[InputCommandMapper::Command::StanceUp])
	{
		fighter.currentStance = Fighter::Stance::UP;
	}
	else if (userInput.keyStates[InputCommandMapper::Command::StanceDown])
	{
		fighter.currentStance = Fighter::Stance::DOWN;
	}
	else
	{
		//If no input is selected, default to center
		fighter.currentStance = Fighter::Stance::MIDDLE;
	}
}


void FighterStateSystem::TransitionState(Entity entity, Fighter& fighter, Fighter::State nextState)
{
	Fighter::State oldState = fighter.GetState();
	TransitionFromState(fighter);
	TransitionToState(fighter, nextState);
	fighter.ChangeState(nextState);
	
	FighterStateChangedMessage message(entity, oldState, nextState);
	MessageManager::PushMessage<FighterStateChangedMessage>(message);
}
void FighterStateSystem::TransitionFromState(Fighter& fighter)
{
	//Do transition from current fighter state
	Fighter::State previousState = fighter.GetState();
	//std::cout<<"Transitioning from fighter state " << previousState << std::endl;
}
void FighterStateSystem::TransitionToState(Fighter& fighter, Fighter::State nextState)
{
	//Do transition to next state
	//std::cout<<"Transitioning to fighter state " << nextState << std::endl;
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
		case Fighter::State::Dashing:
			fighter.dashTimer.Restart();
			if (fighter.HasAction(Fighter::Action::MoveRight))
			{
				fighter.currentDashDirection = Fighter::Direction::RIGHT;
			}
			else if (fighter.HasAction(Fighter::Action::MoveLeft))
			{
				fighter.currentDashDirection = Fighter::Direction::LEFT;
			}
			else
			{
				//If no input pressed for the dash, go backwards
				fighter.currentDashDirection = fighter.defaultDashDirection;
			}
			
		default:
			break;
	}
}

void FighterStateSystem::ResolveMovement(Fighter& fighter, Physics& phys)
{
	float moveSpeed = fighter.moveSpeed;
	float maxSpeed = phys.maxSpeed;
	if (fighter.GetState() == Fighter::State::Attacking)
	{
		maxSpeed += fighter.attackMaxMoveOffset;
		moveSpeed = fighter.attackMoveSpeed;
	}
	else if (fighter.GetState() == Fighter::State::Dashing)
	{
		maxSpeed += fighter.dashMaxMoveOffset;
		moveSpeed = fighter.dashMoveSpeed;
	}
	
	//Movement is left and right
	if (fighter.HasAction(Fighter::Action::MoveLeft))
	{
		float currentVelocityX = phys.velocity.GetX();
		float newVelocityX = currentVelocityX - moveSpeed * KTime::GetDeltaTime();
		if (phys.velocity.GetX() > 0){
			//If we're moving right, give left movement a boost
			newVelocityX -= phys.friction * KTime::GetDeltaTime();
		}
		phys.velocity.SetX(newVelocityX);
	}
	else if (fighter.HasAction(Fighter::Action::MoveRight))
	{
		float currentVelocityX = phys.velocity.GetX();
		float newVelocityX = currentVelocityX + moveSpeed * KTime::GetDeltaTime();
		if (phys.velocity.GetX() < 0){
			//If we're moving right, give left movement a boost
			newVelocityX += phys.friction * KTime::GetDeltaTime();
		}
		phys.velocity.SetX(newVelocityX);
	}
	else{
		//If no input is selected, we apply friction
		if (phys.velocity.GetX() > 0){
			//If we're moving right, apply friction to the left
			phys.velocity.SetX(phys.velocity.GetX() - phys.friction * KTime::GetDeltaTime());

			//If after applying friction the velocity direction changed, zero the velocity out
			if (phys.velocity.GetX() < 0){
				phys.velocity.SetX(0);
			}

		}
		else if (phys.velocity.GetX() < 0){
			//If we're moving left, apply friction to the right
			phys.velocity.SetX(phys.velocity.GetX() + phys.friction * KTime::GetDeltaTime());

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