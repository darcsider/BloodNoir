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
	//auto keyboardState = Keyboard::Get().GetState();
	//m_keyboardTracker.Update(keyboardState);

	//if (m_keyboardTracker.IsKeyPressed(m_key))
	//	m_functionPointer(true);
	//else if (m_keyboardTracker.IsKeyReleased(m_key))
		//m_functionPointer(false);
}

void KeyboardCommand::SetFunctionPointer(function<void(bool)> funcPoint)
{
	m_functionPointer = funcPoint;
}

void KeyboardCommand::SetKeyboardBinding(Keyboard::Keys key)
{
	m_key = key;
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

Win32Input::Win32Input()
{
	m_keyboard = make_unique<Keyboard>();
	m_gamePad = make_unique<GamePad>();

	BuildDefaultBindings();
}

void Win32Input::BuildDefaultBindings()
{
	// Not final key bindings just for testing purposes!!!
	// TODO: finalize key binding defaults
	keyBindings.insert(pair<GameActions, Keyboard::Keys>(ActionUp, Keyboard::Keys::Up));
	keyBindings.insert(pair<GameActions, Keyboard::Keys>(ActionDown, Keyboard::Keys::Down));
	keyBindings.insert(pair<GameActions, Keyboard::Keys>(ActionLeft, Keyboard::Keys::Left));
	keyBindings.insert(pair<GameActions, Keyboard::Keys>(ActionRight, Keyboard::Keys::Right));
	keyBindings.insert(pair<GameActions, Keyboard::Keys>(ActionAccept, Keyboard::Keys::Enter));
	keyBindings.insert(pair<GameActions, Keyboard::Keys>(ActionCancel, Keyboard::Keys::LeftShift));
	keyBindings.insert(pair<GameActions, Keyboard::Keys>(ActionAttack, Keyboard::Keys::F));
	keyBindings.insert(pair<GameActions, Keyboard::Keys>(ActionDefense, Keyboard::Keys::G));
	keyBindings.insert(pair<GameActions, Keyboard::Keys>(SystemTest, Keyboard::Keys::RightShift));
	keyBindings.insert(pair<GameActions, Keyboard::Keys>(SystemExitEarly, Keyboard::Keys::Escape));
	keyBindings.insert(pair<GameActions, Keyboard::Keys>(SystemConsole, Keyboard::Keys::OemTilde));
}

Win32Input::~Win32Input()
{
}

void Win32Input::AddCommand(Command * command)
{
	m_inputCommands.push_back(command);
}

void Win32Input::AddKeyboardCommand(Keyboard::Keys key, function<void(bool)> funcPoint)
{
	KeyboardCommand *keyCommand = new KeyboardCommand();
	keyCommand->SetFunctionPointer(funcPoint);
	keyCommand->SetKeyboardBinding(key);
	m_inputCommands.push_back(keyCommand);
}

void Win32Input::AddGamePadDpadCommand(DpadDirections dir, function<void(bool)> funcPoint)
{
	GamePadDpadCommand *dpadCommand = new GamePadDpadCommand();
	dpadCommand->SetFunctionPointer(funcPoint);
	dpadCommand->SetGamepadDpadBinding(dir);
	m_inputCommands.push_back(dpadCommand);
}

void Win32Input::AddGamePadButtonCommand(GamePadButtons button, function<void(bool)> funcPoint)
{
	GamePadButtonCommand *buttonCommand = new GamePadButtonCommand();
	buttonCommand->SetFunctionPointer(funcPoint);
	buttonCommand->SetGamepadButtonBinding(button);
	m_inputCommands.push_back(buttonCommand);
}

void Win32Input::ClearCommands()
{
	//m_inputCommands.clear();
	gameActionBindings.clear();
}

void Win32Input::ProcessCommands()
{
	/*vector<Command*>::iterator commandIterator;

	for (commandIterator = m_inputCommands.begin(); commandIterator != m_inputCommands.end(); commandIterator++)
	{
		(*commandIterator)->Execute();
	}*/
	map<GameActions, Keyboard::Keys>::iterator actionInputIterator;

	auto keyboardState = Keyboard::Get().GetState();
	m_keyboardTracker.Update(keyboardState);

	for (actionInputIterator = keyBindings.begin(); actionInputIterator != keyBindings.end(); actionInputIterator++)
	{
		auto funcPointIndex = gameActionBindings.find((actionInputIterator)->first);

		if (m_keyboardTracker.IsKeyPressed((actionInputIterator)->second))
		{
			if (funcPointIndex != gameActionBindings.end())
			{
				if (funcPointIndex != gameActionBindings.end())
					funcPointIndex->second(true);
			}
		}
		else if (m_keyboardTracker.IsKeyReleased((actionInputIterator)->second))
		{
			if (funcPointIndex != gameActionBindings.end())
			{
				funcPointIndex->second(false);
			}
		}
	}
}

void Win32Input::ChangeKeybinding(GameActions action, Keyboard::Keys key)
{
	auto actionIndex = keyBindings.find(action);

	if (actionIndex != keyBindings.end())
		actionIndex->second = key;
}

void Win32Input::AddKeyboardActionBinding(GameActions action, function<void(bool)> funcPoint)
{
	gameActionBindings.insert(pair<GameActions, function<void(bool)>>(action, funcPoint));
}
