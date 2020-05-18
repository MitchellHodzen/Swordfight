#pragma once
#include "Components/c_fighter.h"

struct UserInput;
struct Physics;


class FighterStateSystem
{
public:

	void UpdateFighterState();
	
private:
	void UpdateBlockingState(Entity entity, UserInput& userInput, Fighter& fighter);
	void UpdateReadyingState(Entity entity, UserInput& userInput, Fighter& fighter);
	void UpdateAttackingState(Entity entity, Fighter& fighter);
	void UpdateClashingState(Entity entity, Fighter& fighter);
	void UpdateDashingState(Entity entity, Fighter& fighter);

	void HandleMovementInput(UserInput& userInput, Fighter& fighter);
	void HandleAttackDirectionInput(UserInput& userInput, Fighter& fighter);

	void TransitionState(Entity entity, Fighter& fighter, Fighter::State nextState);
	void TransitionFromState(Fighter& fighter);
	void TransitionToState(Fighter& fighter, Fighter::State nextState);

	void ResolveMovement(Fighter& fighter, Physics& phys);
};