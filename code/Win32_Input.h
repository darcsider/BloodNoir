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

enum XBOXOneGamePad
{
	XBOXOneUp,
	XBOXOneDown,
	XBOXOneLeft,
	XBOXOneRight,
	XBOXOneBack,
	XBOXOneStart,
	XBOXOneLeftBumper,
	XBOXOneRightBumper,
	XBOXOneA,
	XBOXOneX,
	XBOXOneY,
	XBOXOneB
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

class Win32Input
{
	protected:
		unique_ptr<GamePad> m_gamePad;
		GamePad::ButtonStateTracker m_gamePadTracker;

		unique_ptr<Keyboard> m_keyboard;
		Keyboard::Keys m_key;
		Keyboard::KeyboardStateTracker m_keyboardTracker;

		map<GameActions, XBOXOneGamePad> m_gpBindings;

		map<GameActions, Keyboard::Keys> m_keyBindings;
		map<GameActions, function<void(bool)>> m_gameActionBindings;

		void BuildDefaultBindings();

	public:
		Win32Input();
		~Win32Input();
		void ClearFunctionPointers();
		void ProcessCommands();

		// testing functions
		void ChangeKeybinding(GameActions action, Keyboard::Keys key);
		void AddKeyboardActionBinding(GameActions action, function<void(bool)> funcPoint);

		void ProcessKeyboard();
		void ProcessGamePad();

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