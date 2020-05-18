#include "systems/CollisionSystem.h"
#include "kecs/KECS.h"
#include "Components/c_transform.h"
#include "Components/c_boxcollider.h"
#include "Components/c_physics.h"
#include "MessageManager.h"
#include "Messages/m_collision.h"

CollisionSystem::CollisionSystem()
{
}


CollisionSystem::~CollisionSystem()
{
}

bool CollisionSystem::RectsColliding(BoxCollider& col1, Vector2& pos1, BoxCollider& col2, Vector2& pos2)
{

	Vector2 rectPos1(pos1.GetX() + col1.offsetX, pos1.GetY() + col1.offsetY);
	Vector2 rectPos2(pos2.GetX() + col2.offsetX, pos2.GetY() + col2.offsetY);

	if ((rectPos1.GetX() < rectPos2.GetX() + col2.width) &&
		(rectPos1.GetX() + col1.width > rectPos2.GetX()) &&
		(rectPos1.GetY() < rectPos2.GetY() + col2.height) &&
		(rectPos1.GetY() + col1.height > rectPos2.GetY()))
	{
		return true;
	}
	return false;
}

void CollisionSystem::CheckCollisions()
{
	std::vector<Entity> entities = EntityManager::GetEntitiesWithComponent<Transform, Physics, BoxCollider>(); //Only entities with physics will collide
	std::vector<Entity> collidableEntities = EntityManager::GetEntitiesWithComponent<Transform, BoxCollider>();
	for (Entity entity : entities)
	{
		for (Entity otherEntity : collidableEntities)
		{
			if (entity != otherEntity)
			{
				BoxCollider* col1 = EntityManager::GetComponent<BoxCollider>(entity);
				Transform* trans1 = EntityManager::GetComponent<Transform>(entity);
				BoxCollider* col2 = EntityManager::GetComponent<BoxCollider>(otherEntity);
				Transform* trans2 = EntityManager::GetComponent<Transform>(otherEntity);

				if (RectsColliding(*col1, trans1->position, *col2, trans2->position))
				{
					CollisionMessage message(entity, otherEntity);
					MessageManager::PushMessage<CollisionMessage>(message);
				}

			}
		}
	}
}