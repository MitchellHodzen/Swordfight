#include "Factories/FighterFactory.h"
#include "kecs/KECS.h"
#include "Components/c_transform.h"
#include "Components/c_rect.h"
#include "Components/c_input.h"
#include "Components/c_physics.h"
#include "Components/c_Render.h"
#include "Components/c_cannon.h"
#include "Tags.h"
#include "ResourceManager.h"
#include "rendering/Spritesheet.h"
#include "rendering/Animation.h"


Entity FighterFactory::ConstructFighter(int posX, int posY)
{
	Entity playerShip = EntityManager::CreateEntity();
	if (playerShip != -1)
	{
		EntityManager::AddTag<Player>(playerShip);
		EntityManager::AddComponent<UserInput>(playerShip);

		Transform trans;
		trans.position.SetValues(posX, posY);
		EntityManager::SetComponent<Transform>(playerShip, trans);

		Cannon cannon;
		cannon.bulletSpeed = 100;
		EntityManager::SetComponent<Cannon>(playerShip, cannon);

		Rect rect;
		rect.width = 50;
		rect.height = 50;
		rect.offsetX = -rect.width / 2;
		rect.offsetY = -rect.height / 2;
		EntityManager::SetComponent<Rect>(playerShip, rect);

		Render render;
		render.spritesheet = ResourceManager::GetInstance().GetSpritesheet(ResourceManager::SpritesheetKey::Fighter);
		render.animationInstance = new AnimationInstance(*render.spritesheet->GetAnimation("idle"), 0, 16, true);
		//sprite.texture = ResourceManager::GetInstance().GetTexture(ResourceManager::TextureKey::Player);
		EntityManager::SetComponent<Render>(playerShip, render);

		Physics physics;
		physics.maxSpeed = 100;
		physics.acceleration = 500;
		physics.friction = 700;
		EntityManager::SetComponent<Physics>(playerShip, physics);
	}

	return playerShip;
}
