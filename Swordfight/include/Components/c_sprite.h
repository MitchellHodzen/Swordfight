#pragma once
//forward declaration
class Texture;
struct Sprite
{
	Sprite()
	{
		texture = nullptr;
	};
	Texture* texture;
};