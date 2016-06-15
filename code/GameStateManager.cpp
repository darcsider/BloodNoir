/*=====================================================================================
$File: GameStateManager.cpp
$Date: June 12, 2016
$Creator: Jamie Cooper
$Notice: (C) Copyright 2015 by Punch Drunk Squirrel Games LLC. All Rights Reserved.
=====================================================================================*/
#include "GameStateManager.h"

/*
BannerParade,
MainMenu,
NewGame,
Tutorial,
MapSelect,
Paused,
LoadSave,
Options,
OnExit
*/
GameStateManager::GameStateManager(DX11RenderManager *graphics, InputHandler *input)
{
	m_graphicsSystem = graphics;
	m_inputHandler = input;
}

GameStateManager::~GameStateManager()
{

}

void GameStateManager::BuildStateManager()
{
	BannerParade *bannerParade = new BannerParade(m_graphicsSystem, m_inputHandler, "..\\data\\bannerParade.txt");
	m_gameStates.push_back(bannerParade);

}

void GameStateManager::ChangeState(StateTypes type)
{
	switch (type)
	{
		case Banner:
			break;
		case MainMenu:
			break;
		case NewGame:
			break;
		case Tutorial:
			break;
		case MapSelect:
			break;
		case Paused:
			break;
		case LoadSave:
			break;
		case Options:
			break;
		case OnExit:
			break;
	}
}

void GameStateManager::Update()
{
	m_currentState->Update();
}