#pragma once
#include "kecs/KECS.h"
struct CollisionMessage
{
	CollisionMessage(Entity entity, float collidedWithX, float collidedWithWidth, bool collidedWithIsTrigger)
	{
		this->entity = entity;
		this->collidedWithX = collidedWithX;
		this->collidedWithWidth = collidedWithWidth;
		this->collidedWithIsTrigger = collidedWithIsTrigger;
	};
	Entity entity;
	float collidedWithX;
	float collidedWithWidth;
	bool collidedWithIsTrigger;
};