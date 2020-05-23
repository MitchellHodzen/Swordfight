#include "systems/PhysicsSystem.h"
#include "kecs/KECS.h"
#include "Components/c_transform.h"
#include "Components/c_physics.h"
#include "Components/c_horizontalcollider.h"
#include "Components/c_input.h"
#include "KTime.h"
#include "MessageManager.h"
#include "Messages/m_collision.h"
#include "Messages/m_swordcollision.h"



PhysicsSystem::PhysicsSystem()
{
}


PhysicsSystem::~PhysicsSystem()
{
}

void PhysicsSystem::HandleCollisions()
{
	unsigned int messageOffsetIndex = MessageManager::GetNewOffsetIndex<CollisionMessage>();
	while (MessageManager::HasNext<CollisionMessage>(messageOffsetIndex))
	{
		CollisionMessage* message = MessageManager::PopMessage<CollisionMessage>(messageOffsetIndex);
		if (message != nullptr)
		{
			HorizontalCollider* col1 = EntityManager::GetComponent<HorizontalCollider>(message->entity);
			Transform* trans1 = EntityManager::GetComponent<Transform>(message->entity);

			float posX1 = trans1->position.GetX() + col1->offsetX;
			float posX2 = message->collidedWithX;
			float col2Width = message->collidedWithWidth;

			if (!message->collidedWithIsTrigger)
			{
				//If the collider wasn't a trigger, effect position
				float newPosX1;

				//Handle horizontal collisions
				if (posX1 < posX2)
				{
					//If colliding on the left
					newPosX1 = posX2 - col1->width - col1->offsetX;
				}
				else
				{
					//If colliding on the right
					newPosX1 =  posX2 + col2Width - col1->offsetX;
				}

				trans1->position.SetX(newPosX1);
			}
		}
	}
}

void PhysicsSystem::ApplyPhysics()
{
	std::vector<Entity> entities = EntityManager::GetEntitiesWithComponent<Transform, Physics>();
	for (Entity entity : entities)
	{
		Transform* trans = EntityManager::GetComponent<Transform>(entity);
		Physics* phys = EntityManager::GetComponent<Physics>(entity);

		//Apply delta time to velocity and assign the resulting vector to the movement vector
		Vector2 movementVector = phys->velocity * KTime::GetDeltaTime();

		//Delta time has already been applied
		trans->position += movementVector;
	}
}