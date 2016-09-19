/*=====================================================================================
$File: InputManager.cpp
$Date: September 15, 2016
$Creator: Jamie Cooper
$Notice: (C) Copyright 2015 by Punch Drunk Squirrel Games LLC. All Rights Reserved.
=====================================================================================*/
#include "InputManager.h"

InputManager::InputManager()
{

}

InputManager::~InputManager()
{

}

InputManager::InputManager(InputManager const & copy)
{

}

void InputManager::AddKeyboardCommand(Keyboard::Keys key, function<void(bool)> funcPoint)
{
#if _WIN32
	m_input->AddKeyboardCommand(key, funcPoint);
#endif
}

void InputManager::AddGamePadDpadCommand(DpadDirections dir, function<void(bool)> funcPoint)
{
#if _WIN32
	m_input->AddGamePadDpadCommand(dir, funcPoint);
#endif
}

void InputManager::AddGamePadButtonCommand(GamePadButtons button, function<void(bool)> funcPoint)
{
#if _WIN32
	m_input->AddGamePadButtonCommand(button, funcPoint);
#endif
}

void InputManager::ProcessCommands()
{
#if _WIN32
	m_input->ProcessCommands();
#endif
}

void InputManager::ClearCommands()
{
#if _WIN32
	m_input->ClearCommands();
#endif
}
