#pragma once
#include "kecs/KECS.h"
class FighterFactory
{
public:
	static Entity ConstructFighter(int posX, int posY);
private:
	FighterFactory() {}
	~FighterFactory() {}
};

