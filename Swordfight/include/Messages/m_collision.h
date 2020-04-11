#pragma once
#include "kecs/KECS.h"
struct CollisionMessage
{
	CollisionMessage(Entity entityOne, Entity entityTwo)
	{
		this->entityOne = entityOne;
		this->entityTwo = entityTwo;
	};
	Entity entityOne;
	Entity entityTwo;
};