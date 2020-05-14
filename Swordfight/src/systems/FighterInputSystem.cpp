#include "systems/FighterInputSystem.h"
#include "kecs/KECS.h"
#include "Components/c_input.h"
#include "Components/c_fighter.h"
#include "Time.h"

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
	if (userInput.keyPressed[UserInput::InputType::SPACE])
	{
		fighter.TakeAction(Fighter::Action::ReadyAttack);
	}
}
void FighterInputSystem::HandleReadyingStateInput(UserInput& userInput, Fighter& fighter)
{
	//std::cout<<"Handling readying state input" <<std::endl;
	HandleMovementInput(userInput, fighter);

	if (!(userInput.keyStates[UserInput::InputType::SPACE]))
	{
		fighter.TakeAction(Fighter::Action::ReleaseAttack);
	}
}
void FighterInputSystem::HandleAttackingStateInput(UserInput& userInput, Fighter& fighter)
{

}
void FighterInputSystem::HandleClashingStateInput(UserInput& userInput, Fighter& fighter)
{

}


void FighterInputSystem::HandleMovementInput(UserInput& userInput, Fighter& fighter)
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

void FighterInputSystem::HandleAttackDirectionInput(UserInput& userInput, Fighter& fighter)
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