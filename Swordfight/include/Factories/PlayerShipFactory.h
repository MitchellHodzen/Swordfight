#pragma once
#include "kecs/KECS.h"
class PlayerShipFactory
{
public:
	static Entity ConstructPlayerShip(int posX, int posY);
private:
	PlayerShipFactory() {}
	~PlayerShipFactory() {}
};

