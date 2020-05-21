#pragma once
#include "kecs/KECS.h"
#include "Components/c_fighter.h"

struct SwordCollisionMessage
{
	SwordCollisionMessage(Entity entity, Entity attacker, Fighter::Stance attackerStance, Fighter::State attackerState)
	{
		this->entity = entity;
		this->attacker = attacker;
		this->attackerStance = attackerStance;
		this->attackerState = attackerState;
	};
	Entity entity;
	Entity attacker;
	Fighter::Stance attackerStance;
	Fighter::State attackerState;
};