#pragma once
#include "kecs/KECS.h"
struct BulletFiredMessage
{
	BulletFiredMessage(Entity firingEntity)
	{
		this->firingEntity = firingEntity;
	};
	Entity firingEntity;
};