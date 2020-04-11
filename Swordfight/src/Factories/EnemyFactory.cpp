#include "Factories/EnemyFactory.h"
#include "kecs/KECS.h"
#include "Components/c_transform.h"
#include "Components/c_physics.h"
#include "Components/c_sprite.h"
#include "ResourceManager.h"


Entity EnemyFactory::ConstructEnemy(int posX, int posY)
{

	Entity enemy = EntityManager::CreateEntity();

	if (enemy != -1)
	{
		Transform enemyTransform;
		enemyTransform.position.SetValues(posX, posY);
		EntityManager::SetComponent<Transform>(enemy, enemyTransform);

		Sprite enemySprite;
		enemySprite.texture = ResourceManager::GetInstance().GetTexture(ResourceManager::SpriteKey::Enemy);
		EntityManager::SetComponent<Sprite>(enemy, enemySprite);

		Physics enemyPhysics;
		enemyPhysics.maxSpeed = 100;
		EntityManager::SetComponent<Physics>(enemy, enemyPhysics);

	}

	return enemy;
}
