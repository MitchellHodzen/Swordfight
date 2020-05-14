#pragma once
#include "Components/c_fighter.h"
struct UserInput;


class FighterSystem
{
public:
	void HandleUserInput();
	void ResolveActions();
	void ResolveAnimations();

	void HandleBlockingStateInput(UserInput& userInput, Fighter& fighter);
	void HandleReadyingStateInput(UserInput& userInput, Fighter& fighter);
	void HandleAttackingStateInput(UserInput& userInput, Fighter& fighter);
	void HandleClashingStateInput(UserInput& userInput, Fighter& fighter);

	void HandleBlockingAnimations(Fighter& fighter);
	void HandleReadyingAnimations(Fighter& fighter);
	void HandleAttackingAnimations(Fighter& fighter);
	void HandleClashingAnimations(Fighter& fighter);

	void HandleWalkAnimations(Fighter& fighter);

	void HandleMovementInput(UserInput& userInput, Fighter& fighter);
	void HandleAttackDirectionInput(UserInput& userInput, Fighter& fighter);

	void TransitionState(Fighter& fighter, Fighter::State nextState);
	void TransitionFromState(Fighter& fighter);
	void TransitionToState(Fighter& fighter, Fighter::State nextState);

};