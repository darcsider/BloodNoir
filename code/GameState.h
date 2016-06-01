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
	public:
		GameState() {}
		virtual ~GameState() {}
		virtual void Update() = 0;
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
		BannerParade();
		~BannerParade();
		void InputCallBack(bool pressedOrReleased);
		void StateChangeCallBack();
		void Update();
};
#endif