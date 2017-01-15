/*=====================================================================================
$File: GameStateManager.cpp
$Date: June 12, 2016
$Creator: Jamie Cooper
$Notice: (C) Copyright 2015 by Punch Drunk Squirrel Games LLC. All Rights Reserved.
=====================================================================================*/
#include "GameStateManager.h"

GameStateManager::GameStateManager()
{
	m_stateChange = false;
}

GameStateManager::~GameStateManager()
{

}

void GameStateManager::BuildStateManager()
{
	function<void(StateTypes)> funcPointer = bind(&GameStateManager::ChangeState, this, placeholders::_1);
	BannerParadeState *banner = new BannerParadeState(funcPointer, "..\\data\\BannerParade.txt");
	m_gameStates.push_back(banner);

	MainMenuState *menu = new MainMenuState(funcPointer, "..\\data\\MainMenu.txt");
	m_gameStates.push_back(menu);

	NewGameState *newG = new NewGameState(funcPointer, "..\\data\\NewGame.txt");
	m_gameStates.push_back(newG);

	OnExitState *exit = new OnExitState();
	m_gameStates.push_back(exit);

	m_currentState = m_gameStates.at(0);
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
		if ((*stateIterator)->GetStateType() == newType)
		{
			m_currentStateType = newType;
			m_stateChange = true;
			break;
		}
	}
}

void GameStateManager::Update(float delta)
{
	m_currentState->Update(delta);
}

void GameStateManager::Execute()
{
	m_currentState->Execute();
	if (m_stateChange)
	{
		vector<GameState*>::iterator stateIterator;

		for (stateIterator = m_gameStates.begin(); stateIterator != m_gameStates.end(); stateIterator++)
		{
			if ((*stateIterator)->GetStateType() == m_currentStateType)
			{
				m_currentState = (*stateIterator);
				m_stateChange = false;
				break;
			}
		}
		m_currentState->SetupInput();
	}
}