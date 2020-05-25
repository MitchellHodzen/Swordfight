#pragma once
#include "kecs/KECS.h"
#include <vector>
#include <iostream>
#include "Timer.h"
#include <cstdint>

//forward declaration


struct Fighter
{
	enum State {Blocking, Readying, Attacking, Clashing, Dashing, Dead};

	enum Stance {UP, MIDDLE, DOWN};

	enum Direction {LEFT, RIGHT};

	Fighter()
	{
		lastState = State::Blocking;
		currentState = State::Blocking;
	};
	~Fighter()
	{
		std::cout<<"Destroying fighter"<<std::endl;
	}

	Entity upperBody = -1;
	Entity lowerBody = -1;
	float moveSpeed = 0;
	float attackMoveSpeed = 0;
	float attackMaxMoveOffset = 0;
	Stance currentStance = Stance::MIDDLE;
	Timer attackTimer;
	uint32_t attackTimeMs = 0;
	uint32_t attackMaskSpawnMs = 0;
	bool hasSpawnedMask = false;
	int attackMaskColOffset = 0;
	int attackMaskWidth = 0;
	Timer dashTimer;
	uint32_t dashTimeMs = 0;
	float dashMoveSpeed = 0;
	float dashMaxMoveOffset = 0;
	Direction defaultDashDirection = Direction::LEFT;
	Direction currentDashDirection = Direction::LEFT;
	Direction primaryDirection = Direction::RIGHT;
	Timer clashTimer;
	uint32_t clashTimeMs = 0;
	float clashMoveSpeed = 0;
	float clashMaxMoveOffset = 0;

	bool shouldMove = false;
	Direction currentMovementDirection = Direction::RIGHT;

	bool isPlayerOne = false;

	void ChangeState(State newState)
	{
		//std::cout<<"Changing state from " << currentState << " to " << newState<<std::endl;
		lastState = currentState;
		currentState = newState;
	};

	State GetState()
	{
		return currentState;
	}

	State GetPreviousState()
	{
		return lastState;
	}

private:
	State currentState;
	State lastState;
};