#pragma once

class InputSystem;
class RenderSystem;
class CollisionSystem;
class PhysicsSystem;
class BulletSystem;
class AnimationSystem;
class FighterSystem;
class KRenderer;

class Game
{
public:
	bool SetUp(int screenWidth, int screenHeight);
	void Run();
	~Game();
private:

	int screenWidth;
	int screenHeight;
	bool quit = false;

	InputSystem* inputSystem;
	RenderSystem* renderSystem;
	CollisionSystem* collisionSystem;
	PhysicsSystem* physicsSystem;
	AnimationSystem* animationSystem;
	FighterSystem* fighterSystem;

	KRenderer* kRenderer;
};

