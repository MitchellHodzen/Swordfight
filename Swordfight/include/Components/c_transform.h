#pragma once
#include "Vector2.h"
struct Transform 
{
	Transform()
	{
		position.SetX(0);
		position.SetY(0);
	};
	Vector2 position;
};