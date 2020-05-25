#pragma once
#include "kecs/KECS.h"
#include "Components/c_fighter.h"
struct FighterEvent
{
	enum EventType {ChangedState, ChangedStance, StartedWalk, EndedWalk, Blocked};

	FighterEvent(Entity baseEntity, Fighter::State oldState, Fighter::State newState)
	{
		this->eventType = EventType::ChangedState;
		this->baseEntity = baseEntity;
		this->oldState = oldState;
		this->newState = newState;
	};

	FighterEvent(Entity baseEntity, Fighter::Stance oldStance, Fighter::Stance newStance)
	{
		this->eventType = EventType::ChangedStance;
		this->baseEntity = baseEntity;
		this->oldStance = oldStance;
		this->newStance = newStance;
	};

	FighterEvent(Entity baseEntity, Fighter::Direction walkDirection)
	{
		this->eventType = EventType::StartedWalk;
		this->walkDirection = walkDirection;
		this->baseEntity = baseEntity;
	};

	FighterEvent(Entity baseEntity, EventType eventType)
	{
		this->eventType = eventType;
		this->baseEntity = baseEntity;
	};

	EventType eventType;
	Entity baseEntity;
	Fighter::State oldState;
	Fighter::State newState;
	Fighter::Stance oldStance;
	Fighter::Stance newStance;
	Fighter::Direction walkDirection;
};