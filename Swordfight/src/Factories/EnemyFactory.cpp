#include "Factories/EnemyFactory.h"
#include "kecs/KECS.h"
#include "Components/c_transform.h"
#include "Components/c_physics.h"
#include "Components/c_render.h"
#include "ResourceManager.h"


Entity EnemyFactory::ConstructEnemy(int posX, int posY)
{

	Entity enemy = EntityManager::CreateEntity();

	if (enemy != -1)
	{
		Transform enemyTransform;
		enemyTransform.position.SetValues(posX, posY);
		EntityManager::SetComponent<Transform>(enemy, enemyTransform);

		Render enemySprite;
		enemySprite.texture = ResourceManager::GetInstance().GetTexture(ResourceManager::TextureKey::Enemy);
		EntityManager::SetComponent<Render>(enemy, enemySprite);

		Physics enemyPhysics;
		enemyPhysics.maxSpeed = 100;
		EntityManager::SetComponent<Physics>(enemy, enemyPhysics);

	}

	return enemy;
}
