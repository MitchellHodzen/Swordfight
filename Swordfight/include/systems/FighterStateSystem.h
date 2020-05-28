#pragma once
#include "Components/c_fighter.h"

struct UserInput;
struct Physics;


class FighterStateSystem
{
public:

	void UpdateFighterState();

	void ListenForSwordHitEvents();
	void HandleSwordHitEvents();
	void HandleSwordBlockEvents();
	void RemoveOrphanedSwords();
	
private:
	void UpdateBlockingState(Entity entity, UserInput& userInput, Fighter& fighter);
	void UpdateReadyingState(Entity entity, UserInput& userInput, Fighter& fighter);
	void UpdateAttackingState(Entity entity, Fighter& fighter);
	void UpdateClashingState(Entity entity, Fighter& fighter);
	void UpdateDashingState(Entity entity, Fighter& fighter);

	void HandleMovementInput(Entity entity, UserInput& userInput, Fighter& fighter);
	void HandleAttackDirectionInput(Entity entity, UserInput& userInput, Fighter& fighter);

	void TransitionState(Entity entity, Fighter& fighter, Fighter::State nextState);
	void TransitionFromState(Entity entity, Fighter& fighter);
	void TransitionToState(Entity entity, Fighter& fighter, Fighter::State nextState);

	void ResolveMovement(Fighter& fighter, Physics& phys);
	void ResetVelocity(Entity entity);
};