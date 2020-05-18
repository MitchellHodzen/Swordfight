#pragma once
struct HorizontalCollider
{
	//Defines the left and right bounds for horizontal collision
	HorizontalCollider()
	{
		offsetX = 0;
		width = 0; 
	};
	int offsetX;
	int width;
};