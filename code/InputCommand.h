#pragma once
/*=====================================================================================
$File: InputCommand.h
$Date: May 8, 2016
$Creator: Jamie Cooper
$Notice: (C) Copyright 2015 by Punch Drunk Squirrel Games LLC. All Rights Reserved.
=====================================================================================*/
#ifndef WININPUT_H
#define WININPUT_H
#include "Includes.h"
#include "GameActor.h"

enum GamePadDPad
{
	Up,
	Down,
	Left,
	Right
};

enum GamePadButtons
{
	A,
	B,
	X,
	Y,
	LeftShoulder,
	RightShoulder,
	Back,
	Start
};

class Command
{
	protected:
		function<void(bool)>	m_functionPointer;
		bool					m_dpadSet;
		bool					m_buttonSet;
		GamePadButtons			m_button;
		GamePadDPad				m_dpad;
		Keyboard::Keys			m_key;

	public:
		virtual void execute(bool pressedOrReleased)
		{
			m_functionPointer(pressedOrReleased);
		}

		virtual void setCallbackFunction(function<void(bool)> funcPoint)
		{
			m_functionPointer = funcPoint;
		}

		virtual void setKeyboardKeyBinding(Keyboard::Keys key)
		{
			m_key = key;
		}

		virtual Keyboard::Keys getKeyboardBinding()
		{
			return m_key;
		}

		virtual void setGamePadDpad(GamePadDPad dpad)
		{
			m_dpad = dpad;
			m_dpadSet = true;
		}

		virtual GamePadDPad getGamePadDpad()
		{
			return m_dpad;
		}

		virtual bool hasDpadBinding()
		{
			return m_dpadSet;
		}

		virtual void setGamePadButton(GamePadButtons button)
		{
			m_button = button;
			m_buttonSet = true;
		}

		virtual GamePadButtons getGamePadButton()
		{
			return m_button;
		}

		virtual bool hasButtonBinding()
		{
			return m_buttonSet;
		}
};

class WinInput
{
	protected:
		unique_ptr<Keyboard::KeyboardStateTracker> m_keyboardTracker;
		unique_ptr<Keyboard> m_keyboard;
		unique_ptr<GamePad> m_gamePad;
		GamePad::State m_gamepadState;
		vector<Command*> m_gameCommands;

	public:
		WinInput();
		~WinInput();

		void InitializeInput();
		void AddCommand(Command *comm);
		void ClearCommands();
		void HandleInput();

		void GamePadOnSusspend()
		{
			m_gamePad->Suspend();
		}

		void GamePadOnResume()
		{
			m_gamePad->Resume();
		}
};
#endif