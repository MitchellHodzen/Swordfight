#include "systems/PhysicsSystem.h"
#include "kecs/KECS.h"
#include "Components/c_transform.h"
#include "Components/c_physics.h"
#include "Components/c_horizontalcollider.h"
#include "Components/c_input.h"
#include "KTime.h"
#include "MessageManager.h"
#include "Messages/m_collision.h"



PhysicsSystem::PhysicsSystem()
{
}


PhysicsSystem::~PhysicsSystem()
{
}

void PhysicsSystem::HandleCollisions()
{
	while (MessageManager::NotEmpty<CollisionMessage>())
	{
		CollisionMessage message = MessageManager::PopMessage<CollisionMessage>();
		HorizontalCollider* col1 = EntityManager::GetComponent<HorizontalCollider>(message.entity);
		Transform* trans1 = EntityManager::GetComponent<Transform>(message.entity);

		float posX1 = trans1->position.GetX() + col1->offsetX;
		float posX2 = message.collidedWithX;
		float col2Width = message.collidedWithWidth;

		float newPosX1;

		//Handle horizontal collisions
		//Recalculation, send this data with the message?
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

		/*
		float posX1 = pos1.GetX() + col1.offsetX;
		float posX2 = pos2.GetX() + col1.offsetX;

		if ((posX1 < posX2 + col2.width) &&
			(posX1 + col1.width > posX2))
		{
			return true;
		}
		*/



		//Do collision handling here
		/*
		//Handle horizontal collisions
		if (rectPos1.x < rectPos2.x && rectPos1.x + rect1.width > rectPos2.x)
		{
			pos1.x = rectPos2.x - rect1.width - rect1.offsetX;
		}
		else if (rectPos1.x  < rectPos2.x + rect2.width && rectPos1.x + rect1.width > rectPos2.x + rect2.width)
		{
			pos1.x = rectPos2.x + rect2.width - rect1.offsetX;
		}
		*/

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