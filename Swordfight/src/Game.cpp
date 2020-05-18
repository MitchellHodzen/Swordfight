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
#include "systems/FighterInputSystem.h"
#include "systems/FighterActionSystem.h"
#include "systems/FighterAnimationSystem.h"
#include "systems/FighterStateSystem.h"
#include "Factories/FighterFactory.h"
#include "Components/c_fighter.h"
#include "systems/BulletSystem.h"
#include "InputManager.h"
#include "systems/AnimationSystem.h"

Game::~Game()
{
	delete inputSystem;
	delete renderSystem;
	delete collisionSystem;
	delete physicsSystem;
	delete animationSystem;
	delete fighterInputSystem;
	delete fighterActionSystem;
	delete fighterAnimationSystem;
	delete fighterStateSystem;
	delete kRenderer;
}


void Game::Run(){

	EntityManager::SetUpComponents<Transform, Rect, UserInput, Render, Physics, Fighter>();
	EntityManager::SetUpTags<Player, Enemy, Wall>();

	Entity player1 = FighterFactory::ConstructFighter(300, 300, true);

	Entity player2 = FighterFactory::ConstructFighter(500, 300, false);

	std::cout<<"Starting game"<<std::endl;
	while (!quit)
	{
		Time::CalculateDeltaTime();
		InputManager::GetInstance().UpdateInput();
		quit = InputManager::GetInstance().ShouldQuit();
		inputSystem->GetUserInput();
		//fighterInputSystem->HandleUserInput();
		//fighterActionSystem->ResolveActions();
		fighterStateSystem->UpdateFighterState();
		fighterAnimationSystem->ResolveAnimations();
		physicsSystem->ApplyPhysics();
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
			animationSystem = new AnimationSystem();
			fighterInputSystem = new FighterInputSystem();
			fighterActionSystem = new FighterActionSystem();
			fighterAnimationSystem = new FighterAnimationSystem();
			fighterStateSystem = new FighterStateSystem();
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
