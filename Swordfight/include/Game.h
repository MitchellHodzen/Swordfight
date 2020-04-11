#pragma once

class InputSystem;
class RenderSystem;
class CollisionSystem;
class PhysicsSystem;
class BulletSystem;

class Game
{
public:
	Game(int screenWidth, int screenHeight);
	void Run();
	~Game();
private:
	void SetUp();

	int screenWidth;
	int screenHeight;
	bool quit = false;

	InputSystem* inputSystem;
	RenderSystem* renderSystem;
	CollisionSystem* collisionSystem;
	PhysicsSystem* physicsSystem;
	BulletSystem* bulletSystem;
};

