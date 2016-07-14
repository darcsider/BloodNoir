#pragma once
/*=====================================================================================
$File: Win32_Input.h
$Date: May 8, 2016
$Creator: Jamie Cooper
$Notice: (C) Copyright 2015 by Punch Drunk Squirrel Games LLC. All Rights Reserved.
=====================================================================================*/
#ifndef WININPUT_H
#define WININPUT_H
#include "Includes.h"

enum DpadDirections
{
	Up,
	Down,
	Left,
	Right
};

enum GamePadButtons
{
	Back,
	Start,
	LeftBumper,
	RightBumper,
	A,
	X,
	Y,
	B
};

class Command
{
	protected:
		function<void(bool)> m_functionPointer;
	public:
		Command() {}
		virtual ~Command() {}
		virtual void Execute() = 0;
		virtual void SetFunctionPointer(function<void(bool)> funcPoint) = 0;
};

class KeyboardCommand : public Command
{
	protected:
		Keyboard::Keys m_key;
		Keyboard::KeyboardStateTracker m_keyboardTracker;

	public:
		KeyboardCommand();
		virtual ~KeyboardCommand();
		virtual void Execute();
		virtual void SetFunctionPointer(function<void(bool)> funcPoint);
		void SetKeyboardBinding(Keyboard::Keys key);
};

class GamePadDpadCommand : public Command
{
	protected:
		DpadDirections m_direction;
		GamePad::ButtonStateTracker m_gamepadTracker;

	public:
		GamePadDpadCommand();
		virtual ~GamePadDpadCommand();
		virtual void Execute();
		virtual void SetFunctionPointer(function<void(bool)> funcPoint);
		void SetGamepadDpadBinding(DpadDirections direction);
};

class GamePadButtonCommand : public Command
{
	protected:
		GamePadButtons m_button;
		GamePad::ButtonStateTracker m_gamepadTracker;

	public:
		GamePadButtonCommand();
		virtual ~GamePadButtonCommand();
		virtual void Execute();
		virtual void SetFunctionPointer(function<void(bool)> funcPoint);
		void SetGamepadButtonBinding(GamePadButtons button);
};

class InputManager
{
	protected:
		vector<Command*> m_inputCommands;
		unique_ptr<Keyboard> m_keyboard;
		unique_ptr<GamePad> m_gamePad;

	public:
		InputManager();
		~InputManager();
		void AddCommand(Command *command);
		void AddKeyboardCommand(Keyboard::Keys key, function<void(bool)> funcPoint);
		void AddGamePadDpadCommand(DpadDirections dir, function<void(bool)> funcPoint);
		void AddGamePadButtonCommand(GamePadButtons button, function<void(bool)> funcPoint);
		void ClearCommands();
		void ProcessCommands();

		void GamePadSuspend()
		{
			m_gamePad->Suspend();
		}

		void GamePadResume()
		{
			m_gamePad->Resume();
		}
};
#endif