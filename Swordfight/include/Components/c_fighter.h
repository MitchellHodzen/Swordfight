#pragma once
#include "kecs/KECS.h"
#include <vector>
#include <iostream>
#include "Timer.h"
#include <cstdint>

//forward declaration


struct Fighter
{
	enum Action { MoveLeft, MoveRight, ReadyAttack, ReleaseAttack};

	enum State {Blocking, Readying, Attacking, Clashing};

	enum Stance {UP, MIDDLE, DOWN};

	Fighter()
	{
		ClearActions();
		lastState = State::Blocking;
		currentState = State::Blocking;
	};

	Entity upperBody = -1;
	Entity lowerBody = -1;
	float moveSpeed = 0;
	Stance currentStance = Stance::MIDDLE;
	Timer attackTimer;
	uint32_t attackTimeMs = 1000;

	void PrintActions()
	{
		for (int i = 0; i < actionCount; ++i)
		{
			std::cout<<actions[i]<<std::endl;
		}
	};

	void ChangeState(State newState)
	{
		std::cout<<"Changing state from " << currentState << " to " << newState<<std::endl;
		lastState = currentState;
		currentState = newState;
	};

	State GetState()
	{
		return currentState;
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
	const static int actionCount = 7;
	bool actions[actionCount];
	State currentState;
	State lastState;
};