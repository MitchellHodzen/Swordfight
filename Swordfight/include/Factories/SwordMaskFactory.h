#pragma once
#include "kecs/KECS.h"
class SwordMaskFactory
{
public:
	static Entity ConstructSwordMask(bool isPlayerOne, Entity fighter);
private:
	SwordMaskFactory() {}
	~SwordMaskFactory() {}
};