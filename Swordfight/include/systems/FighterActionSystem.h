#pragma once
#include "Components/c_fighter.h"
#include "Components/c_physics.h"
class FighterActionSystem
{
public:
	void ResolveActions();

	void ResolveMovement(Fighter& fighter, Physics& phys);

	void TransitionState(Entity entity, Fighter& fighter, Fighter::State nextState);
	void TransitionFromState(Fighter& fighter);
	void TransitionToState(Fighter& fighter, Fighter::State nextState);

};