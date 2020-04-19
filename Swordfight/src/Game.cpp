#include "Game.h"
#include <iostream>
#include "kecs/KECS.h"
#include "rendering/KRenderer.h"
#include "Components/c_transform.h"
#include "Components/c_rect.h"
#include "Tags.h"
#include "Components/c_input.h"
#include "Components/c_physics.h"
#include "Time.h"
#include "systems/RenderSystem.h"
#include "ResourceManager.h"
#include "Components/c_render.h"
#include "systems/InputSystem.h"
#include "systems/CollisionSystem.h"
#include "systems/PhysicsSystem.h"
#include "Factories/PlayerShipFactory.h"
#include "Factories/EnemyFactory.h"
#include "Components/c_cannon.h"
#include "systems/BulletSystem.h"
#include "InputManager.h"
#include "systems/AnimationSystem.h"

Game::~Game()
{
	delete inputSystem;
	delete renderSystem;
	delete collisionSystem;
	delete physicsSystem;
	delete bulletSystem;
	delete kRenderer;
}


void Game::Run(){

	EntityManager::SetUpComponents<Transform, Rect, UserInput, Render, Physics, Cannon>();
	EntityManager::SetUpTags<Player, Enemy, Wall>();

	Entity player1 = PlayerShipFactory::ConstructPlayerShip(300, 300);

	Entity enemy = EnemyFactory::ConstructEnemy(100, 100);

	std::cout<<"Starting game"<<std::endl;
	while (!quit)
	{
		Time::CalculateDeltaTime();
		InputManager::GetInstance().UpdateInput();
		quit = InputManager::GetInstance().ShouldQuit();
		Render* render = EntityManager::GetComponent<Render>(player1);
		render->spriteIndex = (render->spriteIndex + 1) % 18;
		inputSystem->GetUserInput();
		inputSystem->HandleUserInput();
		physicsSystem->ApplyPhysics();
		bulletSystem->FireBullets();
		collisionSystem->CheckCollisions();
		physicsSystem->HandleCollisions();
		animationSystem->AdvanceAnimations();
		renderSystem->Draw(*kRenderer);
	}
}

bool Game::SetUp(int screenWidth, int screenHeight) {
	bool success = true;
	this->screenWidth = screenWidth;
	this->screenHeight = screenHeight;
	kRenderer = new KRenderer();
	if (kRenderer->Initialize(screenWidth, screenHeight))
	{
		if (ResourceManager::GetInstance().Initialize(*kRenderer))
		{
			//Initialize systems
			renderSystem = new RenderSystem();
			inputSystem = new InputSystem();
			collisionSystem = new CollisionSystem();
			physicsSystem = new PhysicsSystem();
			bulletSystem = new BulletSystem();
			animationSystem = new AnimationSystem();
		}
		else
		{
			std::cout<<"Could not initialize resource manager"<<std::endl;
			success = false;
		}
	}
	else
	{
		std::cout<<"Could not initialize renderer"<<std::endl;
		success = false;
	}

	return success;
}
