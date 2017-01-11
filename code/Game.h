#pragma once
/*=====================================================================================
$File: Game.h
$Date: January 6, 2016
$Creator: Jamie Cooper
$Notice: (C) Copyright 2015 by Punch Drunk Squirrel Games LLC. All Rights Reserved.
=====================================================================================*/
#ifndef GAME_H
#define GAME_H
#include "Includes.h"
#include "GameTiming.h"
#include "RenderManager.h"
#include "World.h"
#include "Console.h"
#include "InputManager.h"
#include "AudioSystem.h"
#include "GameStateManager.h"

class Game
{
	private:
		GameTiming m_timer;
		GameTiming m_eventTimer;

		vector<int> handles;
		bool m_appPaused;

		int m_gameWidth = 0;
		int m_gameHeight = 0;
		bool m_gameInitialized = false;
		HWND m_window;
		
		int	backgroundIndex;

		Console *editorConsole;

		GameStateManager *m_stateManager;

		unique_ptr<InputManager> m_input;
		unique_ptr<RenderManager> m_graphicSystem;
		unique_ptr<AudioSystem> m_audioSystem;

	public:
		Game();

		~Game();

		bool GameInitialize(HWND window, int width, int height);

		void GameUpdate();

		void SystemsUpdate();

		void GameRun();

		void PauseGame()
		{
			m_appPaused = true;
			m_timer.Stop();
		}

		void UnPauseGame()
		{
			m_appPaused = false;
			m_timer.Start();
		}

		void CloseGame(bool notUsed)
		{
			PostQuitMessage(0);
		}

		void TestSound(bool notUsed)
		{
			m_audioSystem->PlayEffect("heli");
		}

		void DrawScene();

		void CalculateFrameStats();

		void OnActivated();

		void OnDeactivated();

		void OnSuspending();

		void OnResuming();

		void OnWindowSizeChanged(int width, int height);

		void KeyboardProcess(UINT message, WPARAM wParam, LPARAM lParam);

		void CharactersInput(WPARAM wParam);

		void MouseProcess(UINT message, WPARAM wParam, LPARAM lParam);

		void GetDefaultSize(int& width, int& height) const
		{
			width = 1920;
			height = 1080;
		}

		void SetWindowWidth(int width)
		{
			m_gameWidth = width;
		}

		int GetWindowWidth()
		{
			return m_gameWidth;
		}

		void SetWindowHeight(int height)
		{
			m_gameHeight = height;
		}

		int GetWindowHeight()
		{
			return m_gameHeight;
		}
};
#endif // !GAME_H