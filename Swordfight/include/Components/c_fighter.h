#pragma once
#include "kecs/KECS.h"
#include <vector>
#include <iostream>
#include "Timer.h"
#include <cstdint>

//forward declaration


struct Fighter
{
	enum Action { MoveLeft, MoveRight};

	enum State {Blocking, Readying, Attacking, Clashing, Dashing};

	enum Stance {UP, MIDDLE, DOWN};

	enum Direction {LEFT, RIGHT};

	Fighter()
	{
		ClearActions();
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

	bool isPlayerOne = false;
	

	void PrintActions()
	{
		for (int i = 0; i < actionCount; ++i)
		{
			std::cout<<actions[i]<<std::endl;
		}
	};

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

	void TakeAction(Action action)
	{
		actions[action] = true;
	};

	bool HasAction(Action action)
	{
		return actions[action];
	};

	void ClearActions()
	{
		//Zeroing out actions
		for (int i = 0; i < actionCount; ++i)
		{
			actions[i] = false;
		}
	};

private:
	const static int actionCount = 2;
	bool actions[actionCount];
	State currentState;
	State lastState;
};