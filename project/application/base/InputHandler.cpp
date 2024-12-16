#include "InputHandler.h"
#include "Input.h"

ICommand* InputHandler::HandleInput()
{
	if (Input::GetInstance()->PushKey(DIK_D))
	{
		return pressKeyD_;
	}
	if (Input::GetInstance()->PushKey(DIK_A))
	{
		return pressKeyA_;
	}

	return nullptr;
}

void InputHandler::AssignMoveLeftCommand2PressKeyA()
{
	ICommand* command = new MoveLeftCommand();
	this->pressKeyA_ = command;
}

void InputHandler::AssignMoveRightCommand2PressKeyD()
{
	ICommand* command = new MoveRightCommand();
	this->pressKeyD_ = command;
}
