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
		unique_ptr<Win32Input> m_input;		// windows platform specific input object

	public:
		// constructor instantiate platform specific objects
		InputManager();
		// destructor destroy platform specific objects
		~InputManager();

		// create Singleton object for InputManager
		static InputManager& GetInstance()
		{
			static InputManager* gpSingleton = NULL;
			if (gpSingleton == NULL)
			{
				gpSingleton = new InputManager();
			}
			return *gpSingleton;
		}

		// process commands in a platform specific manner
		void ProcessCommands();
		// clear all function pointers in the container
		void ClearFunctionPointers();

		// chnage what keyboard key is bound to what action
		void ChangeKeybinding(GameActions action, Keyboard::Keys key);
		// add a new keyboard action binding to the container on each platform specific system
		void AddKeyboardActionBinding(GameActions action, function<void(bool,GameActions)> funcPoint);

		// if game paused suspend the gamepad reading
		void GamePadSuspend()
		{
			m_input->GetGamePad()->Suspend();
		}

		// if the game is resumeing then resume gamepad reading
		void GamePadResume()
		{
			m_input->GetGamePad()->Resume();
		}
};
#endif