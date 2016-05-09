/*=====================================================================================
$File: WinInput.cpp
$Date: May 8, 2016
$Creator: Jamie Cooper
$Notice: (C) Copyright 2015 by Punch Drunk Squirrel Games LLC. All Rights Reserved.
=====================================================================================*/
#include "WinInput.h"

WinInput::WinInput()
{

}

WinInput::~WinInput()
{

}

void WinInput::InitializeInput()
{
	m_keyboard = make_unique<Keyboard>();
	m_keyboardTracker.reset(new Keyboard::KeyboardStateTracker);
	m_mouse = make_unique<Mouse>();
	m_gamePad = make_unique<GamePad>();
}

void WinInput::AddKeyboardInput(Keyboard::Keys key, function<void()> functionPointer)
{
	m_keyboardMap.insert(pair<Keyboard::Keys, function<void()>>(key, functionPointer));
}

/*void WinInput::AddGamePadInput(GamePad::Buttons button, function<void()> functionPointer)
{
	m_gamePadMap.insert(pair<GamePad::Buttons, function<void()>>(button, functionPointer));
}

void WinInput::AddGamePadDPadInput(GamePad::DPad button, function<void()> functionPointer)
{
	m_gamePadDPadMap.insert(pair<GamePad::DPad, function<void()>>(button, functionPointer));
}*/

void WinInput::HandleInput()
{
	auto keyboardState = m_keyboard->GetState();
	m_keyboardTracker->Update(keyboardState);
	auto mouseState = m_mouse->GetState();
	auto gamePadState = m_gamePad->GetState(0);

	map<Keyboard::Keys, function<void()>>::iterator keyboardIterator;
	map<GamePad::Buttons, function<void()>>::iterator gamePadIterator;
	map<GamePad::DPad, function<void()>>::iterator dPadIterator;

	for (keyboardIterator = m_keyboardMap.begin(); keyboardIterator != m_keyboardMap.end(); keyboardIterator++)
	{
		if (m_keyboardTracker->IsKeyPressed(keyboardIterator->first))
		{
			keyboardIterator->second();
		}
	}

	//for (gamePadIterator = m_gamePadMap.begin(); gamePadIterator != m_gamePadMap.end(); gamePadIterator++)
	//{
		//if ()
	//}

	//for (dPadIterator = m_gamePadDPadMap.begin(); dPadIterator != m_gamePadDPadMap.end(); dPadIterator++)
	//{

	//}
}