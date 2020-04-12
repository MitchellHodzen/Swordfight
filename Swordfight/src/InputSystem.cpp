#include "InputSystem.h"
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

void InputSystem::HandleUserInput()
{
	std::vector<Entity> entities = EntityManager::GetEntitiesWithComponent<Physics, UserInput>();

	for (Entity entity : entities)
	{
		Physics* phys = EntityManager::GetComponent<Physics>(entity);
		UserInput* uin = EntityManager::GetComponent<UserInput>(entity);

		//Movement is left and right
		if (uin->keyStates[UserInput::InputType::LEFT])
		{
			float currentVelocityX = phys->velocity.GetX();
			float newVelocityX = currentVelocityX - phys->acceleration * Time::GetDeltaTime();
			if (phys->velocity.GetX() > 0){
				//If we're moving right, give left movement a boost
				newVelocityX -= phys->friction * Time::GetDeltaTime();
			}
			phys->velocity.SetX(newVelocityX);
		}
		else if (uin->keyStates[UserInput::InputType::RIGHT])
		{
			float currentVelocityX = phys->velocity.GetX();
			float newVelocityX = currentVelocityX + phys->acceleration * Time::GetDeltaTime();
			if (phys->velocity.GetX() < 0){
				//If we're moving right, give left movement a boost
				newVelocityX += phys->friction * Time::GetDeltaTime();
			}
			phys->velocity.SetX(newVelocityX);
		}
		else{
			//If no input is selected, we apply friction
			if (phys->velocity.GetX() > 0){
				//If we're moving right, apply friction to the left
				phys->velocity.SetX(phys->velocity.GetX() - phys->friction * Time::GetDeltaTime());

				//If after applying friction the velocity direction changed, zero the velocity out
				if (phys->velocity.GetX() < 0){
					phys->velocity.SetX(0);
				}

			}
			else if (phys->velocity.GetX() < 0){
				//If we're moving left, apply friction to the right
				phys->velocity.SetX(phys->velocity.GetX() + phys->friction * Time::GetDeltaTime());

				//If after applying friction the velocity direction changed, zero the velocity out
				if (phys->velocity.GetX() > 0){
					phys->velocity.SetX(0);
				}
			}
		}

		//Clamping speed to max speed
		if (phys->velocity.GetMagnitude() > phys->maxSpeed)
		{
			phys->velocity.SetMagnitude(phys->maxSpeed);
		}


		if (uin->keyStates[UserInput::InputType::SPACE] && EntityManager::HasComponent<Cannon>(entity))
		{
			BulletFiredMessage message(entity);
			MessageManager::PushMessage<BulletFiredMessage>(message);
		}
	}
}