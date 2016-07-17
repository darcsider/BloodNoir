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
#include "RenderManager.h"
#include "Win32_Input.h"
#include "Map.h"
#include "Console.h"

enum StateTypes
{
	Banner,
	MainMenu,
	NewGame,
	Tutorial,
	MapSelect,
	InGame,
	Paused,
	LoadSave,
	Options,
	OnExit
};

class GameState
{
	protected:
		RenderManager *m_graphics;
		InputManager *m_input;
		function<void(StateTypes)> m_stateChange;
		StateTypes m_stateType;

	public:
		GameState() {}
		virtual ~GameState() {}
		virtual StateTypes GetStateType() = 0;
		virtual void InputCallBack(bool pressed) = 0;
		virtual void SetupInput() = 0;
		virtual void Update(float delta) = 0;
		virtual void Execute() = 0;
};

class BannerParadeState : public GameState
{
	protected:
		map<string, bool> m_banners;
		string m_currentBanner;
		float m_bannerTimer;
		float m_bannerDelay;

	public:
		BannerParadeState();
		BannerParadeState(RenderManager *graphics, InputManager *input, function<void(StateTypes)> funcPoint, string filename);
		virtual ~BannerParadeState();
		virtual StateTypes GetStateType();
		void BuildBanners(string filename);
		virtual void InputCallBack(bool pressed);
		virtual void SetupInput();
		virtual void Update(float delta);
		virtual void Execute();
};

class MainMenuState : public GameState
{
	protected:
		float m_mainMenuDelay;				// this will be used later to show a gameplay video if player sits at MainMenu to long
		string m_mainMenuBackground;		// string for the main menu background texture
		vector<string> m_menuTextures;		// vector of the text for the menu options and their selected counter parts
		vector<string> m_renderTextures;	// vector of the current textures to be rendered on the screen.
		string m_pressAnyKey;				// text saying press any key to start
		int m_currentSelection;				// currently selected menu option
		bool anyKeyPressed;					// have we moved beyond the press any key screen
		bool initialized;

	public:
		MainMenuState();
		MainMenuState(RenderManager *graphics, InputManager *input, function<void(StateTypes)> funcPoint, string filename);
		virtual ~MainMenuState();
		virtual StateTypes GetStateType();
		void BuildMainMenu(string filename);
		virtual void InputCallBack(bool pressed);
		void InputUpCallBack(bool pressed);
		void InputDownCallBack(bool pressed);
		virtual void SetupInput();
		virtual void Update(float delta);
		virtual void Execute();
};

// New Game State in its current shape is not representative of what it will be in final version
// this is here so I can test out certain features as I'm developing them.
class NewGameState : public GameState
{
	protected:
		Map *testMap;
		string m_fileName;

	public:
		NewGameState();
		NewGameState(RenderManager *graphics, InputManager *input, function<void(StateTypes)> funcPoint, string filename);
		virtual ~NewGameState();
		virtual StateTypes GetStateType();
		void BuildNewGameState();
		virtual void InputCallBack(bool pressed);
		virtual void SetupInput();
		virtual void Update(float delta);
		virtual void Execute();
};

class OnExitState : public GameState
{
	public:
		OnExitState() 
		{
			m_stateType = OnExit;
		}
		virtual ~OnExitState() {}
		StateTypes GetStateType()
		{
			return m_stateType;
		}
		void InputCallBack(bool notUsed) {}
		void SetupInput() {}
		void Update(float notUsed) {}
		void Execute()
		{
			PostQuitMessage(0);
		}
};
#endif