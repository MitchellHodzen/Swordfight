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
				Vector2 pos1 = GetParentModifiedPosition(*trans1);
				HorizontalCollider* col2 = EntityManager::GetComponent<HorizontalCollider>(otherEntity);
				Transform* trans2 = EntityManager::GetComponent<Transform>(otherEntity);
				Vector2 pos2 = GetParentModifiedPosition(*trans2);


				if (HorizontalColliding(*col1, pos1, *col2, pos2))
				{
					CollisionMessage message(entity, otherEntity, pos2.GetX() + col2->offsetX, col2->width, col2->isTrigger);
					MessageManager::PushMessage<CollisionMessage>(message);
				}

			}
		}
	}
}

Vector2 CollisionSystem::GetParentModifiedPosition(Transform& transform)
{
	if (EntityManager::IsValidEntity(transform.parentEntity))
	{
		//If there is a parent entity, add the transform
		Transform* parentTrans = EntityManager::GetComponent<Transform>(transform.parentEntity);
		if (parentTrans != nullptr)
		{
			return Vector2::ComponentWiseAddition(transform.position, parentTrans->position);
		}
	}
	return transform.position;
}