#include "Factories/FighterFactory.h"
#include "kecs/KECS.h"
#include "Components/c_transform.h"
#include "Components/c_boxcollider.h"
#include "Components/c_input.h"
#include "Components/c_physics.h"
#include "Components/c_Render.h"
#include "Components/c_cannon.h"
#include "Components/c_fighter.h"
#include "Components/c_horizontalcollider.h"
#include "Tags.h"
#include "ResourceManager.h"
#include "rendering/Spritesheet.h"
#include "rendering/Animation.h"


Entity FighterFactory::ConstructFighter(int posX, int posY, bool isPlayerOne)
{
	Entity playerFighter = EntityManager::CreateEntity();
	Entity lowerBody = EntityManager::CreateEntity();
	Entity upperBody = EntityManager::CreateEntity();
	if (playerFighter != -1 && upperBody != -1 && lowerBody != -1)
	{
		bool isFlipped;
		Fighter::Direction primaryDirection;
		Fighter::Direction dashDirection;
		if (isPlayerOne)
		{
			primaryDirection = Fighter::Direction::RIGHT;
			dashDirection = Fighter::Direction::LEFT;
			isFlipped = false;
			EntityManager::AddTag<Player1>(playerFighter);
		}
		else
		{
			primaryDirection = Fighter::Direction::LEFT;
			dashDirection = Fighter::Direction::RIGHT;
			isFlipped = true;
			EntityManager::AddTag<Player2>(playerFighter);
		}
		
		UserInput* userInput = EntityManager::AddComponent<UserInput>(playerFighter);
		userInput->isPlayerOne = isPlayerOne;

		Fighter* fighter = EntityManager::AddComponent<Fighter>(playerFighter);
		if (fighter != nullptr)
		{
			if (isPlayerOne)
			{
				fighter->attackMaskColOffset = 20;
			}
			else
			{
				fighter->attackMaskColOffset = 20;
			}
			fighter->isPlayerOne = isPlayerOne;
			fighter->lowerBody = lowerBody;
			fighter->upperBody = upperBody;
			fighter->moveSpeed = 500;
			fighter->attackMoveSpeed = 5000;
			fighter->attackMaxMoveOffset = 150;
			fighter->attackTimeMs = 500;
			fighter->attackMaskSpawnMs = 50;
			fighter->attackMaskWidth = 100;
			fighter->dashTimeMs = 500;
			fighter->dashMoveSpeed = 7000;
			fighter->dashMaxMoveOffset = 100;
			fighter->defaultDashDirection = dashDirection;
			fighter->primaryDirection = primaryDirection;
			fighter->clashTimeMs = 500;
			fighter->clashMoveSpeed = 7000;
			fighter->clashMaxMoveOffset = 100;
		}

		Transform* playerTrans = EntityManager::AddComponent<Transform>(playerFighter);
		if (playerTrans != nullptr)
		{
			playerTrans->position.SetValues(posX, posY);
		}

		HorizontalCollider* horzCol = EntityManager::AddComponent<HorizontalCollider>(playerFighter);
		if (horzCol != nullptr)
		{
			horzCol->width = 50; //How wide the fighter is
			int modifier = isPlayerOne ? -1 : 1;
			horzCol->offsetX =  (-(horzCol->width / 2)) + (modifier * 20); //We put the collider in the middle of the transform
		}
		
		Transform* lowerBodyTrans = EntityManager::AddComponent<Transform>(lowerBody);
		if (lowerBodyTrans != nullptr)
		{
			lowerBodyTrans->position.SetValues(0,0);
			lowerBodyTrans->parentEntity = playerFighter;
		}

		Transform* upperBodyTrans = EntityManager::AddComponent<Transform>(upperBody);
		if (upperBodyTrans != nullptr)
		{
			upperBodyTrans->position.SetValues(0,0);
			upperBodyTrans->parentEntity = playerFighter;			
		}

		/*
		Rect rect;
		rect.width = 50;
		rect.height = 50;
		rect.offsetX = -rect.width / 2;
		rect.offsetY = -rect.height / 2;
		EntityManager::SetComponent<Rect>(playerShip, rect);
		*/

		Render* lowerBodyRender = EntityManager::AddComponent<Render>(lowerBody);
		if (lowerBodyRender != nullptr)
		{
			lowerBodyRender->spritesheet = ResourceManager::GetInstance().GetSpritesheet(ResourceManager::SpritesheetKey::Fighter);
			lowerBodyRender->SetAnimationInstance("feetIdle", 0, 5, true);
			lowerBodyRender->isFlipped = isFlipped;
		}

		Render* upperBodyRender = EntityManager::AddComponent<Render>(upperBody);
		if (upperBodyRender != nullptr)
		{
			upperBodyRender->spritesheet = ResourceManager::GetInstance().GetSpritesheet(ResourceManager::SpritesheetKey::Fighter);
			upperBodyRender->SetAnimationInstance("midGuard", 0, 0, false);
			upperBodyRender->isFlipped = isFlipped;
		}

		Physics* physics = EntityManager::AddComponent<Physics>(playerFighter);
		if (physics != nullptr)
		{
			physics->maxSpeed = 100;
			physics->friction = 700;
		}
	}

	return playerFighter;
}
