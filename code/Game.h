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
#include "DX11RenderManager.h"
#include "Map.h"
#include "Console.h"

// not complete must do more work just doing this to get started
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

		bool m_fpsOn;
		Map testMap;

		unique_ptr<DX11RenderManager> m_graphicSystem;

		unique_ptr<Keyboard::KeyboardStateTracker> m_keyboardTracker;
		unique_ptr<Keyboard> m_keyboard;
		unique_ptr<GamePad> m_gamePad;

		unique_ptr<Mouse> m_mouse;
		unique_ptr<AudioEngine> m_audioEngine;

		unique_ptr<SoundEffect> m_backgroundMusic;
		unique_ptr<SoundEffectInstance> m_backgroundEffect;

		unique_ptr<SoundEffect> m_soundEffect;
		unique_ptr<SoundEffectInstance> m_effect;

	public:
		Game();

		~Game();

		bool GameInitialize(HWND window, int width, int height);

		void InitializeResources();

		bool DirectXTKInitialize();

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