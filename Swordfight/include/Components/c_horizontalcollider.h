#pragma once
struct HorizontalCollider
{
	//Defines the left and right bounds for horizontal collision
	int offsetX = 0;
	int width = 0;
	bool isTrigger = false;
};