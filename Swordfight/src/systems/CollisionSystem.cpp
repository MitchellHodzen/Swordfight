#include "systems/CollisionSystem.h"
#include "kecs/KECS.h"
#include "Components/c_transform.h"
#include "Components/c_horizontalcollider.h"
#include "Components/c_physics.h"
#include "MessageManager.h"
#include "Messages/m_collision.h"

CollisionSystem::CollisionSystem()
{
}


CollisionSystem::~CollisionSystem()
{
}

bool CollisionSystem::HorizontalColliding(HorizontalCollider& col1, Vector2& pos1, HorizontalCollider& col2, Vector2& pos2)
{
	float posX1 = pos1.GetX() + col1.offsetX;
	float posX2 = pos2.GetX() + col2.offsetX;

	if ((posX1 < posX2 + col2.width) &&
		(posX1 + col1.width > posX2))
	{
		return true;
	}
	return false;
}

void CollisionSystem::CheckCollisions()
{
	std::vector<Entity> entities = EntityManager::GetEntitiesWithComponent<Transform, Physics, HorizontalCollider>(); //Only entities with physics will collide
	std::vector<Entity> collidableEntities = EntityManager::GetEntitiesWithComponent<Transform, HorizontalCollider>();
	for (Entity entity : entities)
	{
		for (Entity otherEntity : collidableEntities)
		{
			if (entity != otherEntity)
			{
				HorizontalCollider* col1 = EntityManager::GetComponent<HorizontalCollider>(entity);
				Transform* trans1 = EntityManager::GetComponent<Transform>(entity);
				HorizontalCollider* col2 = EntityManager::GetComponent<HorizontalCollider>(otherEntity);
				Transform* trans2 = EntityManager::GetComponent<Transform>(otherEntity);

				if (HorizontalColliding(*col1, trans1->position, *col2, trans2->position))
				{
					CollisionMessage message(entity, trans2->position.GetX() + col2->offsetX, col2->width);
					MessageManager::PushMessage<CollisionMessage>(message);
				}

			}
		}
	}
}