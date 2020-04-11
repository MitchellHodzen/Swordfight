#include "InputSystem.h"
#include "kecs/KECS.h"
#include "Components/c_input.h"
#include "Components/c_physics.h"
#include "Components/c_cannon.h"
#include "MessageManager.h"
#include "Messages/m_bulletFired.h"
#include "InputManager.h"

InputSystem::InputSystem()
{
}


InputSystem::~InputSystem()
{
}

void InputSystem::GetUserInput()
{
	std::vector<Entity> entities = EntityManager::GetEntitiesWithComponent<UserInput>();

	const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);

	InputManager inputManager = InputManager::GetInstance();

	//For every entity which captures user input, record user input
	for (Entity entity : entities)
	{
		UserInput* uin = EntityManager::GetComponent<UserInput>(entity);

		uin->keyStates[UserInput::InputType::UP] = inputManager.GetKeyDown(InputManager::KeyboardKey::KEY_UP);//currentKeyStates[SDL_SCANCODE_UP];
		uin->keyStates[UserInput::InputType::DOWN] = inputManager.GetKeyDown(InputManager::KeyboardKey::KEY_DOWN);
		uin->keyStates[UserInput::InputType::LEFT] = inputManager.GetKeyDown(InputManager::KeyboardKey::KEY_LEFT);
		uin->keyStates[UserInput::InputType::RIGHT] = inputManager.GetKeyDown(InputManager::KeyboardKey::KEY_RIGHT);
		uin->keyStates[UserInput::InputType::SPACE] = inputManager.GetKeyPressed(InputManager::KeyboardKey::KEY_SPACE);

	}
}

void InputSystem::HandleUserInput()
{
	std::vector<Entity> entities = EntityManager::GetEntitiesWithComponent<Physics, UserInput>();

	for (Entity entity : entities)
	{
		Physics* phys = EntityManager::GetComponent<Physics>(entity);
		UserInput* uin = EntityManager::GetComponent<UserInput>(entity);
		if (uin->keyStates[UserInput::InputType::UP])
		{
			phys->velocity.SetY(-1);
		}
		else if (uin->keyStates[UserInput::InputType::DOWN])
		{
			phys->velocity.SetY(1);
		}
		else
		{
			phys->velocity.SetY(0);
		}
		if (uin->keyStates[UserInput::InputType::LEFT])
		{
			phys->velocity.SetX(-1);
		}
		else if (uin->keyStates[UserInput::InputType::RIGHT])
		{
			phys->velocity.SetX(1);
		}
		else
		{
			phys->velocity.SetX(0);
		}

		phys->velocity *= phys->maxSpeed;

		if (uin->keyStates[UserInput::InputType::SPACE] && EntityManager::HasComponent<Cannon>(entity))
		{
			BulletFiredMessage message(entity);
			MessageManager::PushMessage<BulletFiredMessage>(message);
		}
	}
}