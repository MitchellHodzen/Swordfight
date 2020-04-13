#pragma once

class Texture;
class Rectangle;

class Spritesheet
{
public:
	~Spritesheet();

	bool GenerateSpritesheet(Texture* texture, int spriteWidth, int spriteHeight, int rows, int columns, int spriteCount);


	Texture* GetTexture();
	Rectangle* GetSprite(unsigned int spriteIndex);

	int GetSpriteCount();
	int GetSpriteHeight();
	int GetSpriteWidth();

private:

	Rectangle* spriteList = nullptr;
	Texture* texture = nullptr;
	int spriteWidth = 0;
	int spriteHeight = 0;
	int rows = 0;
	int columns = 0;
	int spriteCount = 0;
};

