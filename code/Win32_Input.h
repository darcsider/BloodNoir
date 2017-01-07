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
	XBOXOneUp = 1,
	XBOXOneDown = 2,
	XBOXOneLeft = 3,
	XBOXOneRight = 4,
	XBOXDirectionMoveUp = 5,
	XBOXDirectionMoveDown = 6,
	XBOXDirectionMoveLeft = 7,
	XBOXDirectionMoveRight = 8,
	XBOXOneBack = 9,
	XBOXOneStart = 10,
	XBOXOneLeftBumper = 11,
	XBOXOneRightBumper = 12,
	XBOXOneA = 13,
	XBOXOneX = 14,
	XBOXOneY = 15,
	XBOXOneB = 16
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
	CharacterAttack = 7,
	CharacterDefense = 8,
	DirectionMoveLeft = 9,
	DirectionMoveRight = 10,
	DirectionMoveUp = 11,
	DirectionMoveDown = 12,
	SystemTest = 13,
	SystemExitEarly = 14,
	SystemConsole = 15
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
		map<GameActions, function<void(bool, GameActions)>> m_gameActionBindings;

		void BuildDefaultBindings();

	public:
		Win32Input();
		~Win32Input();
		void ClearFunctionPointers();
		void ProcessCommands();

		// testing functions
		void ChangeKeybinding(GameActions action, Keyboard::Keys key);
		void AddKeyboardActionBinding(GameActions action, function<void(bool,GameActions)> funcPoint);

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