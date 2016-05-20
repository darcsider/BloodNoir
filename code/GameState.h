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

class GameState
{
	protected:

	public:
		GameState() {}
		virtual ~GameState() {}
		virtual void Update() = 0;
};

class BannerParade : public GameState
{
	protected:
		bool m_nextBanner;
		int m_remainingBanners;
		int m_numberOfBanners;
		EventManager *m_eventManager;

	public:
		void CallbackFunction();
		BannerParade();
		~BannerParade();
		void Update();

};
#endif