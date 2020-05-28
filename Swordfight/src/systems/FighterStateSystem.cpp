#include "systems/FighterStateSystem.h"
#include "kecs/KECS.h"
#include "Components/c_input.h"
#include "Components/c_fighter.h"
#include "Components/c_physics.h"
#include "Components/c_transform.h"
#include "KTime.h"
#include "Timer.h"
#include "MessageManager.h"
#include "InputCommandMapper.h"
#include "Messages/m_fighterEvent.h"
#include "Factories/SwordMaskFactory.h"
#include "Messages/m_swordcollision.h"
#include "Messages/m_collision.h"
#include "Tags.h"


void FighterStateSystem::UpdateFighterState()
{
	std::vector<Entity> entities = EntityManager::GetEntitiesWithComponent<UserInput, Fighter, Physics>();

	for (Entity entity : entities)
	{
		UserInput* uin = EntityManager::GetComponent<UserInput>(entity);
		Fighter* fighter = EntityManager::GetComponent<Fighter>(entity);
		Physics* physics = EntityManager::GetComponent<Physics>(entity);
		Fighter::State fighterState = fighter->GetState();

		//Reset Movement
		fighter->shouldMove = false;

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

void FighterStateSystem::ListenForSwordHitEvents()
{
	unsigned int messageOffsetIndex = MessageManager::GetNewOffsetIndex<CollisionMessage>();
	while (MessageManager::HasNext<CollisionMessage>(messageOffsetIndex))
	{
		CollisionMessage* message = MessageManager::PopMessage<CollisionMessage>(messageOffsetIndex);
		if (message != nullptr)
		{
			Entity entity = message->entity;
			Entity collidedWith = message->collidedWithEntity;

			//If the collided entity is a fighter, and the collided with is a trigger, it was hit by a sword
			if (EntityManager::HasComponent<Fighter>(entity) && message->collidedWithIsTrigger)
			{
				//The only triggers are currently swords, so a trigger hit is a sword hit
				//Ugly, not expandable, change this plz
				Transform* collidedTrans = EntityManager::GetComponent<Transform>(collidedWith);
				if (collidedTrans != nullptr)
				{
					Entity parentEntity = collidedTrans->parentEntity;
					if (EntityManager::IsValidEntity(parentEntity) && EntityManager::HasComponent<Fighter>(parentEntity))
					{
						Fighter* collidedWithFighter = EntityManager::GetComponent<Fighter>(parentEntity);
						Fighter::Stance collidedWithStance = collidedWithFighter->currentStance;
						SwordCollisionMessage swordColMsg(message->entity, parentEntity, collidedWithStance, collidedWithFighter->GetState());
						MessageManager::PushMessage<SwordCollisionMessage>(swordColMsg);
					}
				}
			}
		}
		
	}
}
void FighterStateSystem::HandleSwordHitEvents()
{
	unsigned int messageOffsetIndex = MessageManager::GetNewOffsetIndex<SwordCollisionMessage>();
	while (MessageManager::HasNext<SwordCollisionMessage>(messageOffsetIndex))
	{

		SwordCollisionMessage* message = MessageManager::PopMessage<SwordCollisionMessage>(messageOffsetIndex);
		if (message != nullptr)
		{
			Entity entity = message->entity;
			if (EntityManager::HasComponent<Fighter>(entity))
			{
				Fighter* fighter = EntityManager::GetComponent<Fighter>(entity);

				Entity attacker = message->attacker;
				Fighter::Stance attackerStance = message->attackerStance;
				Fighter::State attackerState = message->attackerState;

				Fighter::State fighterState = fighter->GetState();
				std::cout<<"Fighter stance: " << fighter->currentStance << ". Attacker stance: " << attackerStance << std::endl;
				std::cout<<"	Fighter state: " << fighterState << ". Attacker state: " << attackerState << std::endl;

				if (fighterState == Fighter::State::Blocking && fighter->currentStance == attackerStance)
				{
					//If the fighter is blocking correctly, the other entity gets blocked
					FighterEvent fighterEvent(attacker, FighterEvent::EventType::Blocked);
					MessageManager::PushMessage<FighterEvent>(fighterEvent);
				}
				else
				{
					//If the fighter is not blocking correctly, or is in another state
					bool dies = false;
					if (fighterState == Fighter::State::Dashing || fighterState == Fighter::State::Readying ||
						(fighterState == Fighter::State::Blocking && fighter->currentStance != attackerStance))
					{
						//If the fighter is dashing, readying, or blocking in the wrong position, the fighter dies
						dies = true;
					}
					else if (fighterState == Fighter::State::Attacking)
					{
						//If we both attack at the same time, at the same position, both fighters clash
						if (fighter->currentStance == attackerStance)
						{
							//This fighter clashes
							TransitionState(entity, *fighter, Fighter::State::Clashing);
							//The other fighter is blocked
							FighterEvent fighterEvent(attacker, FighterEvent::EventType::Blocked);
							MessageManager::PushMessage<FighterEvent>(fighterEvent);
						}
						else
						{
							dies = true;
						}
					}

					if (dies)
					{
						TransitionState(entity, *fighter, Fighter::State::Dead);
					}
				}
			}
		}
		
		
	}
}

void FighterStateSystem::HandleSwordBlockEvents()
{
	unsigned int messageOffsetIndex = MessageManager::GetNewOffsetIndex<FighterEvent>();
	while (MessageManager::HasNext<FighterEvent>(messageOffsetIndex))
	{

		FighterEvent* message = MessageManager::PopMessage<FighterEvent>(messageOffsetIndex);
		if (message != nullptr && message->eventType == FighterEvent::EventType::Blocked && EntityManager::HasComponent<Fighter>(message->baseEntity))
		{
			//If a fighter is blocked it goes to the clashing state
			TransitionState(message->baseEntity, *EntityManager::GetComponent<Fighter>(message->baseEntity), Fighter::State::Clashing);
		}
		
		
	}
}

void FighterStateSystem::RemoveOrphanedSwords()
{
	std::vector<Entity> entities = EntityManager::GetEntitiesWithTag<Sword>();

	for (Entity entity : entities)
	{
		//Get the parent of the sword's transform
		Transform* trans = EntityManager::GetComponent<Transform>(entity);
		if (trans != nullptr)
		{
			Entity parent = trans->parentEntity;
			if (EntityManager::IsValidEntity(parent) && EntityManager::HasComponent<Fighter>(parent))
			{
				//If there is a parent fighter, check the state
				Fighter* fighter = EntityManager::GetComponent<Fighter>(parent);

				Fighter::State fighterState = fighter->GetState();

				if (fighterState != Fighter::State::Attacking)
				{
					//If the fighter associated with this sword is no longer attacking, destroy it
					EntityManager::AddTag<Destroy>(entity);
				}
			}
		}
	}
}

void FighterStateSystem::UpdateBlockingState(Entity entity, UserInput& userInput, Fighter& fighter)
{
	//std::cout<<"Handling blocking state input" <<std::endl;
	HandleMovementInput(entity, userInput, fighter);
	HandleAttackDirectionInput(entity, userInput, fighter);

	//Dash overrides attack
	if (userInput.keyPressed[InputCommandMapper::Command::Dash])
	{
		TransitionState(entity, fighter, Fighter::State::Dashing);
	}
	else if (userInput.keyStates[InputCommandMapper::Command::Attack])
	{
		TransitionState(entity, fighter, Fighter::State::Readying);
	}
}
void FighterStateSystem::UpdateReadyingState(Entity entity, UserInput& userInput, Fighter& fighter)
{
	//std::cout<<"Handling readying state input" <<std::endl;
	HandleMovementInput(entity, userInput, fighter);

	//Dash overrides attack
	if (userInput.keyPressed[InputCommandMapper::Command::Dash])
	{
		TransitionState(entity, fighter, Fighter::State::Dashing);
	}
	else if (!(userInput.keyStates[InputCommandMapper::Command::Attack]))
	{
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
			fighter.shouldMove = true;
			fighter.currentMovementDirection = Fighter::Direction::RIGHT;
		}
		else
		{
			fighter.shouldMove = true;
			fighter.currentMovementDirection = Fighter::Direction::LEFT;
		}
	}
	else if (elapsedAttackTime >= fighter.attackTimeMs)
	{
		//If the attack is over, transition back to block
		TransitionState(entity, fighter, Fighter::State::Blocking);
	}
}
void FighterStateSystem::UpdateClashingState(Entity entity, Fighter& fighter)
{
	uint32_t elapsedClashTime = fighter.clashTimer.GetTimeElapsedMs();
	if (elapsedClashTime <= fighter.clashTimeMs / 2.0f)
	{
		//Clashing moves backwards
		if (fighter.primaryDirection == Fighter::Direction::RIGHT)
		{
			fighter.shouldMove = true;
			fighter.currentMovementDirection = Fighter::Direction::LEFT;
		}
		else
		{
			fighter.shouldMove = true;
			fighter.currentMovementDirection = Fighter::Direction::RIGHT;
		}
	}
	else if (elapsedClashTime >= fighter.clashTimeMs)
	{
		//If the attack is over, transition back to block
		TransitionState(entity, fighter, Fighter::State::Blocking);
	}
}
void FighterStateSystem::UpdateDashingState(Entity entity, Fighter& fighter)
{
	uint32_t elapsedDashTimeMs = fighter.dashTimer.GetTimeElapsedMs();
	if (elapsedDashTimeMs <= fighter.dashTimeMs / 2.0f)
	{			
		if (fighter.currentDashDirection == Fighter::Direction::LEFT)
		{
			fighter.shouldMove = true;
			fighter.currentMovementDirection = Fighter::Direction::LEFT;
		}
		else
		{
			fighter.shouldMove = true;
			fighter.currentMovementDirection = Fighter::Direction::RIGHT;
		}
	}
	else if (elapsedDashTimeMs >= fighter.dashTimeMs)
	{
		//If the dash is over, transition back to previous state
		TransitionState(entity, fighter, fighter.GetPreviousState());
	}
}


void FighterStateSystem::HandleMovementInput(Entity entity, UserInput& userInput, Fighter& fighter)
{
	//Check if dash is clicked
	//if (userInput.keyPressed[InputCommandMapper::Command::Dash])
	//{
	//	fighter.TakeAction(Fighter::Action::Dash);
	//}
	//Movement is left and right
	if (userInput.keyStates[InputCommandMapper::Command::MoveLeft])
	{
		FighterEvent startedMoveEvent(entity, Fighter::Direction::LEFT);
		MessageManager::PushMessage<FighterEvent>(startedMoveEvent);
		fighter.shouldMove = true;
		fighter.currentMovementDirection = Fighter::Direction::LEFT;
	}
	else if (userInput.keyStates[InputCommandMapper::Command::MoveRight])
	{
		FighterEvent startedMoveEvent(entity, Fighter::Direction::RIGHT);
		MessageManager::PushMessage<FighterEvent>(startedMoveEvent);
		fighter.shouldMove = true;
		fighter.currentMovementDirection = Fighter::Direction::RIGHT;
	}
	else
	{
		//If no input recorded, stop moving
		FighterEvent stoppedMoveEvent(entity, FighterEvent::EventType::EndedWalk);
		MessageManager::PushMessage<FighterEvent>(stoppedMoveEvent);
	}
	
}

void FighterStateSystem::HandleAttackDirectionInput(Entity entity, UserInput& userInput, Fighter& fighter)
{
	//Choose sword direction
	if (userInput.keyStates[InputCommandMapper::Command::StanceUp])
	{
		if (fighter.currentStance != Fighter::Stance::UP)
		{
			FighterEvent stanceChangeEvent(entity, fighter.currentStance, Fighter::Stance::UP);
			MessageManager::PushMessage<FighterEvent>(stanceChangeEvent);
			fighter.currentStance = Fighter::Stance::UP;
		}
	}
	else if (userInput.keyStates[InputCommandMapper::Command::StanceDown])
	{
		if (fighter.currentStance != Fighter::Stance::DOWN)
		{
			FighterEvent stanceChangeEvent(entity, fighter.currentStance, Fighter::Stance::DOWN);
			MessageManager::PushMessage<FighterEvent>(stanceChangeEvent);
			fighter.currentStance = Fighter::Stance::DOWN;
		}
	}
	else
	{
		//If no input is selected, default to center
		if (fighter.currentStance != Fighter::Stance::MIDDLE)
		{
			FighterEvent stanceChangeEvent(entity, fighter.currentStance, Fighter::Stance::MIDDLE);
			MessageManager::PushMessage<FighterEvent>(stanceChangeEvent);
			fighter.currentStance = Fighter::Stance::MIDDLE;
		}
	}
}


void FighterStateSystem::TransitionState(Entity entity, Fighter& fighter, Fighter::State nextState)
{
	Fighter::State oldState = fighter.GetState();
	TransitionFromState(entity, fighter);
	TransitionToState(entity, fighter, nextState);
	fighter.ChangeState(nextState);
	
	FighterEvent stateChangeEvent(entity, oldState, nextState);
	MessageManager::PushMessage<FighterEvent>(stateChangeEvent);
}
void FighterStateSystem::TransitionFromState(Entity entity, Fighter& fighter)
{
	//Do transition from current fighter state
	Fighter::State previousState = fighter.GetState();
	if (previousState == Fighter::State::Attacking)
	{
		fighter.hasSpawnedMask = false;
	}
	//std::cout<<"Transitioning from fighter state " << previousState << std::endl;
}
void FighterStateSystem::TransitionToState(Entity entity, Fighter& fighter, Fighter::State nextState)
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
			fighter.clashTimer.Restart();
			//ResetVelocity(entity);
			break;
		case Fighter::State::Dashing:
			fighter.dashTimer.Restart();
			if (fighter.shouldMove)
			{
				//If moving in a direction, dash in that direction
				fighter.currentDashDirection = fighter.currentMovementDirection;
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
	else if (fighter.GetState() == Fighter::State::Clashing)
	{
		maxSpeed += fighter.clashMaxMoveOffset;
		moveSpeed = fighter.clashMoveSpeed;
	}
	
	//Movement is left and right
	if (fighter.shouldMove && fighter.currentMovementDirection == Fighter::Direction::LEFT)
	{
		float currentVelocityX = phys.velocity.GetX();
		float newVelocityX = currentVelocityX - moveSpeed * KTime::GetDeltaTime();
		if (phys.velocity.GetX() > 0){
			//If we're moving right, give left movement a boost
			newVelocityX -= phys.friction * KTime::GetDeltaTime();
		}
		phys.velocity.SetX(newVelocityX);
	}
	else if (fighter.shouldMove && fighter.currentMovementDirection == Fighter::Direction::RIGHT)
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


void FighterStateSystem::ResetVelocity(Entity entity)
{
	if (EntityManager::HasComponent<Physics>(entity))
	{
		EntityManager::GetComponent<Physics>(entity)->ResetVelocity();
	}
}