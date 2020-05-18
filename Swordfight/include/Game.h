#pragma once

class InputSystem;
class RenderSystem;
class CollisionSystem;
class PhysicsSystem;
class BulletSystem;
class AnimationSystem;
class FighterInputSystem;
class FighterActionSystem;
class FighterAnimationSystem;
class FighterStateSystem;
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
	FighterInputSystem* fighterInputSystem;
	FighterActionSystem* fighterActionSystem;
	FighterAnimationSystem* fighterAnimationSystem;
	FighterStateSystem* fighterStateSystem;
	KRenderer* kRenderer;
};

