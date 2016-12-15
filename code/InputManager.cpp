/*=====================================================================================
$File: InputManager.cpp
$Date: September 15, 2016
$Creator: Jamie Cooper
$Notice: (C) Copyright 2015 by Punch Drunk Squirrel Games LLC. All Rights Reserved.
=====================================================================================*/
#include "InputManager.h"

InputManager::InputManager()
{
#if _WIN32
	m_input = make_unique<Win32Input>();
#endif
}

InputManager::~InputManager()
{

}

InputManager::InputManager(InputManager const & copy)
{

}

void InputManager::ProcessCommands()
{
#if _WIN32
	m_input->ProcessCommands();
#endif
}

void InputManager::ClearFunctionPointers()
{
#if _WIN32
	m_input->ClearFunctionPointers();
#endif
}

void InputManager::ChangeKeybinding(GameActions action, Keyboard::Keys key)
{
#if _WIN32
	m_input->ChangeKeybinding(action, key);
#endif
}

void InputManager::AddKeyboardActionBinding(GameActions action, function<void(bool)> funcPoint)
{
#if _WIN32
	m_input->AddKeyboardActionBinding(action, funcPoint);
#endif
}