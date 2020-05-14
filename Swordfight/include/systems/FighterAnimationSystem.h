#pragma once
#include "Components/c_fighter.h"

class FighterAnimationSystem
{
public:
	void ResolveAnimations();

	void HandleBlockingAnimations(Fighter& fighter);
	void HandleReadyingAnimations(Fighter& fighter);
	void HandleAttackingAnimations(Fighter& fighter);
	void HandleClashingAnimations(Fighter& fighter);

	void HandleWalkAnimations(Fighter& fighter);
};