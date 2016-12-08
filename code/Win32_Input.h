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

enum MouseButtons
{
	LeftButton,
	MiddleButton,
	RightButton
};

enum GameActions
{
	ActionUp = 1,
	ActionDown = 2,
	ActionLeft = 3,
	ActionRight = 4,
	ActionAccept = 5,
	ActionCancel = 6,
	ActionAttack = 7,
	ActionDefense = 8,
	SystemTest = 9,
	SystemExitEarly = 10,
	SystemConsole = 11
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

class Win32Input
{
	protected:
		vector<Command*> m_inputCommands;
		// test code not final yet does this work!!!!>!>!>!>
		Keyboard::Keys m_key;
		Keyboard::KeyboardStateTracker m_keyboardTracker;
		map<GameActions, Keyboard::Keys> keyBindings;
		map<GameActions, function<void(bool)>> gameActionBindings;

		unique_ptr<Keyboard> m_keyboard;
		unique_ptr<GamePad> m_gamePad;

		void BuildDefaultBindings();

	public:
		Win32Input();
		~Win32Input();
		void AddCommand(Command *command);
		void AddKeyboardCommand(Keyboard::Keys key, function<void(bool)> funcPoint);
		void AddGamePadDpadCommand(DpadDirections dir, function<void(bool)> funcPoint);
		void AddGamePadButtonCommand(GamePadButtons button, function<void(bool)> funcPoint);
		void ClearCommands();
		void ProcessCommands();

		// testing functions
		void ChangeKeybinding(GameActions action, Keyboard::Keys key);
		void AddKeyboardActionBinding(GameActions action, function<void(bool)> funcPoint);

		void GamePadSuspend()
		{
			m_gamePad->Suspend();
		}

		void GamePadResume()
		{
			m_gamePad->Resume();
		}

		GamePad* GetGamePad()
		{
			return m_gamePad.get();
		}
};
#endif