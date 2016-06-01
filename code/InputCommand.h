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

enum DpadDirections
{
	DPadNone,
	Up,
	Down,
	Left,
	Right
};

enum GamePadButton
{
	ButtonNone,
	A,
	B,
	X,
	Y,
	Back,
	Start,
	LeftShoulder,
	RightShoulder
};

// a class that defines a specific Command or Action in the game this class
// encompasses a keyboard control and a game pad control each instance of the
// command class will represent a single action in this game engine the actions
// are Up, Down, Left, Right, Accept, Cancel, and Pause
class Command
{
	protected:
		function<void(bool)>	m_functionPointer;		// function pointer to the callback method
		DpadDirections			m_dpadDirection;
		bool					m_dpadDirectionSet;
		GamePadButton					m_gamePadButton;
		bool					m_buttonSet;
		Keyboard::Keys			m_key;					// keyboard key that is set to the action
														// no matter what the default is that a keyboard key is required
														// for each action

	public:
		Command() {}

		~Command() {}
		// execute method that is called when a key is pressed for the action
		// this method takes a bool on whether the action was a press or a release
		// of the specified button, it takes that value and passes it on to the callback
		// method that was set for the action, this method takes care of the actual action
		// that the command is linked to
		void execute(bool pressedOrReleased)
		{
			m_functionPointer(pressedOrReleased);
		}

		// method that simply sets the callback function associated with the command
		void setCallbackFunction(function<void(bool)> funcPoint)
		{
			m_functionPointer = funcPoint;
		}

		// method that sets the keyboard binding to which ever key is specified
		void setKeyboardKeyBinding(Keyboard::Keys key)
		{
			m_key = key;
		}

		void setGamePadDpadBinding(DpadDirections dpad)
		{
			m_dpadDirection = dpad;
			m_dpadDirectionSet = true;
		}

		DpadDirections getGamePadDpadBinding()
		{
			return m_dpadDirection;
		}

		void setGamePadButtonBinding(GamePadButton button)
		{
			m_gamePadButton = button;
			m_buttonSet = true;
		}

		GamePadButton getGamePadButtonBinding()
		{
			return m_gamePadButton;
		}

		bool getIsDPadDirectionSet()
		{
			return m_dpadDirectionSet;
		}

		bool getIsGamePadButtonSet()
		{
			return m_buttonSet;
		}

		// method that returns the key that is bound to the action for the keyboard
		Keyboard::Keys getKeyboardBinding()
		{
			return m_key;
		}
};

class InputHandler
{
	protected:
		unique_ptr<Keyboard::KeyboardStateTracker> m_keyboardTracker;
		unique_ptr<Keyboard> m_keyboard;
		unique_ptr<GamePad> m_gamePad;
		vector<Command*> m_gameCommands;

	public:
		InputHandler();
		~InputHandler();

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