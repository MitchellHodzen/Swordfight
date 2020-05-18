#include "systems/InputSystem.h"
#include "kecs/KECS.h"
#include "Components/c_input.h"
#include "Components/c_physics.h"
#include "Components/c_cannon.h"
#include "MessageManager.h"
#include "Messages/m_bulletFired.h"
#include "InputManager.h"
#include "KTime.h"
#include "InputCommandMapper.h"

InputSystem::InputSystem()
{
	InputCommandMapper::GetInstance().MapInputToCommand(InputManager::KeyboardKey::KEY_S, InputCommandMapper::Command::StanceDown);
	InputCommandMapper::GetInstance().MapInputToCommand(InputManager::KeyboardKey::KEY_W, InputCommandMapper::Command::StanceUp);
	InputCommandMapper::GetInstance().MapInputToCommand(InputManager::KeyboardKey::KEY_A, InputCommandMapper::Command::MoveLeft);
	InputCommandMapper::GetInstance().MapInputToCommand(InputManager::KeyboardKey::KEY_D, InputCommandMapper::Command::MoveRight);
	InputCommandMapper::GetInstance().MapInputToCommand(InputManager::KeyboardKey::KEY_SPACE, InputCommandMapper::Command::Attack);
	InputCommandMapper::GetInstance().MapInputToCommand(InputManager::KeyboardKey::KEY_SHIFT, InputCommandMapper::Command::Dash);

	InputCommandMapper::GetInstance().MapInputToCommand(InputManager::KeyboardKey::KEY_DOWN, InputCommandMapper::Command::StanceDownP2);
	InputCommandMapper::GetInstance().MapInputToCommand(InputManager::KeyboardKey::KEY_UP, InputCommandMapper::Command::StanceUpP2);
	InputCommandMapper::GetInstance().MapInputToCommand(InputManager::KeyboardKey::KEY_LEFT, InputCommandMapper::Command::MoveLeftP2);
	InputCommandMapper::GetInstance().MapInputToCommand(InputManager::KeyboardKey::KEY_RIGHT, InputCommandMapper::Command::MoveRightP2);
	InputCommandMapper::GetInstance().MapInputToCommand(InputManager::KeyboardKey::KEY_CTRL_R, InputCommandMapper::Command::AttackP2);
	InputCommandMapper::GetInstance().MapInputToCommand(InputManager::KeyboardKey::KEY_NUM_0, InputCommandMapper::Command::DashP2);
}


InputSystem::~InputSystem()
{
}

void InputSystem::GetUserInput()
{
	std::vector<Entity> entities = EntityManager::GetEntitiesWithComponent<UserInput>();

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
		

		uin->keyStates[InputCommandMapper::Command::Attack] = InputCommandMapper::GetInstance().GetCommandDown(attackCommand);
		uin->keyStates[InputCommandMapper::Command::MoveLeft] = InputCommandMapper::GetInstance().GetCommandDown(moveLeftCommand);
		uin->keyStates[InputCommandMapper::Command::MoveRight] = InputCommandMapper::GetInstance().GetCommandDown(moveRightCommand);
		uin->keyStates[InputCommandMapper::Command::Dash] = InputCommandMapper::GetInstance().GetCommandDown(dashCommand);
		uin->keyStates[InputCommandMapper::Command::StanceDown] = InputCommandMapper::GetInstance().GetCommandDown(stanceDownCommand);
		uin->keyStates[InputCommandMapper::Command::StanceUp] = InputCommandMapper::GetInstance().GetCommandDown(stanceUpCommand);

		uin->keyPressed[InputCommandMapper::Command::Attack] = InputCommandMapper::GetInstance().GetCommandPressed(attackCommand);
		uin->keyPressed[InputCommandMapper::Command::MoveLeft] = InputCommandMapper::GetInstance().GetCommandPressed(moveLeftCommand);
		uin->keyPressed[InputCommandMapper::Command::MoveRight] = InputCommandMapper::GetInstance().GetCommandPressed(moveRightCommand);
		uin->keyPressed[InputCommandMapper::Command::Dash] = InputCommandMapper::GetInstance().GetCommandPressed(dashCommand);
		uin->keyPressed[InputCommandMapper::Command::StanceDown] = InputCommandMapper::GetInstance().GetCommandPressed(stanceDownCommand);
		uin->keyPressed[InputCommandMapper::Command::StanceUp] = InputCommandMapper::GetInstance().GetCommandPressed(stanceUpCommand);

		uin->keyReleased[InputCommandMapper::Command::Attack] = InputCommandMapper::GetInstance().GetCommandReleased(attackCommand);
		uin->keyReleased[InputCommandMapper::Command::MoveLeft] = InputCommandMapper::GetInstance().GetCommandReleased(moveLeftCommand);
		uin->keyReleased[InputCommandMapper::Command::MoveRight] = InputCommandMapper::GetInstance().GetCommandReleased(moveRightCommand);
		uin->keyReleased[InputCommandMapper::Command::Dash] = InputCommandMapper::GetInstance().GetCommandReleased(dashCommand);
		uin->keyReleased[InputCommandMapper::Command::StanceDown] = InputCommandMapper::GetInstance().GetCommandReleased(stanceDownCommand);
		uin->keyReleased[InputCommandMapper::Command::StanceUp] = InputCommandMapper::GetInstance().GetCommandReleased(stanceUpCommand);
	}
}