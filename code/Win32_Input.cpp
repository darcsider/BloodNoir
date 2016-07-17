/*=====================================================================================
$File: Win32_Input.cpp
$Date: May 8, 2016
$Creator: Jamie Cooper
$Notice: (C) Copyright 2015 by Punch Drunk Squirrel Games LLC. All Rights Reserved.
=====================================================================================*/
#include "Win32_Input.h"

KeyboardCommand::KeyboardCommand()
{
}

KeyboardCommand::~KeyboardCommand()
{

}

void KeyboardCommand::Execute()
{
	auto keyboardState = Keyboard::Get().GetState();
	m_keyboardTracker.Update(keyboardState);

	if (m_keyboardTracker.IsKeyPressed(m_key))
		m_functionPointer(true);
	else if (m_keyboardTracker.IsKeyReleased(m_key))
		m_functionPointer(false);
}

void KeyboardCommand::SetFunctionPointer(function<void(bool)> funcPoint)
{
	m_functionPointer = funcPoint;
}

void KeyboardCommand::SetKeyboardBinding(Keyboard::Keys key)
{
	m_key = key;
}

MouseButtonCommand::MouseButtonCommand()
{

}

MouseButtonCommand::~MouseButtonCommand()
{

}

void MouseButtonCommand::Execute()
{
	auto mouseState = Mouse::Get().GetState();
	XMFLOAT2 mousePosition{ (float)mouseState.x, (float)mouseState.y };
	bool pointerInObject = false;

	if ((mousePosition.x >= m_collisionRect.left && mousePosition.x <= m_collisionRect.right) 
	&& (mousePosition.y >= m_collisionRect.top && mousePosition.y <= m_collisionRect.bottom))
		pointerInObject = true;
	else
		pointerInObject = false;

	m_mouseTracker.Update(mouseState);

	if ((m_button == LeftButton && m_mouseTracker.leftButton == m_mouseTracker.PRESSED) && pointerInObject)
	{
		Mouse::Get().SetMode(Mouse::MODE_RELATIVE);
		m_functionPointer(true);
	}
	else if (m_button == LeftButton && m_mouseTracker.leftButton == m_mouseTracker.RELEASED)
	{
		Mouse::Get().SetMode(Mouse::MODE_ABSOLUTE);
		m_functionPointer(false);
	}

	if ((m_button == MiddleButton && m_mouseTracker.middleButton == m_mouseTracker.PRESSED) && pointerInObject)
	{
		m_functionPointer(true);
	}
	else if (m_button == MiddleButton && m_mouseTracker.middleButton == m_mouseTracker.RELEASED)
	{
		Mouse::Get().SetMode(Mouse::MODE_ABSOLUTE);
		m_functionPointer(false);
	}

	if ((m_button == RightButton && m_mouseTracker.rightButton == m_mouseTracker.PRESSED) && pointerInObject)
	{
		m_functionPointer(true);
	}
	else if (m_button == RightButton && m_mouseTracker.rightButton == m_mouseTracker.RELEASED)
	{
		Mouse::Get().SetMode(Mouse::MODE_ABSOLUTE);
		m_functionPointer(false);
	}
}

void MouseButtonCommand::SetFunctionPointer(function<void(bool)> funcPoint)
{
	m_functionPointer = funcPoint;
}

void MouseButtonCommand::SetMouseButtonBinding(MouseButtons button)
{
	m_button = button;
}

void MouseButtonCommand::SetSelectedObjectPos(RECT objectRect)
{
	m_collisionRect = objectRect;
}

GamePadDpadCommand::GamePadDpadCommand()
{
	m_gamepadTracker = GamePad::ButtonStateTracker();
}

GamePadDpadCommand::~GamePadDpadCommand()
{

}

void GamePadDpadCommand::Execute()
{
	auto state = GamePad::Get().GetState(0);

	if (state.IsConnected())
	{
		m_gamepadTracker.Update(state);

		if (m_direction == Left && (m_gamepadTracker.dpadLeft == m_gamepadTracker.PRESSED))
			m_functionPointer(true);
		else if (m_direction == Left && (m_gamepadTracker.dpadLeft == m_gamepadTracker.RELEASED))
			m_functionPointer(false);

		if (m_direction == Right && (m_gamepadTracker.dpadRight == m_gamepadTracker.PRESSED))
			m_functionPointer(true);
		else if (m_direction == Right && (m_gamepadTracker.dpadRight == m_gamepadTracker.RELEASED))
			m_functionPointer(false);

		if (m_direction == Up && (m_gamepadTracker.dpadUp == m_gamepadTracker.PRESSED))
			m_functionPointer(true);
		else if (m_direction == Up && (m_gamepadTracker.dpadUp == m_gamepadTracker.RELEASED))
			m_functionPointer(false);

		if (m_direction == Down && (m_gamepadTracker.dpadDown == m_gamepadTracker.PRESSED))
			m_functionPointer(true);
		else if (m_direction == Down && (m_gamepadTracker.dpadDown == m_gamepadTracker.RELEASED))
			m_functionPointer(false);
	}
	else
	{
		m_gamepadTracker.Reset();
	}
}

void GamePadDpadCommand::SetFunctionPointer(function<void(bool)> funcPoint)
{
	m_functionPointer = funcPoint;
}

void GamePadDpadCommand::SetGamepadDpadBinding(DpadDirections direction)
{
	m_direction = direction;
}

GamePadButtonCommand::GamePadButtonCommand()
{
	m_gamepadTracker = GamePad::ButtonStateTracker();
}

GamePadButtonCommand::~GamePadButtonCommand()
{

}

void GamePadButtonCommand::Execute()
{
	auto state = GamePad::Get().GetState(0);

	if (state.IsConnected())
	{
		m_gamepadTracker.Update(state);

		if (m_button == Back && (m_gamepadTracker.back == m_gamepadTracker.PRESSED))
			m_functionPointer(true);
		else if (m_button == Back && (m_gamepadTracker.back == m_gamepadTracker.RELEASED))
			m_functionPointer(false);

		if (m_button == Start && (m_gamepadTracker.start == m_gamepadTracker.PRESSED))
			m_functionPointer(true);
		else if (m_button == Start && (m_gamepadTracker.start == m_gamepadTracker.RELEASED))
			m_functionPointer(false);

		if (m_button == LeftBumper && (m_gamepadTracker.leftShoulder == m_gamepadTracker.PRESSED))
			m_functionPointer(true);
		else if (m_button == LeftBumper && (m_gamepadTracker.leftShoulder == m_gamepadTracker.RELEASED))
			m_functionPointer(false);

		if (m_button == RightBumper && (m_gamepadTracker.rightShoulder == m_gamepadTracker.PRESSED))
			m_functionPointer(true);
		else if (m_button == RightBumper && (m_gamepadTracker.rightShoulder == m_gamepadTracker.RELEASED))
			m_functionPointer(false);

		if (m_button == A && (m_gamepadTracker.a == m_gamepadTracker.PRESSED))
			m_functionPointer(true);
		else if (m_button == A && (m_gamepadTracker.a == m_gamepadTracker.RELEASED))
			m_functionPointer(false);

		if (m_button == B && (m_gamepadTracker.b == m_gamepadTracker.PRESSED))
			m_functionPointer(true);
		else if (m_button == B && (m_gamepadTracker.b == m_gamepadTracker.RELEASED))
			m_functionPointer(false);

		if (m_button == X && (m_gamepadTracker.x == m_gamepadTracker.PRESSED))
			m_functionPointer(true);
		else if (m_button == X && (m_gamepadTracker.x == m_gamepadTracker.RELEASED))
			m_functionPointer(false);

		if (m_button == Y && (m_gamepadTracker.y == m_gamepadTracker.PRESSED))
			m_functionPointer(true);
		else if (m_button == Y && (m_gamepadTracker.y == m_gamepadTracker.RELEASED))
			m_functionPointer(false);
	}
	else
	{
		m_gamepadTracker.Reset();
	}
}

void GamePadButtonCommand::SetFunctionPointer(function<void(bool)> funcPoint)
{
	m_functionPointer = funcPoint;
}

void GamePadButtonCommand::SetGamepadButtonBinding(GamePadButtons button)
{
	m_button = button;
}

InputManager::InputManager()
{
	m_keyboard = make_unique<Keyboard>();
	m_gamePad = make_unique<GamePad>();
}

InputManager::~InputManager()
{
}

void InputManager::AddCommand(Command * command)
{
	m_inputCommands.push_back(command);
}

void InputManager::AddKeyboardCommand(Keyboard::Keys key, function<void(bool)> funcPoint)
{
	KeyboardCommand *keyCommand = new KeyboardCommand();
	keyCommand->SetFunctionPointer(funcPoint);
	keyCommand->SetKeyboardBinding(key);
	m_inputCommands.push_back(keyCommand);
}

void InputManager::AddMouseCommand(MouseButtons button, function<void(bool)> funcPoint)
{
	MouseButtonCommand *mouseCommand = new MouseButtonCommand();
	mouseCommand->SetFunctionPointer(funcPoint);
	mouseCommand->SetMouseButtonBinding(button);
	m_inputCommands.push_back(mouseCommand);
}

void InputManager::AddGamePadDpadCommand(DpadDirections dir, function<void(bool)> funcPoint)
{
	GamePadDpadCommand *dpadCommand = new GamePadDpadCommand();
	dpadCommand->SetFunctionPointer(funcPoint);
	dpadCommand->SetGamepadDpadBinding(dir);
	m_inputCommands.push_back(dpadCommand);
}

void InputManager::AddGamePadButtonCommand(GamePadButtons button, function<void(bool)> funcPoint)
{
	GamePadButtonCommand *buttonCommand = new GamePadButtonCommand();
	buttonCommand->SetFunctionPointer(funcPoint);
	buttonCommand->SetGamepadButtonBinding(button);
	m_inputCommands.push_back(buttonCommand);
}

void InputManager::ClearCommands()
{
	m_inputCommands.clear();
}

void InputManager::ProcessCommands()
{
	vector<Command*>::iterator commandIterator;

	for (commandIterator = m_inputCommands.begin(); commandIterator != m_inputCommands.end(); commandIterator++)
	{
		(*commandIterator)->Execute();
	}
}