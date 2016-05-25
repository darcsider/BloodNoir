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
	auto gamepadState = m_gamePad->GetState(0);
	static bool keyboardEntry = false;
	GamePad::ButtonStateTracker tracker;

	m_keyboardTracker->Update(keyboardState);

	if (gamepadState.IsConnected())
		tracker.Update(gamepadState);

	vector<Command*>::iterator commandIterator;

	for (commandIterator = m_gameCommands.begin(); commandIterator != m_gameCommands.end(); commandIterator++)
	{
		if (m_keyboardTracker->IsKeyPressed((*commandIterator)->getKeyboardBinding()))
		{
			keyboardEntry = true;
			(*commandIterator)->execute(true);
		}

		if (m_keyboardTracker->IsKeyReleased((*commandIterator)->getKeyboardBinding()))
		{
			keyboardEntry = false;
			(*commandIterator)->execute(false);
		}

		if (!keyboardEntry)
		{
			if ((*commandIterator)->getIsDPadDirectionSet())
			{
				if (tracker.dpadUp == GamePad::ButtonStateTracker::PRESSED && ((*commandIterator)->getGamePadDpadBinding() == Up))
					(*commandIterator)->execute(true);

				else if (tracker.dpadUp == GamePad::ButtonStateTracker::UP && ((*commandIterator)->getGamePadDpadBinding() == Up))
					(*commandIterator)->execute(false);

				if (tracker.dpadDown == GamePad::ButtonStateTracker::PRESSED && ((*commandIterator)->getGamePadDpadBinding() == Down))
					(*commandIterator)->execute(true);

				else if (tracker.dpadDown == GamePad::ButtonStateTracker::UP && ((*commandIterator)->getGamePadDpadBinding() == Down))
					(*commandIterator)->execute(false);

				if (tracker.dpadLeft == GamePad::ButtonStateTracker::PRESSED && ((*commandIterator)->getGamePadDpadBinding() == Left))
					(*commandIterator)->execute(true);

				else if (tracker.dpadLeft == GamePad::ButtonStateTracker::UP && ((*commandIterator)->getGamePadDpadBinding() == Left))
					(*commandIterator)->execute(false);

				if (tracker.dpadRight == GamePad::ButtonStateTracker::PRESSED && ((*commandIterator)->getGamePadDpadBinding() == Right))
					(*commandIterator)->execute(true);

				else if (tracker.dpadRight == GamePad::ButtonStateTracker::UP && ((*commandIterator)->getGamePadDpadBinding() == Right))
					(*commandIterator)->execute(false);
			}


			if ((*commandIterator)->getIsGamePadButtonSet())
			{
				if (tracker.a == GamePad::ButtonStateTracker::PRESSED && ((*commandIterator)->getGamePadButtonBinding() == A))
					(*commandIterator)->execute(true);

				if (tracker.a == GamePad::ButtonStateTracker::RELEASED && ((*commandIterator)->getGamePadButtonBinding() == A))
					(*commandIterator)->execute(false);

				if (tracker.b == GamePad::ButtonStateTracker::PRESSED && ((*commandIterator)->getGamePadButtonBinding() == B))
					(*commandIterator)->execute(true);

				if (tracker.b == GamePad::ButtonStateTracker::RELEASED && ((*commandIterator)->getGamePadButtonBinding() == B))
					(*commandIterator)->execute(false);

				if (tracker.x == GamePad::ButtonStateTracker::PRESSED && ((*commandIterator)->getGamePadButtonBinding() == X))
					(*commandIterator)->execute(true);

				if (tracker.x == GamePad::ButtonStateTracker::RELEASED && ((*commandIterator)->getGamePadButtonBinding() == X))
					(*commandIterator)->execute(false);

				if (tracker.y == GamePad::ButtonStateTracker::PRESSED && ((*commandIterator)->getGamePadButtonBinding() == Y))
					(*commandIterator)->execute(true);

				if (tracker.y == GamePad::ButtonStateTracker::RELEASED && ((*commandIterator)->getGamePadButtonBinding() == Y))
					(*commandIterator)->execute(false);

				if (tracker.back == GamePad::ButtonStateTracker::PRESSED && ((*commandIterator)->getGamePadButtonBinding() == Back))
					(*commandIterator)->execute(true);

				if (tracker.back == GamePad::ButtonStateTracker::RELEASED && ((*commandIterator)->getGamePadButtonBinding() == Back))
					(*commandIterator)->execute(false);

				if (tracker.start == GamePad::ButtonStateTracker::PRESSED && ((*commandIterator)->getGamePadButtonBinding() == Start))
					(*commandIterator)->execute(true);

				if (tracker.start == GamePad::ButtonStateTracker::RELEASED && ((*commandIterator)->getGamePadButtonBinding() == Start))
					(*commandIterator)->execute(false);

				if (tracker.leftShoulder == GamePad::ButtonStateTracker::PRESSED && ((*commandIterator)->getGamePadButtonBinding() == LeftShoulder))
					(*commandIterator)->execute(true);

				if (tracker.leftShoulder == GamePad::ButtonStateTracker::RELEASED && ((*commandIterator)->getGamePadButtonBinding() == LeftShoulder))
					(*commandIterator)->execute(false);

				if (tracker.rightShoulder == GamePad::ButtonStateTracker::PRESSED && ((*commandIterator)->getGamePadButtonBinding() == RightShoulder))
					(*commandIterator)->execute(true);

				if (tracker.rightShoulder == GamePad::ButtonStateTracker::RELEASED && ((*commandIterator)->getGamePadButtonBinding() == RightShoulder))
					(*commandIterator)->execute(false);
			}
		}
	}
}