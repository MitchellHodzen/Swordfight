#include "Game.h"
#include <iostream>
#include "kecs/KECS.h"
#include "Components/c_transform.h"
#include "Components/c_rect.h"
#include "Tags.h"
#include "Components/c_input.h"
#include "Components/c_physics.h"
#include "Time.h"
#include "RenderSystem.h"
#include "ResourceManager.h"
#include "Components/c_sprite.h"
#include "InputSystem.h"
#include "CollisionSystem.h"
#include "PhysicsSystem.h"
#include "Factories/PlayerShipFactory.h"
#include "Factories/EnemyFactory.h"
#include "Components/c_cannon.h"
#include "BulletSystem.h"
#include "InputManager.h"

Game::Game(int screenWidth, int screenHeight)
{
	this->screenWidth = screenWidth;
	this->screenHeight = screenHeight;
	SetUp();
}


Game::~Game()
{
	delete inputSystem;
	delete renderSystem;
	delete collisionSystem;
	delete physicsSystem;
	delete bulletSystem;
}


void Game::Run(){

	EntityManager::SetUpComponents<Transform, Rect, UserInput, Sprite, Physics, Cannon>();
	EntityManager::SetUpTags<Player, Enemy, Wall>();

	Entity player1 = PlayerShipFactory::ConstructPlayerShip(500, 500);

	Entity enemy = EnemyFactory::ConstructEnemy(100, 100);

	std::cout<<"Starting game"<<std::endl;
	while (!quit)
	{
		Time::CalculateDeltaTime();
		InputManager::GetInstance().UpdateInput();
		quit = InputManager::GetInstance().ShouldQuit();
		
		inputSystem->GetUserInput();
		inputSystem->HandleUserInput();
		physicsSystem->ApplyPhysics();
		bulletSystem->FireBullets();
		collisionSystem->CheckCollisions();
		physicsSystem->HandleCollisions();
		renderSystem->Draw();
	}
}

void Game::SetUp() {
	renderSystem = new RenderSystem(screenWidth, screenHeight);
	ResourceManager::GetInstance().Initialize(renderSystem);
	inputSystem = new InputSystem();
	collisionSystem = new CollisionSystem();
	physicsSystem = new PhysicsSystem();
	bulletSystem = new BulletSystem();
}
