#include "BulletSystem.h"
#include "kecs/KECS.h"
#include "MessageManager.h"
#include "Messages/m_bulletFired.h"
#include "Factories/BulletFactory.h"
#include "Components/c_cannon.h"
#include "Components/c_transform.h"

BulletSystem::BulletSystem()
{
}


BulletSystem::~BulletSystem()
{
}

void BulletSystem::FireBullets()
{
	while (MessageManager::NotEmpty<BulletFiredMessage>())
	{
		BulletFiredMessage message = MessageManager::PopMessage<BulletFiredMessage>();
		Transform* trans = EntityManager::GetComponent<Transform>(message.firingEntity);
		Cannon* cannon = EntityManager::GetComponent<Cannon>(message.firingEntity);
		BulletFactory::ConstructBullet(trans->position.GetX(), trans->position.GetY(), cannon->bulletSpeed);
	}
}