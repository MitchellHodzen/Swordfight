#pragma once
#include "kecs/KECS.h"
#include "Components/c_fighter.h"
struct FighterStateChangedMessage
{
	FighterStateChangedMessage(Entity fighterEntity, Fighter::State oldState, Fighter::State newState)
	{
		this->fighterEntity = fighterEntity;
		this->oldState = oldState;
		this->newState = newState;
	};
	Entity fighterEntity;
	Fighter::State oldState;
	Fighter::State newState;
};