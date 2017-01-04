/*=====================================================================================
$File: Win32_Input.cpp
$Date: May 8, 2016
$Creator: Jamie Cooper
$Notice: (C) Copyright 2015 by Punch Drunk Squirrel Games LLC. All Rights Reserved.
=====================================================================================*/
#include "Win32_Input.h"

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
	m_keyBindings.insert(pair<GameActions, Keyboard::Keys>(ActionUp, Keyboard::Keys::Up));
	m_keyBindings.insert(pair<GameActions, Keyboard::Keys>(ActionDown, Keyboard::Keys::Down));
	m_keyBindings.insert(pair<GameActions, Keyboard::Keys>(ActionLeft, Keyboard::Keys::Left));
	m_keyBindings.insert(pair<GameActions, Keyboard::Keys>(ActionRight, Keyboard::Keys::Right));
	m_keyBindings.insert(pair<GameActions, Keyboard::Keys>(ActionAccept, Keyboard::Keys::Enter));
	m_keyBindings.insert(pair<GameActions, Keyboard::Keys>(ActionCancel, Keyboard::Keys::LeftShift));
	m_keyBindings.insert(pair<GameActions, Keyboard::Keys>(ActionAttack, Keyboard::Keys::F));
	m_keyBindings.insert(pair<GameActions, Keyboard::Keys>(ActionDefense, Keyboard::Keys::G));
	m_keyBindings.insert(pair<GameActions, Keyboard::Keys>(SystemTest, Keyboard::Keys::RightShift));
	m_keyBindings.insert(pair<GameActions, Keyboard::Keys>(SystemExitEarly, Keyboard::Keys::Escape));
	m_keyBindings.insert(pair<GameActions, Keyboard::Keys>(SystemConsole, Keyboard::Keys::OemTilde));
	
	m_gpBindings.insert(pair<GameActions, XBOXOneGamePad>(ActionUp, XBOXOneUp));
	m_gpBindings.insert(pair<GameActions, XBOXOneGamePad>(ActionDown, XBOXOneDown));
	m_gpBindings.insert(pair<GameActions, XBOXOneGamePad>(ActionLeft, XBOXOneLeft));
	m_gpBindings.insert(pair<GameActions, XBOXOneGamePad>(ActionRight, XBOXOneRight));
	m_gpBindings.insert(pair<GameActions, XBOXOneGamePad>(ActionAccept, XBOXOneStart));
	m_gpBindings.insert(pair<GameActions, XBOXOneGamePad>(ActionCancel, XBOXOneBack));
	m_gpBindings.insert(pair<GameActions, XBOXOneGamePad>(ActionAttack, XBOXOneA));
	m_gpBindings.insert(pair<GameActions, XBOXOneGamePad>(ActionDefense, XBOXOneB));
	m_gpBindings.insert(pair<GameActions, XBOXOneGamePad>(SystemTest, XBOXOneLeftBumper));
	m_gpBindings.insert(pair<GameActions, XBOXOneGamePad>(SystemExitEarly, XBOXOneRightBumper));
}

Win32Input::~Win32Input()
{
}

void Win32Input::ClearFunctionPointers()
{
	m_gameActionBindings.clear();
}

void Win32Input::ProcessCommands()
{
	ProcessKeyboard();
	ProcessGamePad();
}

void Win32Input::ChangeKeybinding(GameActions action, Keyboard::Keys key)
{
	auto actionIndex = m_keyBindings.find(action);

	if (actionIndex != m_keyBindings.end())
		actionIndex->second = key;
}

void Win32Input::AddKeyboardActionBinding(GameActions action, function<void(bool,GameActions)> funcPoint)
{
	m_gameActionBindings.insert(pair<GameActions, function<void(bool,GameActions)>>(action, funcPoint));
}

void Win32Input::ProcessKeyboard()
{
	map<GameActions, Keyboard::Keys>::iterator keyboardInputIterator;

	auto keyboardState = Keyboard::Get().GetState();
	m_keyboardTracker.Update(keyboardState);

	for (keyboardInputIterator = m_keyBindings.begin(); keyboardInputIterator != m_keyBindings.end(); keyboardInputIterator++)
	{
		auto funcPointIndex = m_gameActionBindings.find((keyboardInputIterator)->first);

		if (m_keyboardTracker.IsKeyPressed((keyboardInputIterator)->second))
		{
			if (funcPointIndex != m_gameActionBindings.end())
			{
				if (funcPointIndex != m_gameActionBindings.end())
					funcPointIndex->second(true,funcPointIndex->first);
			}
		}
		else if (m_keyboardTracker.IsKeyReleased((keyboardInputIterator)->second))
		{
			if (funcPointIndex != m_gameActionBindings.end())
			{
				funcPointIndex->second(false, funcPointIndex->first);
			}
		}
	}
}

void Win32Input::ProcessGamePad()
{
	auto state = GamePad::Get().GetState(0);

	map<GameActions, XBOXOneGamePad>::iterator gamepadInputIterator;

	if (state.IsConnected())
	{
		m_gamePadTracker.Update(state);

		for (gamepadInputIterator = m_gpBindings.begin(); gamepadInputIterator != m_gpBindings.end(); gamepadInputIterator++)
		{
			auto funcPointIndex = m_gameActionBindings.find((gamepadInputIterator)->first);

			if ((gamepadInputIterator)->second == XBOXOneUp && m_gamePadTracker.dpadUp == m_gamePadTracker.PRESSED)
				funcPointIndex->second(true, funcPointIndex->first);
			else if ((gamepadInputIterator)->second == XBOXOneUp && m_gamePadTracker.dpadUp == m_gamePadTracker.RELEASED)
				funcPointIndex->second(false, funcPointIndex->first);
			if ((gamepadInputIterator)->second == XBOXOneDown && m_gamePadTracker.dpadDown == m_gamePadTracker.PRESSED)
				funcPointIndex->second(true, funcPointIndex->first);
			else if ((gamepadInputIterator)->second == XBOXOneDown && m_gamePadTracker.dpadDown == m_gamePadTracker.RELEASED)
				funcPointIndex->second(false, funcPointIndex->first);
			if ((gamepadInputIterator)->second == XBOXOneLeft && m_gamePadTracker.dpadLeft == m_gamePadTracker.PRESSED)
				funcPointIndex->second(true, funcPointIndex->first);
			else if ((gamepadInputIterator)->second == XBOXOneLeft && m_gamePadTracker.dpadLeft == m_gamePadTracker.RELEASED)
				funcPointIndex->second(false, funcPointIndex->first);
			if ((gamepadInputIterator)->second == XBOXOneRight && m_gamePadTracker.dpadRight == m_gamePadTracker.PRESSED)
				funcPointIndex->second(true, funcPointIndex->first);
			else if ((gamepadInputIterator)->second == XBOXOneRight && m_gamePadTracker.dpadRight == m_gamePadTracker.RELEASED)
				funcPointIndex->second(false, funcPointIndex->first);
			if ((gamepadInputIterator)->second == XBOXOneBack && m_gamePadTracker.back == m_gamePadTracker.PRESSED)
				funcPointIndex->second(true, funcPointIndex->first);
			else if ((gamepadInputIterator)->second == XBOXOneBack && m_gamePadTracker.back == m_gamePadTracker.RELEASED)
				funcPointIndex->second(false, funcPointIndex->first);
			if ((gamepadInputIterator)->second == XBOXOneStart && m_gamePadTracker.start == m_gamePadTracker.PRESSED)
				funcPointIndex->second(true, funcPointIndex->first);
			else if ((gamepadInputIterator)->second == XBOXOneStart && m_gamePadTracker.start == m_gamePadTracker.RELEASED)
				funcPointIndex->second(false, funcPointIndex->first);
			if ((gamepadInputIterator)->second == XBOXOneLeftBumper && m_gamePadTracker.leftShoulder == m_gamePadTracker.PRESSED)
				funcPointIndex->second(true, funcPointIndex->first);
			else if ((gamepadInputIterator)->second == XBOXOneLeftBumper && m_gamePadTracker.leftShoulder == m_gamePadTracker.RELEASED)
				funcPointIndex->second(false, funcPointIndex->first);
			if ((gamepadInputIterator)->second == XBOXOneRightBumper && m_gamePadTracker.rightShoulder == m_gamePadTracker.PRESSED)
				funcPointIndex->second(true, funcPointIndex->first);
			else if ((gamepadInputIterator)->second == XBOXOneRightBumper && m_gamePadTracker.rightShoulder == m_gamePadTracker.RELEASED)
				funcPointIndex->second(false, funcPointIndex->first);
			if ((gamepadInputIterator)->second == XBOXOneA && m_gamePadTracker.a == m_gamePadTracker.PRESSED)
				funcPointIndex->second(true, funcPointIndex->first);
			else if ((gamepadInputIterator)->second == XBOXOneA && m_gamePadTracker.a == m_gamePadTracker.RELEASED)
				funcPointIndex->second(false, funcPointIndex->first);
			if ((gamepadInputIterator)->second == XBOXOneX && m_gamePadTracker.x == m_gamePadTracker.PRESSED)
				funcPointIndex->second(true, funcPointIndex->first);
			else if ((gamepadInputIterator)->second == XBOXOneX && m_gamePadTracker.x == m_gamePadTracker.RELEASED)
				funcPointIndex->second(false, funcPointIndex->first);
			if ((gamepadInputIterator)->second == XBOXOneY && m_gamePadTracker.y == m_gamePadTracker.PRESSED)
				funcPointIndex->second(true, funcPointIndex->first);
			else if ((gamepadInputIterator)->second == XBOXOneY && m_gamePadTracker.y == m_gamePadTracker.RELEASED)
				funcPointIndex->second(false, funcPointIndex->first);
			if ((gamepadInputIterator)->second == XBOXOneB && m_gamePadTracker.b == m_gamePadTracker.PRESSED)
				funcPointIndex->second(true, funcPointIndex->first);
			else if ((gamepadInputIterator)->second == XBOXOneB && m_gamePadTracker.b == m_gamePadTracker.RELEASED)
				funcPointIndex->second(false, funcPointIndex->first);
		}
	}
	else
	{
		m_gamePadTracker.Reset();
	}
}