#pragma once
/*=====================================================================================
$File: InputManager.h
$Date: September 15, 2016
$Creator: Jamie Cooper
$Notice: (C) Copyright 2015 by Punch Drunk Squirrel Games LLC. All Rights Reserved.
=====================================================================================*/
#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H
#include "Win32_Input.h"

class InputManager
{
	protected:
		unique_ptr<Win32Input> m_input;

	public:
		InputManager();
		~InputManager();
		InputManager(InputManager const& copy);
		InputManager& operator=(InputManager const& copy);

		void AddKeyboardCommand(Keyboard::Keys key, function<void(bool)> funcPoint);
		void AddGamePadDpadCommand(DpadDirections dir, function<void(bool)> funcPoint);
		void AddGamePadButtonCommand(GamePadButtons button, function<void(bool)> funcPoint);
		void ProcessCommands();
		void ClearCommands();

		void GamePadSuspend()
		{
			m_input->GetGamePad()->Suspend();
		}

		void GamePadResume()
		{
			m_input->GetGamePad()->Resume();
		}
};
#endif