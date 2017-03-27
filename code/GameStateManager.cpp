/*=====================================================================================
$File: GameStateManager.cpp
$Date: June 12, 2016
$Creator: Jamie Cooper
$Notice: (C) Copyright 2015 by Punch Drunk Squirrel Games LLC. All Rights Reserved.
=====================================================================================*/
#include "GameStateManager.h"

GameStateManager::GameStateManager() :
	m_stateChange(false)
{
	// left blank for now
}

GameStateManager::~GameStateManager()
{
	// left blank for now
}

void GameStateManager::BuildStateManager()
{
	// build the state manager adding all states to the state manager
	// and passing each state its text file to be used to build itself
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
	// change the current state based on what is passed in
	vector<GameState*>::iterator stateIterator;

	for (stateIterator = m_gameStates.begin(); stateIterator != m_gameStates.end(); stateIterator++)
	{
		if ((*stateIterator)->GetStateType() == type)
		{
			m_currentStateType = type;
			m_stateChange = true;
			break;
		}
	}
}

void GameStateManager::Update(float delta)
{
	// update the current state
	m_currentState->Update(delta);
}

void GameStateManager::Execute()
{
	// execute the currentState then check for stateChange happening
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