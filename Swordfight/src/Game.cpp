#include "Game.h"
#include <iostream>
#include "kecs/KECS.h"
#include "rendering/KRenderer.h"
#include "Components/c_transform.h"
#include "Components/c_boxcollider.h"
#include "Tags.h"
#include "Components/c_input.h"
#include "Components/c_physics.h"
#include "KTime.h"
#include "systems/RenderSystem.h"
#include "ResourceManager.h"
#include "Components/c_render.h"
#include "systems/InputSystem.h"
#include "systems/CollisionSystem.h"
#include "systems/PhysicsSystem.h"
#include "systems/FighterAnimationSystem.h"
#include "systems/FighterStateSystem.h"
#include "Factories/FighterFactory.h"
#include "Components/c_fighter.h"
#include "Components/c_horizontalcollider.h"
#include "systems/BulletSystem.h"
#include "InputManager.h"
#include "systems/AnimationSystem.h"
#include <chrono>
#include <thread>

Game::~Game()
{
	delete inputSystem;
	delete renderSystem;
	delete collisionSystem;
	delete physicsSystem;
	delete animationSystem;
	delete fighterAnimationSystem;
	delete fighterStateSystem;
	delete kRenderer;
}


void Game::Run(){

	EntityManager::SetUpComponents<Transform, BoxCollider, UserInput, Render, Physics, Fighter, HorizontalCollider>();
	EntityManager::SetUpTags<Player1, Player2>();

	Entity player1 = FighterFactory::ConstructFighter(300, 300, true);
	//Entity player1 = FighterFactory::ConstructFighter(500, 300, true);

	Entity player2 = FighterFactory::ConstructFighter(500, 300, false);
	//Entity player2 = FighterFactory::ConstructFighter(300, 300, false);

	std::cout<<"Starting game"<<std::endl;
	while (!quit)
	{
		KTime::CalculateDeltaTime();
		InputManager::GetInstance().UpdateInput();
		quit = InputManager::GetInstance().ShouldQuit();
		inputSystem->GetUserInput();
		fighterStateSystem->UpdateFighterState();
		fighterAnimationSystem->ResolveAnimations();
		physicsSystem->ApplyPhysics();
		collisionSystem->CheckCollisions();
		physicsSystem->HandleCollisions();
		animationSystem->AdvanceAnimations();
		renderSystem->Draw(*kRenderer, true);
		//std::this_thread::sleep_for(std::chrono::milliseconds(100));
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
