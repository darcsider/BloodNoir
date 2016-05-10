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
}

void WinInput::AddKeyboardInput(Keyboard::Keys key, function<void()> functionPointer)
{
	m_keyboardMap.insert(pair<Keyboard::Keys, function<void()>>(key, functionPointer));
}

void WinInput::HandleInput()
{
	auto keyboardState = m_keyboard->GetState();
	m_keyboardTracker->Update(keyboardState);

	map<Keyboard::Keys, function<void()>>::iterator keyboardIterator;

	for (keyboardIterator = m_keyboardMap.begin(); keyboardIterator != m_keyboardMap.end(); keyboardIterator++)
	{
		if (m_keyboardTracker->IsKeyPressed(keyboardIterator->first))
		{
			keyboardIterator->second();
		}
	}
}