#pragma once
#include "kecs/KECS.h"
struct CollisionMessage
{
	CollisionMessage(Entity entity, float collidedWithX, float collidedWithWidth)
	{
		this->entity = entity;
		this->collidedWithX = collidedWithX;
		this->collidedWithWidth = collidedWithWidth;
	};
	Entity entity;
	float collidedWithX;
	float collidedWithWidth;
};