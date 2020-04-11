#pragma once
#include "kecs/KECS.h"
class EnemyFactory
{
public:
	static Entity ConstructEnemy(int posX, int posY);
private:
	EnemyFactory() {}
	~EnemyFactory() {}
};

