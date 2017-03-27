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
#include "InputManager.h"
#include "World.h"
#include "Actor.h"
#include "Console.h"

// different types of states that the game will have used for switching
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

// basic abstract virtual class gamestate
// not to be instantiated, must be inherited and built
class GameState
{
	protected:
		function<void(StateTypes)> m_stateChange;	// function pointer for changing the state
		StateTypes m_stateType;						// what type of state is the state

	public:
		// constructor
		GameState() {}
		// destructor
		virtual ~GameState() {}
		// return the state type of the state pure virtual
		virtual StateTypes GetStateType() = 0;
		// input was received process if the state needs to pure virtual
		virtual void InputCallBack(bool pressed, GameActions action) = 0;
		// setup the input the state needs to handle pure virtual
		virtual void SetupInput() = 0;
		// update the state pure virtual
		virtual void Update(float delta) = 0;
		// execute/ process what needs to be done in this state pure virtual
		virtual void Execute() = 0;
};

class BannerParadeState : public GameState
{
	protected:
		map<string, bool> m_banners;	// "banners"/ logo's at beginning of game
		string m_currentBanner;			// what is the current banner on screen
		float m_bannerTimer;			// how long has the current banner been on screen
		float m_bannerDelay;			// how long should the current banner be on screen before moving to next

	public:
		// default constructor does nothing
		BannerParadeState();
		// constructor takes the function pointer used to change states and the filename of text file to build state
		BannerParadeState(function<void(StateTypes)> funcPoint, string filename);
		// destructor does nothing
		virtual ~BannerParadeState();
		// return the states type
		virtual StateTypes GetStateType();
		// build the banner Parade state with the text file passed in
		void BuildBanners(string filename);
		// if this state needs to process input it is received here
		virtual void InputCallBack(bool pressed, GameActions action);
		// setup any input commands here
		virtual void SetupInput();
		// update the banner parade state
		virtual void Update(float delta);
		// execute / process what needs done in the banner parade
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
		MainMenuState(function<void(StateTypes)> funcPoint, string filename);
		virtual ~MainMenuState();
		virtual StateTypes GetStateType();
		void BuildMainMenu(string filename);
		virtual void InputCallBack(bool pressed, GameActions action);
		virtual void SetupInput();
		virtual void Update(float delta);
		virtual void Execute();
};

// New Game State in its current shape is not representative of what it will be in final version
// this is here so I can test out certain features as I'm developing them.
class NewGameState : public GameState
{
	protected:
		GlobalWorld *testWorld;
		Actor *testCharacter;
		Sprite *testCharacterSprite;
		string m_fileName;

	public:
		NewGameState();
		NewGameState(function<void(StateTypes)> funcPoint, string filename);
		virtual ~NewGameState();
		virtual StateTypes GetStateType();
		void BuildNewGameState();
		virtual void InputCallBack(bool pressed, GameActions action);
		virtual void SetupInput();
		virtual void Update(float delta);
		virtual void Execute();

		//method for testing not staying
		void SetupCharacter();
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
		void InputCallBack(bool notUsed, GameActions action) {}
		void SetupInput() {}
		void Update(float notUsed) {}
		void Execute()
		{
			PostQuitMessage(0);
		}
};
#endif