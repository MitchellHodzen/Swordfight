#pragma once

class InputSystem;
class RenderSystem;
class CollisionSystem;
class PhysicsSystem;
class AnimationSystem;
class FighterAnimationSystem;
class FighterStateSystem;
class LifetimeSystem;
class EntityDestructionSystem;
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
	FighterAnimationSystem* fighterAnimationSystem;
	FighterStateSystem* fighterStateSystem;
	LifetimeSystem* lifetimeSystem;
	EntityDestructionSystem* entityDestructionSystem;
	KRenderer* kRenderer;
};

