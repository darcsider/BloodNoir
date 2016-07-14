#pragma once
/*=====================================================================================
$File: GameStateManager.h
$Date: June 12, 2016
$Creator: Jamie Cooper
$Notice: (C) Copyright 2015 by Punch Drunk Squirrel Games LLC. All Rights Reserved.
=====================================================================================*/
#ifndef GAMESTATEMANAGER_H
#define GAMESTATEMANAGER_H
#include "Includes.h"
#include "GameState.h"

/*Banner Parade - timed show opening screens including the company logo and any other screens required
- Main Menu - main title screen where it will be waiting for input from the user to progress the state
- New Game - Load new game specific stuff then move state to tutorial
- Tutorial - This state comes after New Game and introduces the main game play mechanics of the game
- Map Select - This state can be initiated from Tutorial, or the In Map state this is where the player selects
  what area of the city they want to go to
- In Map - This is the main game play state where all of the story will take place for the game this state can
  transition to Paused, or back to Map Select
- Paused - This is the state where the paused screen will be displayed this is where the player can check out
  certain collections of theirs, their achievements, etc... this screen is only reachable from InMap state this
  state/ window will also be where the player can select to go back to Map Select, Options Menu, Save and Exit
  the game.
- Load Saved Game - This is only selectable from Main Menu it will load the necessary data to load the players
  saved game progress and get them back to where they left off this state will either transition to In Map or
  to Map Select states.
- Options Menu - This is accessible from Main Menu, Paused and Map Select this will show a window where the player
  can set certain settings like key bindings and music and effect volumes.
- On Exit - THis state is accessible from Paused as well as any other state by pressing Escape when this state is
  invoked it will ask the player if they would like to save their game before exiting.*/

class GameStateManager
{
	protected:
		RenderManager *m_graphicsSystem;
		InputManager *m_inputHandler;
		GameState *m_currentState;
		vector<GameState*> m_gameStates;

	public:
		GameStateManager() { }
		GameStateManager(RenderManager *graphics, InputManager *input);
		~GameStateManager();
		void BuildStateManager();
		void ChangeState(StateTypes type);
		void Update(float delta);
		void Execute();
};
#endif