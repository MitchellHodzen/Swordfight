#pragma once
#include <string>
#include "Components/c_fighter.h"

class FighterAnimationSystem
{
public:
	void ResolveAnimations();
	
private:
	void TransitionToBlockAnimation(Fighter& fighter);
	void TransitionToReadyAnimation(Fighter& fighter);
	void TransitionToAttackAnimation(Fighter& fighter);
	void TransitionToDashAnimation(Fighter& fighter);

	void StartWalkAnimation(Fighter& fighter, Fighter::Direction direction);
	void StopWalkAnimation(Fighter& fighter);
	
	void ChangeSwordPositionAnimation(Fighter& fighter, Fighter::Stance stance);

	void ApplyFighterAnimation(Entity fighterPart, std::string animationName, int startFrame, int fps, bool looping);
};