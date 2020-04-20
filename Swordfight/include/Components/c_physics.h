#pragma once
#include "Vector2.h"
struct Physics
{
	Physics()
	{
		maxSpeed = 0;
		friction = 0;
		velocity.SetX(0);
		velocity.SetY(0);
	};
	float maxSpeed;
	float friction;
	Vector2 velocity;
};