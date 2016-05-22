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


class Command
{
	protected:
		function<void()>	m_functionPointer;
		bool				m_dpadSet;
		bool				m_buttonSet;
		GamePad::Buttons	m_button;
		GamePad::DPad		m_dpad;
		Keyboard::Keys		m_key;

	public:
		virtual void execute()
		{
			m_functionPointer();
		}

		virtual void setCallbackFunction(function<void()> funcPoint)
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

		virtual void setGamePadDpad(GamePad::DPad dpad)
		{
			m_dpad = dpad;
			m_dpadSet = true;
		}

		virtual GamePad::DPad getGamePadDpad()
		{
			return m_dpad;
		}

		virtual bool hasDpadBinding()
		{
			return m_dpadSet;
		}

		virtual void setGamePadButton(GamePad::Buttons button)
		{
			m_button = button;
			m_buttonSet = true;
		}

		virtual GamePad::Buttons getGamePadButton()
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
		map<Keyboard::Keys, function<void(bool)>> m_keyboardMap;

	public:
		WinInput();
		~WinInput();

		void InitializeInput();
		void AddKeyboardInput(Keyboard::Keys key, function<void(bool)> functionPointer);
		void HandleInput();
};
#endif

/*
auto state = gamePad->GetState( 0 );

if ( state.IsConnected() )
{
if ( state.IsAPressed() )
// Do action for button A being down

if ( state.buttons.y )
// Do action for button Y being down

if ( state.IsDPadLeftPressed() )
// Do action for DPAD Left being down

if ( state.dpad.up || state.dpad.down || state.dpad.left || state.dpad.right )
// Do action based on any DPAD change

float posx = state.thumbSticks.leftX;
float posy = state.thumbSticks.leftY;
// These values are normalized to -1 to 1

float throttle = state.triggers.right;
// This value is normalized 0 -> 1

if ( state.IsLeftTriggerPressed() )
// Do action based on a left trigger pressed more than halfway

if ( state.IsViewPressed() )
// This is an alias for the Xbox 360 'Back' button
// which is called 'View' on the Xbox One controller.
}

if ( gamePad->SetVibration( 0, 0.5f, 0.25f ) )
// If true, the vibration was successfully set.
auto caps = gamePad->GetCapabilities( 0 );
if ( caps.IsConnected() )
{
if ( caps.gamepadType == GamePad::Capabilities::FLIGHT_STICK )
// Use specific controller layout based on a flight stick controller
else
// Default to treating any unknown type as a standard gamepad
}

GamePad::ButtonStateTracker tracker;

...

auto state = gamePad->GetState( 0 );
if ( state.IsConnected() )
{
tracker.Update( state );

if ( tracker.a == GamePad::ButtonStateTracker::PRESSED )
// Take an action when Button A is first pressed, but don't do it again until
// the button is released and then pressed again
}
else
{
tracker.Reset();
}
*/

/*	Just in case I find out that my changes are not going to work
class Command
{
protected:
Keyboard::Keys m_key;
function<void()> m_functionPointer;

public:
virtual void execute()
{
m_functionPointer();
}

virtual void setCallbackFunction(function<void()> funcPoint)
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

virtual void setGamePadDpad(GamePad::DPad dpad);
virtual GamePad::DPad getGamePadDpad();
virtual bool hasDpadBinding();

virtual void setGamePadButton(GamePad::Buttons button);
virtual GamePad::Buttons getGamePadButton();
virtual bool hasButtonBinding();
};

class DirectionalCommand : public Command
{
protected:
Keyboard::Keys m_key;
GamePad::DPad m_dpad;

public:
DirectionalCommand() {}
~DirectionalCommand() {}

void setGamePadDpad(GamePad::DPad dpad)
{
m_dpad = dpad;
}

GamePad::DPad getGamePadDpad()
{
return m_dpad;
}

bool hasDpadBinding()
{
return true;
}

bool hasButtonBinding()
{
return false;
}
};

class ActionCommand : public Command
{
protected:
GamePad::Buttons m_button;

public:
ActionCommand() {}
~ActionCommand() {}

void setGamePadButton(GamePad::Buttons button)
{
m_button = button;
}

GamePad::Buttons getGamePadButton()
{
return m_button;
}

bool hasDpadBinding()
{
return false;
}

bool hasButtonBinding()
{
return true;
}
};
*/