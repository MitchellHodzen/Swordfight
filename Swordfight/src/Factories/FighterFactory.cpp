#include "Factories/FighterFactory.h"
#include "kecs/KECS.h"
#include "Components/c_transform.h"
#include "Components/c_rect.h"
#include "Components/c_input.h"
#include "Components/c_physics.h"
#include "Components/c_Render.h"
#include "Components/c_cannon.h"
#include "Components/c_fighter.h"
#include "Tags.h"
#include "ResourceManager.h"
#include "rendering/Spritesheet.h"
#include "rendering/Animation.h"


Entity FighterFactory::ConstructFighter(int posX, int posY)
{
	Entity playerFighter = EntityManager::CreateEntity();
	Entity lowerBody = EntityManager::CreateEntity();
	Entity upperBody = EntityManager::CreateEntity();
	if (playerFighter != -1 && upperBody != -1 && lowerBody != -1)
	{
		EntityManager::AddTag<Player>(playerFighter);
		EntityManager::AddComponent<UserInput>(playerFighter);

		Fighter* fighter = EntityManager::AddComponent<Fighter>(playerFighter);
		if (fighter != nullptr)
		{
			fighter->lowerBody = lowerBody;
			fighter->upperBody = upperBody;
			fighter->moveSpeed = 500;
		}

		Transform* playerTrans = EntityManager::AddComponent<Transform>(playerFighter);
		if (playerTrans != nullptr)
		{
			playerTrans->position.SetValues(posX, posY);
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
			lowerBodyRender->animationInstance = new AnimationInstance(*lowerBodyRender->spritesheet->GetAnimation("feetIdle"), 0, 5, true);
		}

		Render* upperBodyRender = EntityManager::AddComponent<Render>(upperBody);
		if (upperBodyRender != nullptr)
		{
			upperBodyRender->spritesheet = ResourceManager::GetInstance().GetSpritesheet(ResourceManager::SpritesheetKey::Fighter);
			upperBodyRender->animationInstance = new AnimationInstance(*upperBodyRender->spritesheet->GetAnimation("animTest"), 0, 16, true);
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
