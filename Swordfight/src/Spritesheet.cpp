#include <iostream>
#include "Spritesheet.h"
#include "Rectangle.h"


Spritesheet::~Spritesheet()
{
	//Delete all the sprite rectangles
	delete[] spriteList;
	spriteList = nullptr;
	//Spritesheet does not own the texture, so don't destroy it just dereference it
	texture = nullptr;
	//Reset all values to 0
	int spriteWidth = 0;
	int spriteHeight = 0;
	int rows = 0;
	int columns = 0;
	int spriteCount = 0;
}

bool Spritesheet::GenerateSpritesheet(Texture* texture, int spriteWidth, int spriteHeight, int rows, int columns, int spriteCount)
{
	//Set all the class level variables for the spritesheet
	this->texture = texture;
	this->spriteWidth = spriteWidth;
	this->spriteHeight = spriteHeight;
	this->rows = rows;
	this->columns = columns;
	this->spriteCount = spriteCount;

	//Instantiate our list of sprite rectangles
	spriteList = new Rectangle[spriteCount];

	//Starting at the top left of the spritesheet, move left to right
	int currentRow = 0;
	int currentColumn = 0;
	for(int i = 0; i < spriteCount; ++i)
	{
		//If we've passed the last column in the spritesheet, go to the next row
		if (currentColumn >= columns)
		{
			currentRow++;
			currentColumn = 0;
		}

		//For each sprite in the spritesheet, find the sprite rectangle
		spriteList[i].posX = currentColumn * spriteWidth;
		spriteList[i].posY = currentRow * spriteHeight;
		spriteList[i].width = spriteWidth - 1;
		spriteList[i].height = spriteHeight - 1;

		std::cout<<"Sprite " << i << ":"<<std::endl;
		std::cout<<"\tX: " << spriteList[i].posX<<std::endl;
		std::cout<<"\tY: " << spriteList[i].posY<<std::endl;
		std::cout<<"\tRight: " << spriteList[i].posX + spriteList[i].width - 1 << std::endl;
		std::cout<<"\tBottom: " << spriteList[i].posY + spriteList[i].height - 1 << std::endl;
		std::cout<<"\tWidth: " << spriteList[i].width<<std::endl;
		std::cout<<"\tHeight: " << spriteList[i].height<<std::endl;
		currentColumn ++;
		
	}
	return true;
}


Texture* Spritesheet::GetTexture()
{
	return texture;
}

Rectangle* Spritesheet::GetSprite(unsigned int spriteIndex)
{
	if (spriteIndex < spriteCount){
		return &(spriteList[spriteIndex]);
	}
	return nullptr;
}

int Spritesheet::GetSpriteCount()
{
	return spriteCount;
}

int Spritesheet::GetSpriteHeight()
{
	return spriteHeight;
}

int Spritesheet::GetSpriteWidth()
{
	return spriteWidth;
}