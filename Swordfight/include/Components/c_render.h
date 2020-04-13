#pragma once
//forward declaration
class Texture;
struct Render
{
	Render()
	{
		texture = nullptr;
	};
	Texture* texture;
};