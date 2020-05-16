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
		uin->keyStates[UserInput::InputType::SPACE] = inputManager.GetKeyDown(InputManager::KeyboardKey::KEY_SPACE);
		uin->keyStates[UserInput::InputType::SHIFT] = inputManager.GetKeyDown(InputManager::KeyboardKey::KEY_SHIFT);

		//wipe keys pressed
		uin->keyPressed[UserInput::InputType::UP] = inputManager.GetKeyPressed(InputManager::KeyboardKey::KEY_UP);
		uin->keyPressed[UserInput::InputType::DOWN] = inputManager.GetKeyPressed(InputManager::KeyboardKey::KEY_DOWN);
		uin->keyPressed[UserInput::InputType::LEFT] = inputManager.GetKeyPressed(InputManager::KeyboardKey::KEY_LEFT);
		uin->keyPressed[UserInput::InputType::RIGHT] = inputManager.GetKeyPressed(InputManager::KeyboardKey::KEY_RIGHT);
		uin->keyPressed[UserInput::InputType::SPACE] = inputManager.GetKeyPressed(InputManager::KeyboardKey::KEY_SPACE);
		uin->keyPressed[UserInput::InputType::SHIFT] = inputManager.GetKeyPressed(InputManager::KeyboardKey::KEY_SHIFT);

		//wipe keys released
		uin->keyReleased[UserInput::InputType::UP] = inputManager.GetKeyReleased(InputManager::KeyboardKey::KEY_UP);
		uin->keyReleased[UserInput::InputType::DOWN] = inputManager.GetKeyReleased(InputManager::KeyboardKey::KEY_DOWN);
		uin->keyReleased[UserInput::InputType::LEFT] = inputManager.GetKeyReleased(InputManager::KeyboardKey::KEY_LEFT);
		uin->keyReleased[UserInput::InputType::RIGHT] = inputManager.GetKeyReleased(InputManager::KeyboardKey::KEY_RIGHT);
		uin->keyReleased[UserInput::InputType::SPACE] = inputManager.GetKeyReleased(InputManager::KeyboardKey::KEY_SPACE);
		uin->keyReleased[UserInput::InputType::SHIFT] = inputManager.GetKeyReleased(InputManager::KeyboardKey::KEY_SHIFT);
	}
}