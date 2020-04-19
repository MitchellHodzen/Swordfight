#include "systems/InputSystem.h"
#include "kecs/KECS.h"
#include "Components/c_input.h"
#include "Components/c_physics.h"
#include "Components/c_cannon.h"
#include "MessageManager.h"
#include "Messages/m_bulletFired.h"
#include "InputManager.h"
#include "Time.h"

InputSystem::InputSystem()
{
}


InputSystem::~InputSystem()
{
}

void InputSystem::GetUserInput()
{
	std::vector<Entity> entities = EntityManager::GetEntitiesWithComponent<UserInput>();

	InputManager inputManager = InputManager::GetInstance();

	//For every entity which captures user input, record user input
	for (Entity entity : entities)
	{
		UserInput* uin = EntityManager::GetComponent<UserInput>(entity);

		uin->keyStates[UserInput::InputType::UP] = inputManager.GetKeyDown(InputManager::KeyboardKey::KEY_UP);
		uin->keyStates[UserInput::InputType::DOWN] = inputManager.GetKeyDown(InputManager::KeyboardKey::KEY_DOWN);
		uin->keyStates[UserInput::InputType::LEFT] = inputManager.GetKeyDown(InputManager::KeyboardKey::KEY_LEFT);
		uin->keyStates[UserInput::InputType::RIGHT] = inputManager.GetKeyDown(InputManager::KeyboardKey::KEY_RIGHT);
		uin->keyStates[UserInput::InputType::SPACE] = inputManager.GetKeyPressed(InputManager::KeyboardKey::KEY_SPACE);

	}
}