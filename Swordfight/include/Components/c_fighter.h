#pragma once
#include "kecs/KECS.h"
//forward declaration


struct Fighter
{
	Fighter(Entity upperBody, Entity lowerBody, float moveSpeed)
	{
		this->upperBody = upperBody;
		this->lowerBody = lowerBody;
		this->moveSpeed = moveSpeed;
	};
	Entity upperBody;
	Entity lowerBody;
	float moveSpeed;

};