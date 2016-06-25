/*=====================================================================================
$File: GameStateManager.cpp
$Date: June 12, 2016
$Creator: Jamie Cooper
$Notice: (C) Copyright 2015 by Punch Drunk Squirrel Games LLC. All Rights Reserved.
=====================================================================================*/
#include "GameStateManager.h"

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
	BannerParade *bannerParade = new BannerParade(m_graphicsSystem, m_inputHandler, "..\\data\\BannerParade.txt");
	function<void(StateTypes)> functionPointer = bind(&GameStateManager::ChangeState, this, placeholders::_1);
	bannerParade->SetCallback(functionPointer);
	bannerParade->InputSetup();
	m_gameStates.push_back(bannerParade);

	// NOTE TODO change this at a later date just here for testing purposes
	m_currentState = m_gameStates[0];
}

void GameStateManager::ChangeState(StateTypes type)
{
	StateTypes newType;
	switch (type)
	{
		case Banner:
			newType = Banner;
			break;
		case MainMenu:
			newType = MainMenu;
			break;
		case NewGame:
			newType = NewGame;
			break;
		case Tutorial:
			newType = Tutorial;
			break;
		case MapSelect:
			newType = MapSelect;
			break;
		case InGame:
			newType = InGame;
			break;
		case Paused:
			newType = Paused;
			break;
		case LoadSave:
			newType = LoadSave;
			break;
		case Options:
			newType = Options;
			break;
		case OnExit:
			newType = OnExit;
			break;
	}

	vector<GameState*>::iterator stateIterator;

	for (stateIterator = m_gameStates.begin(); stateIterator != m_gameStates.end(); stateIterator++)
	{
		if ((*stateIterator)->ReturnType() == newType)
			m_currentState = (*stateIterator);
	}
}

void GameStateManager::Process()
{
	m_currentState->Update();
}