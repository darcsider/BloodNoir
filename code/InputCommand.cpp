/*=====================================================================================
$File: InputCommand.cpp
$Date: May 8, 2016
$Creator: Jamie Cooper
$Notice: (C) Copyright 2015 by Punch Drunk Squirrel Games LLC. All Rights Reserved.
=====================================================================================*/
#include "InputCommand.h"

WinInput::WinInput()
{

}

WinInput::~WinInput()
{

}

void WinInput::InitializeInput()
{
	m_keyboard = make_unique<Keyboard>();
	m_gamePad = make_unique<GamePad>();
	m_keyboardTracker.reset(new Keyboard::KeyboardStateTracker);
}

void WinInput::AddCommand(Command *comm)
{
	m_gameCommands.push_back(comm);
}

void WinInput::ClearCommands()
{
	m_gameCommands.clear();
}

void WinInput::HandleInput()
{
	auto keyboardState = m_keyboard->GetState();
	m_gamepadState = m_gamePad->GetState(0);

	GamePad::ButtonStateTracker tracker;

	m_gamepadState = m_gamePad->GetState(0);
	m_keyboardTracker->Update(keyboardState);

	if (m_gamepadState.IsConnected())
		tracker.Update(m_gamepadState);

	for (int i = 0; i < m_gameCommands.size(); i++)
	{
		if (m_keyboardTracker->IsKeyPressed(m_gameCommands[i]->getKeyboardBinding()))
			m_gameCommands[i]->execute(true);

		if (m_keyboardTracker->IsKeyReleased(m_gameCommands[i]->getKeyboardBinding()))
			m_gameCommands[i]->execute(false);

	}

	for (int i = 0; i < m_gameCommands.size(); i++)
	{
		if (m_gameCommands[i]->hasDpadBinding())
		{
			if (tracker.dpadUp == GamePad::ButtonStateTracker::PRESSED && (m_gameCommands[i]->getGamePadDpad() == Up))
				m_gameCommands[i]->execute(true);

			if (tracker.dpadUp == GamePad::ButtonStateTracker::RELEASED && (m_gameCommands[i]->getGamePadDpad() == Up))
				m_gameCommands[i]->execute(false);

			if (tracker.dpadDown == GamePad::ButtonStateTracker::PRESSED && (m_gameCommands[i]->getGamePadDpad() == Down))
				m_gameCommands[i]->execute(true);

			if (tracker.dpadDown == GamePad::ButtonStateTracker::RELEASED && (m_gameCommands[i]->getGamePadDpad() == Down))
				m_gameCommands[i]->execute(false);

			if (tracker.dpadLeft == GamePad::ButtonStateTracker::PRESSED && (m_gameCommands[i]->getGamePadDpad() == Left))
				m_gameCommands[i]->execute(true);

			if (tracker.dpadLeft == GamePad::ButtonStateTracker::UP && (m_gameCommands[i]->getGamePadDpad() == Left))
				m_gameCommands[i]->execute(false);

			if (tracker.dpadRight == GamePad::ButtonStateTracker::PRESSED && (m_gameCommands[i]->getGamePadDpad() == Right))
				m_gameCommands[i]->execute(true);

			if (tracker.dpadRight == GamePad::ButtonStateTracker::RELEASED && (m_gameCommands[i]->getGamePadDpad() == Right))
				m_gameCommands[i]->execute(false);
		}


		if (m_gameCommands[i]->hasButtonBinding())
		{
			if (tracker.a == GamePad::ButtonStateTracker::PRESSED && (m_gameCommands[i]->getGamePadButton() == A))
				m_gameCommands[i]->execute(true);

			if (tracker.a == GamePad::ButtonStateTracker::RELEASED && (m_gameCommands[i]->getGamePadButton() == A))
				m_gameCommands[i]->execute(false);

			if (tracker.b == GamePad::ButtonStateTracker::PRESSED && (m_gameCommands[i]->getGamePadButton() == B))
				m_gameCommands[i]->execute(true);

			if (tracker.b == GamePad::ButtonStateTracker::RELEASED && (m_gameCommands[i]->getGamePadButton() == B))
				m_gameCommands[i]->execute(false);

			if (tracker.x == GamePad::ButtonStateTracker::PRESSED && (m_gameCommands[i]->getGamePadButton() == X))
				m_gameCommands[i]->execute(true);

			if (tracker.x == GamePad::ButtonStateTracker::RELEASED && (m_gameCommands[i]->getGamePadButton() == X))
				m_gameCommands[i]->execute(false);

			if (tracker.y == GamePad::ButtonStateTracker::PRESSED && (m_gameCommands[i]->getGamePadButton() == Y))
				m_gameCommands[i]->execute(true);

			if (tracker.y == GamePad::ButtonStateTracker::RELEASED && (m_gameCommands[i]->getGamePadButton() == Y))
				m_gameCommands[i]->execute(false);

			if (tracker.back == GamePad::ButtonStateTracker::PRESSED && (m_gameCommands[i]->getGamePadButton() == Back))
				m_gameCommands[i]->execute(true);

			if (tracker.back == GamePad::ButtonStateTracker::RELEASED && (m_gameCommands[i]->getGamePadButton() == Back))
				m_gameCommands[i]->execute(false);

			if (tracker.start == GamePad::ButtonStateTracker::PRESSED && (m_gameCommands[i]->getGamePadButton() == Start))
				m_gameCommands[i]->execute(true);

			if (tracker.start == GamePad::ButtonStateTracker::RELEASED && (m_gameCommands[i]->getGamePadButton() == Start))
				m_gameCommands[i]->execute(false);

			if (tracker.leftShoulder == GamePad::ButtonStateTracker::PRESSED && (m_gameCommands[i]->getGamePadButton() == LeftShoulder))
				m_gameCommands[i]->execute(true);

			if (tracker.leftShoulder == GamePad::ButtonStateTracker::RELEASED && (m_gameCommands[i]->getGamePadButton() == LeftShoulder))
				m_gameCommands[i]->execute(false);

			if (tracker.rightShoulder == GamePad::ButtonStateTracker::PRESSED && (m_gameCommands[i]->getGamePadButton() == RightShoulder))
				m_gameCommands[i]->execute(true);

			if (tracker.rightShoulder == GamePad::ButtonStateTracker::RELEASED && (m_gameCommands[i]->getGamePadButton() == RightShoulder))
				m_gameCommands[i]->execute(false);
		}
	}
}