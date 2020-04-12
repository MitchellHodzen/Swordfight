#pragma once
#include "Vector2.h"
struct Physics
{
	Physics()
	{
		maxSpeed = 0;
		acceleration = 0;
		friction = 0;
		velocity.SetX(0);
		velocity.SetY(0);
	};
	float maxSpeed;
	float acceleration;
	float friction;
	Vector2 velocity;
};