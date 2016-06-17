#pragma once
/*=====================================================================================
$File: GameState.h
$Date: May 18, 2016
$Creator: Jamie Cooper
$Notice: (C) Copyright 2015 by Punch Drunk Squirrel Games LLC. All Rights Reserved.
=====================================================================================*/
#ifndef GAMESTATE_H
#define GAMESTATE_H
#include "Includes.h"
#include "EventManager.h"
#include "InputCommand.h"

class GameState
{
	protected:
		DX11RenderManager *m_graphics;
		InputHandler *m_input;
		string m_stateFileName;

	public:
		GameState() {}
		virtual ~GameState() {}
		virtual void Update() = 0;
		virtual void InputSetup() = 0;
};

class BannerParade : public GameState
{
	protected:
		bool m_nextBanner;
		int m_currentBanner;
		bool m_changeBanner;
		string m_fileName;
		vector<string> m_textureNames;
		vector<bool> m_skipBanner;
		EventManager *m_eventManager;
		Command *m_actionCommand;

	public:
		BannerParade(DX11RenderManager *graphics, InputHandler *input, string file);
		~BannerParade();
		void BuildBanners();
		void InputCallBack(bool pressedOrReleased);
		void TimerCallBack();
		void Update();
};

class MainMenu : public GameState
{
	protected:
		int m_optionSelected;
		string m_mainMenuTexture;
		vector<string> m_menuOptions;
	public:
		MainMenu();
		~MainMenu();
		void InputCallBack(bool pressedOrReleased);
		void StateChangeCallBack();
		void Update();
};
#endif

/*
- MainMenu- Game State
InputObject - inherited from base GameState set in constructor
Input callback methods for up, down, and AcceptButton press - these will be set in constructor
current Menu option selected variable
Constructor()
Destructor()
Update()
draw titleScreen
draw MenuItems in current state

input Callback for Up
If first menu option is selected then
switch to last menu option
else
menu option selected - 1

input callback for down
if last menu option is selected then
switch to first menu option
else
menu option selected + 1

input callback accept button
fire an event off for changing the GameState
if New Game Selected set GameState to NewGame/Tutorial
if LoadGame Selected set GameState to LoadGame
if Options Selected set GameState to OptionsMenu
if Exit Selected set GameState to OnExit
*/