#pragma once
#include "kecs/KECS.h"
struct CollisionMessage
{
	CollisionMessage(Entity entity, Entity collidedWithEntity, float collidedWithX, float collidedWithWidth, bool collidedWithIsTrigger)
	{
		this->entity = entity;
		this->collidedWithEntity = collidedWithEntity;
		this->collidedWithX = collidedWithX;
		this->collidedWithWidth = collidedWithWidth;
		this->collidedWithIsTrigger = collidedWithIsTrigger;
	};
	Entity entity;
	Entity collidedWithEntity;
	float collidedWithX;
	float collidedWithWidth;
	bool collidedWithIsTrigger;
};