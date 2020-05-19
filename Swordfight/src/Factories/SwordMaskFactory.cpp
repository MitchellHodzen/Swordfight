#include "Factories/SwordMaskFactory.h"
#include "kecs/KECS.h"
#include "Components/c_transform.h"
#include "Components/c_horizontalcollider.h"
#include "Tags.h"


Entity SwordMaskFactory::ConstructSwordMask(bool isPlayerOne, Entity fighter)
{
	Entity swordMask = EntityManager::CreateEntity();
	if (swordMask != -1)
	{
		if (isPlayerOne)
		{
			EntityManager::AddTag<Player1>(swordMask);
		}
		else
		{
			EntityManager::AddTag<Player2>(swordMask);
		}

		Transform* swordTrans = EntityManager::AddComponent<Transform>(swordMask);
		if (swordTrans != nullptr)
		{
			swordTrans->position.SetValues(0, 0);
			swordTrans->parentEntity = fighter;
		}

		HorizontalCollider* horzCol = EntityManager::AddComponent<HorizontalCollider>(swordMask);
		if (horzCol != nullptr)
		{
			horzCol->width = 120; //How wide the sword
			int modifier = 1;
			int additionalOffset = 35;
			if (!isPlayerOne)
			{
				modifier = -1;
				additionalOffset += horzCol->width;
			}
			horzCol->offsetX = modifier *(additionalOffset);//modifier * 10;//((horzCol->width / 2)); //We put the collider in the middle of the transform
			horzCol->isTrigger = true;
		}
	}

	return swordMask;
}
