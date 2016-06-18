/*=====================================================================================
$File: InputCommand.cpp
$Date: May 8, 2016
$Creator: Jamie Cooper
$Notice: (C) Copyright 2015 by Punch Drunk Squirrel Games LLC. All Rights Reserved.
=====================================================================================*/
#include "InputCommand.h"

InputHandler::InputHandler()
{

}

InputHandler::~InputHandler()
{

}

void InputHandler::InitializeInput()
{
	m_keyboard = make_unique<Keyboard>();
	m_gamePad = make_unique<GamePad>();
	m_keyboardTracker.reset(new Keyboard::KeyboardStateTracker);
}

void InputHandler::AddCommand(Command *comm)
{
	m_gameCommands.push_back(comm);
}

void InputHandler::ClearCommands()
{
	m_gameCommands.clear();
}

void InputHandler::HandleInput()
{
	auto keyboardState = m_keyboard->GetState();

	m_keyboardTracker->Update(keyboardState);

	vector<Command*>::iterator commandIterator;

	for (commandIterator = m_gameCommands.begin(); commandIterator != m_gameCommands.end(); commandIterator++)
	{
		if (m_keyboardTracker->IsKeyPressed((*commandIterator)->getKeyboardBinding()))
			(*commandIterator)->execute(true);

		if (m_keyboardTracker->IsKeyReleased((*commandIterator)->getKeyboardBinding()))
			(*commandIterator)->execute(false);
	}
}