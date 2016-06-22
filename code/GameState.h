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

enum StateTypes
{
	Banner,
	MainMenu,
	NewGame,
	Tutorial,
	MapSelect,
	Map,
	Paused,
	LoadSave,
	Options,
	OnExit
};

class GameState
{
	protected:
		DX11RenderManager *m_graphics;
		InputHandler *m_input;
		string m_stateFileName;
		StateTypes m_type;
		function<void(StateTypes)> m_callBack;

	public:
		GameState() {}
		virtual ~GameState() {}
		virtual StateTypes ReturnType() = 0;
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
		void InputSetup();
		void SetCallback(function<void(StateTypes)> point);

		StateTypes ReturnType()
		{
			return m_type;
		}
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