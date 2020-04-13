#include "Factories/BulletFactory.h"
#include "kecs/KECS.h"
#include "Components/c_transform.h"
#include "Components/c_physics.h"
#include "Components/c_render.h"
#include "ResourceManager.h"


Entity BulletFactory::ConstructBullet(int posX, int posY, float speed)
{

	Entity bullet = EntityManager::CreateEntity();

	if (bullet != -1)
	{
		Transform bulletTransform;
		bulletTransform.position.SetValues(posX, posY);
		EntityManager::SetComponent<Transform>(bullet, bulletTransform);

		Render bulletSprite;
		bulletSprite.texture = ResourceManager::GetInstance().GetTexture(ResourceManager::TextureKey::Bullet);
		EntityManager::SetComponent<Render>(bullet, bulletSprite);

		Physics bulletPhysics;
		bulletPhysics.velocity.SetValues(0, -speed);
		bulletPhysics.maxSpeed = speed;
		EntityManager::SetComponent<Physics>(bullet, bulletPhysics);

	}

	return bullet;
}
