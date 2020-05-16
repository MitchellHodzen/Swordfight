#pragma once
#include "kecs/KECS.h"
#include <vector>
#include <iostream>
#include "Timer.h"
#include <cstdint>

//forward declaration


struct Fighter
{
	enum Action { MoveLeft, MoveRight, ReadyAttack, ReleaseAttack, Dash};

	enum State {Blocking, Readying, Attacking, Clashing, Dashing};

	enum Stance {UP, MIDDLE, DOWN};

	enum DashDirection {LEFT, RIGHT};

	Fighter()
	{
		ClearActions();
		lastState = State::Blocking;
		currentState = State::Blocking;
	};

	Entity upperBody = -1;
	Entity lowerBody = -1;
	float moveSpeed = 0;
	float attackMoveSpeed = 0;
	float attackMaxMoveOffset = 0;
	Stance currentStance = Stance::MIDDLE;
	Timer attackTimer;
	uint32_t attackTimeMs = 0;
	Timer dashTimer;
	uint32_t dashTimeMs = 0;
	float dashMoveSpeed = 0;
	float dashMaxMoveOffset = 0;
	DashDirection currentDashDirection = DashDirection::LEFT;

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
	const static int actionCount = 5;
	bool actions[actionCount];
	State currentState;
	State lastState;
};