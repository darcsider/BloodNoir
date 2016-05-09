#pragma once
/*=====================================================================================
$File: WinInput.h
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
	public:
		virtual ~Command() {};
		virtual void execute() = 0;
		virtual void execute(GameActor& actor) = 0;
};

class WinInput
{
	protected:
		unique_ptr<Keyboard::KeyboardStateTracker> m_keyboardTracker;
		unique_ptr<Keyboard> m_keyboard;
		unique_ptr<GamePad> m_gamePad;
		unique_ptr<Mouse> m_mouse;
		map<Keyboard::Keys, function<void()>> m_keyboardMap;
		//map<GamePad::Buttons, function<void()>> m_gamePadMap;
		//map<GamePad::DPad, function<void()>> m_gamePadDPadMap;

	public:
		WinInput();
		~WinInput();

		void InitializeInput();
		void AddKeyboardInput(Keyboard::Keys key, function<void()> functionPointer);
		//void AddGamePadInput(GamePad::Buttons button, function<void()> functionPointer);
		//void AddGamePadDPadInput(GamePad::DPad button, function<void()> functionPointer);
		void HandleInput();
};
#endif