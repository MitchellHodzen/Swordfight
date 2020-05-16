#pragma once
struct Fighter;
struct UserInput;
struct Physics;


class FighterInputSystem
{
public:

	void HandleUserInput();

	void HandleBlockingStateInput(UserInput& userInput, Fighter& fighter);
	void HandleReadyingStateInput(UserInput& userInput, Fighter& fighter);
	void HandleAttackingStateInput(UserInput& userInput, Fighter& fighter);
	void HandleClashingStateInput(UserInput& userInput, Fighter& fighter);

	void HandleMovementInput(UserInput& userInput, Fighter& fighter);
	void HandleAttackDirectionInput(UserInput& userInput, Fighter& fighter);
};