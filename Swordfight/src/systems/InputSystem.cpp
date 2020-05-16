#include "systems/InputSystem.h"
#include "kecs/KECS.h"
#include "Components/c_input.h"
#include "Components/c_physics.h"
#include "Components/c_cannon.h"
#include "MessageManager.h"
#include "Messages/m_bulletFired.h"
#include "InputManager.h"
#include "Time.h"
#include "InputCommandMapper.h"

InputSystem::InputSystem()
{
	InputCommandMapper commandMapper = InputCommandMapper::GetInstance();
	commandMapper.MapInputToCommand(InputManager::KeyboardKey::KEY_DOWN, InputCommandMapper::Command::StanceDown);
	commandMapper.MapInputToCommand(InputManager::KeyboardKey::KEY_UP, InputCommandMapper::Command::StanceUp);
	commandMapper.MapInputToCommand(InputManager::KeyboardKey::KEY_LEFT, InputCommandMapper::Command::MoveLeft);
	commandMapper.MapInputToCommand(InputManager::KeyboardKey::KEY_RIGHT, InputCommandMapper::Command::MoveRight);
	commandMapper.MapInputToCommand(InputManager::KeyboardKey::KEY_SPACE, InputCommandMapper::Command::Attack);
	commandMapper.MapInputToCommand(InputManager::KeyboardKey::KEY_SHIFT, InputCommandMapper::Command::Dash);
}


InputSystem::~InputSystem()
{
}

void InputSystem::GetUserInput()
{
	std::vector<Entity> entities = EntityManager::GetEntitiesWithComponent<UserInput>();

	InputCommandMapper commandMapper = InputCommandMapper::GetInstance();

	//For every entity which captures user input, record user input
	for (Entity entity : entities)
	{
		UserInput* uin = EntityManager::GetComponent<UserInput>(entity);

		uin->keyStates[InputCommandMapper::Command::Attack] = commandMapper.GetCommandDown(InputCommandMapper::Command::Attack);
		uin->keyStates[InputCommandMapper::Command::MoveLeft] = commandMapper.GetCommandDown(InputCommandMapper::Command::MoveLeft);
		uin->keyStates[InputCommandMapper::Command::MoveRight] = commandMapper.GetCommandDown(InputCommandMapper::Command::MoveRight);
		uin->keyStates[InputCommandMapper::Command::Dash] = commandMapper.GetCommandDown(InputCommandMapper::Command::Dash);
		uin->keyStates[InputCommandMapper::Command::StanceDown] = commandMapper.GetCommandDown(InputCommandMapper::Command::StanceDown);
		uin->keyStates[InputCommandMapper::Command::StanceUp] = commandMapper.GetCommandDown(InputCommandMapper::Command::StanceUp);

		uin->keyPressed[InputCommandMapper::Command::Attack] = commandMapper.GetCommandPressed(InputCommandMapper::Command::Attack);
		uin->keyPressed[InputCommandMapper::Command::MoveLeft] = commandMapper.GetCommandPressed(InputCommandMapper::Command::MoveLeft);
		uin->keyPressed[InputCommandMapper::Command::MoveRight] = commandMapper.GetCommandPressed(InputCommandMapper::Command::MoveRight);
		uin->keyPressed[InputCommandMapper::Command::Dash] = commandMapper.GetCommandPressed(InputCommandMapper::Command::Dash);
		uin->keyPressed[InputCommandMapper::Command::StanceDown] = commandMapper.GetCommandPressed(InputCommandMapper::Command::StanceDown);
		uin->keyPressed[InputCommandMapper::Command::StanceUp] = commandMapper.GetCommandPressed(InputCommandMapper::Command::StanceUp);

		uin->keyReleased[InputCommandMapper::Command::Attack] = commandMapper.GetCommandReleased(InputCommandMapper::Command::Attack);
		uin->keyReleased[InputCommandMapper::Command::MoveLeft] = commandMapper.GetCommandReleased(InputCommandMapper::Command::MoveLeft);
		uin->keyReleased[InputCommandMapper::Command::MoveRight] = commandMapper.GetCommandReleased(InputCommandMapper::Command::MoveRight);
		uin->keyReleased[InputCommandMapper::Command::Dash] = commandMapper.GetCommandReleased(InputCommandMapper::Command::Dash);
		uin->keyReleased[InputCommandMapper::Command::StanceDown] = commandMapper.GetCommandReleased(InputCommandMapper::Command::StanceDown);
		uin->keyReleased[InputCommandMapper::Command::StanceUp] = commandMapper.GetCommandReleased(InputCommandMapper::Command::StanceUp);
	}
}