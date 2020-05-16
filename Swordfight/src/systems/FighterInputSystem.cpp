#include "systems/FighterInputSystem.h"
#include "kecs/KECS.h"
#include "Components/c_input.h"
#include "Components/c_fighter.h"
#include "Time.h"
#include "InputCommandMapper.h"

void FighterInputSystem::HandleUserInput()
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

void FighterInputSystem::HandleBlockingStateInput(UserInput& userInput, Fighter& fighter)
{
	//std::cout<<"Handling blocking state input" <<std::endl;
	HandleMovementInput(userInput, fighter);
	HandleAttackDirectionInput(userInput, fighter);

	//If blocking, we can get into a ready attack state by holding spacebar
	if (userInput.keyPressed[InputCommandMapper::Command::Attack])
	{
		fighter.TakeAction(Fighter::Action::ReadyAttack);
	}
}
void FighterInputSystem::HandleReadyingStateInput(UserInput& userInput, Fighter& fighter)
{
	//std::cout<<"Handling readying state input" <<std::endl;
	HandleMovementInput(userInput, fighter);

	if (!(userInput.keyStates[InputCommandMapper::Command::Attack]))
	{
		fighter.TakeAction(Fighter::Action::ReleaseAttack);
	}
}
void FighterInputSystem::HandleAttackingStateInput(UserInput& userInput, Fighter& fighter)
{
	//No attack specific input
}
void FighterInputSystem::HandleClashingStateInput(UserInput& userInput, Fighter& fighter)
{
	//No clash specific input
}


void FighterInputSystem::HandleMovementInput(UserInput& userInput, Fighter& fighter)
{
	//Check if dash is clicked
	if (userInput.keyPressed[InputCommandMapper::Command::Dash])
	{
		fighter.TakeAction(Fighter::Action::Dash);
	}
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

void FighterInputSystem::HandleAttackDirectionInput(UserInput& userInput, Fighter& fighter)
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