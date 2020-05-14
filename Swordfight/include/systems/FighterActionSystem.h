#pragma once
#include "Components/c_fighter.h"

class FighterActionSystem
{
public:
	void ResolveActions();

	void TransitionState(Entity entity, Fighter& fighter, Fighter::State nextState);
	void TransitionFromState(Fighter& fighter);
	void TransitionToState(Fighter& fighter, Fighter::State nextState);

};