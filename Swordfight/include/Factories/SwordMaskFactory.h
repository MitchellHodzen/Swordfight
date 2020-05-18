#pragma once
#include "kecs/KECS.h"
class SwordMaskFactory
{
public:
	static Entity ConstructSwordMask(int posX, int posY, bool isPlayerOne, Entity fighter);
private:
	SwordMaskFactory() {}
	~SwordMaskFactory() {}
};