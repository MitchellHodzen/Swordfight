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
	commandMapper.MapInputToCommand(InputManager::KeyboardKey::KEY_S, InputCommandMapper::Command::StanceDown);
	commandMapper.MapInputToCommand(InputManager::KeyboardKey::KEY_W, InputCommandMapper::Command::StanceUp);
	commandMapper.MapInputToCommand(InputManager::KeyboardKey::KEY_A, InputCommandMapper::Command::MoveLeft);
	commandMapper.MapInputToCommand(InputManager::KeyboardKey::KEY_D, InputCommandMapper::Command::MoveRight);
	commandMapper.MapInputToCommand(InputManager::KeyboardKey::KEY_SPACE, InputCommandMapper::Command::Attack);
	commandMapper.MapInputToCommand(InputManager::KeyboardKey::KEY_SHIFT, InputCommandMapper::Command::Dash);

	commandMapper.MapInputToCommand(InputManager::KeyboardKey::KEY_DOWN, InputCommandMapper::Command::StanceDownP2);
	commandMapper.MapInputToCommand(InputManager::KeyboardKey::KEY_UP, InputCommandMapper::Command::StanceUpP2);
	commandMapper.MapInputToCommand(InputManager::KeyboardKey::KEY_LEFT, InputCommandMapper::Command::MoveLeftP2);
	commandMapper.MapInputToCommand(InputManager::KeyboardKey::KEY_RIGHT, InputCommandMapper::Command::MoveRightP2);
	commandMapper.MapInputToCommand(InputManager::KeyboardKey::KEY_CTRL_R, InputCommandMapper::Command::AttackP2);
	commandMapper.MapInputToCommand(InputManager::KeyboardKey::KEY_NUM_0, InputCommandMapper::Command::DashP2);
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

		//Decide what commands to use
		InputCommandMapper::Command attackCommand;
		InputCommandMapper::Command moveLeftCommand;
		InputCommandMapper::Command moveRightCommand;
		InputCommandMapper::Command dashCommand;
		InputCommandMapper::Command stanceDownCommand;
		InputCommandMapper::Command stanceUpCommand;

		if (uin->isPlayerOne)
		{
			//If it's player 1, assign player one commands
			attackCommand = InputCommandMapper::Command::Attack;
			moveLeftCommand = InputCommandMapper::Command::MoveLeft;
			moveRightCommand = InputCommandMapper::Command::MoveRight;
			dashCommand = InputCommandMapper::Command::Dash;
			stanceDownCommand = InputCommandMapper::Command::StanceDown;
			stanceUpCommand = InputCommandMapper::Command::StanceUp;
		}
		else
		{
			//Otherwise, assign player 2 commands
			attackCommand = InputCommandMapper::Command::AttackP2;
			moveLeftCommand = InputCommandMapper::Command::MoveLeftP2;
			moveRightCommand = InputCommandMapper::Command::MoveRightP2;
			dashCommand = InputCommandMapper::Command::DashP2;
			stanceDownCommand = InputCommandMapper::Command::StanceDownP2;
			stanceUpCommand = InputCommandMapper::Command::StanceUpP2;
		}
		

		uin->keyStates[InputCommandMapper::Command::Attack] = commandMapper.GetCommandDown(attackCommand);
		uin->keyStates[InputCommandMapper::Command::MoveLeft] = commandMapper.GetCommandDown(moveLeftCommand);
		uin->keyStates[InputCommandMapper::Command::MoveRight] = commandMapper.GetCommandDown(moveRightCommand);
		uin->keyStates[InputCommandMapper::Command::Dash] = commandMapper.GetCommandDown(dashCommand);
		uin->keyStates[InputCommandMapper::Command::StanceDown] = commandMapper.GetCommandDown(stanceDownCommand);
		uin->keyStates[InputCommandMapper::Command::StanceUp] = commandMapper.GetCommandDown(stanceUpCommand);

		uin->keyPressed[InputCommandMapper::Command::Attack] = commandMapper.GetCommandPressed(attackCommand);
		uin->keyPressed[InputCommandMapper::Command::MoveLeft] = commandMapper.GetCommandPressed(moveLeftCommand);
		uin->keyPressed[InputCommandMapper::Command::MoveRight] = commandMapper.GetCommandPressed(moveRightCommand);
		uin->keyPressed[InputCommandMapper::Command::Dash] = commandMapper.GetCommandPressed(dashCommand);
		uin->keyPressed[InputCommandMapper::Command::StanceDown] = commandMapper.GetCommandPressed(stanceDownCommand);
		uin->keyPressed[InputCommandMapper::Command::StanceUp] = commandMapper.GetCommandPressed(stanceUpCommand);

		uin->keyReleased[InputCommandMapper::Command::Attack] = commandMapper.GetCommandReleased(attackCommand);
		uin->keyReleased[InputCommandMapper::Command::MoveLeft] = commandMapper.GetCommandReleased(moveLeftCommand);
		uin->keyReleased[InputCommandMapper::Command::MoveRight] = commandMapper.GetCommandReleased(moveRightCommand);
		uin->keyReleased[InputCommandMapper::Command::Dash] = commandMapper.GetCommandReleased(dashCommand);
		uin->keyReleased[InputCommandMapper::Command::StanceDown] = commandMapper.GetCommandReleased(stanceDownCommand);
		uin->keyReleased[InputCommandMapper::Command::StanceUp] = commandMapper.GetCommandReleased(stanceUpCommand);
	}
}