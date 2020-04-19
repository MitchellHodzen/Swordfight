#include "systems/CollisionSystem.h"
#include "kecs/KECS.h"
#include "Components/c_transform.h"
#include "Components/c_rect.h"
#include "Components/c_physics.h"
#include "MessageManager.h"
#include "Messages/m_collision.h"

CollisionSystem::CollisionSystem()
{
}


CollisionSystem::~CollisionSystem()
{
}

bool CollisionSystem::RectsColliding(Rect& rect1, Vector2& pos1, Rect& rect2, Vector2& pos2)
{

	Vector2 rectPos1(pos1.GetX() + rect1.offsetX, pos1.GetY() + rect1.offsetY);
	Vector2 rectPos2(pos2.GetX() + rect2.offsetX, pos2.GetY() + rect2.offsetY);

	if ((rectPos1.GetX() < rectPos2.GetX() + rect2.width) &&
		(rectPos1.GetX() + rect1.width > rectPos2.GetX()) &&
		(rectPos1.GetY() < rectPos2.GetY() + rect2.height) &&
		(rectPos1.GetY() + rect1.height > rectPos2.GetY()))
	{
		return true;
	}
	return false;
}

void CollisionSystem::CheckCollisions()
{
	std::vector<Entity> entities = EntityManager::GetEntitiesWithComponent<Transform, Physics, Rect>(); //Only entities with physics will collide
	std::vector<Entity> collidableEntities = EntityManager::GetEntitiesWithComponent<Transform, Rect>();
	for (Entity entity : entities)
	{
		for (Entity otherEntity : collidableEntities)
		{
			if (entity != otherEntity)
			{
				Rect* rect1 = EntityManager::GetComponent<Rect>(entity);
				Transform* trans1 = EntityManager::GetComponent<Transform>(entity);
				Rect* rect2 = EntityManager::GetComponent<Rect>(otherEntity);
				Transform* trans2 = EntityManager::GetComponent<Transform>(otherEntity);

				if (RectsColliding(*rect1, trans1->position, *rect2, trans2->position))
				{
					CollisionMessage message(entity, otherEntity);
					MessageManager::PushMessage<CollisionMessage>(message);
				}

			}
		}
	}
}