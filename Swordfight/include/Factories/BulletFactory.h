#pragma once
#include "kecs/KECS.h"
class BulletFactory
{
public:
	static Entity ConstructBullet(int posX, int posY, float speed);
private:
	BulletFactory() {}
	~BulletFactory() {}
};

