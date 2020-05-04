#pragma once
#include "kecs/KECS.h"
#include <vector>
#include <iostream>
//forward declaration


struct Fighter
{
	enum Action { MoveLeft, MoveRight, SwordUp, SwordDown, SwordCenter, ReadyAttack };
	Fighter()
	{
		ClearActions();
	};

	Entity upperBody = -1;
	Entity lowerBody = -1;
	float moveSpeed = 0;

	void PrintActions()
	{
		for (int i = 0; i < actionCount; ++i)
		{
			std::cout<<actions[i]<<std::endl;
		}
	};

	void TakeAction(Action action)
	{
		actions[action] = true;
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
	const static int actionCount = 6;
	bool actions[actionCount];
};