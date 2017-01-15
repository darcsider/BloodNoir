/*=====================================================================================
$File: Game.cpp
$Date: January 6, 2016
$Creator: Jamie Cooper
$Notice: (C) Copyright 2015 by Punch Drunk Squirrel Games LLC. All Rights Reserved.
=====================================================================================*/
#include "Game.h"

Game::Game() :
	m_window(0),
	m_gameWidth(1920),
	m_gameHeight(1080)
{

}

Game::~Game()
{
	m_audioSystem.release();
}

bool Game::GameInitialize(HWND window, int width, int height)
{
	m_timer.Reset();
	m_window = window;
	m_gameWidth = max(width, 1);
	m_gameHeight = max(height, 1);

	RenderManager::GetInstance().InitializeGraphics(m_window, width, height);

	m_audioSystem = make_unique<AudioSystem>();
	m_audioSystem->InitializeAudioSystem();

	// EditorConsole will be going away from the game class as soon as I complete writing the level editor class
	// then I will be moving it over to there, I will then figure out how to change the Console class in a way
	// that other systems can subscribe to the Console class and can setup their own commands that the console
	// will recognize and process.
	editorConsole = new Console();
	editorConsole->Initialize(Vector2(0, 0), RenderManager::GetInstance().GetGameWidth(), 200, XMFLOAT4(0, 0, 0, 0.8f));
	
	m_audioSystem->SetBackgroundMusic("..\\Music\\Electro.wav");
	m_audioSystem->AddEffect("heli", "..\\Music\\heli.wav");

	m_stateManager = new GameStateManager();
	m_stateManager->BuildStateManager();

	m_audioSystem->PlayBackgroundMusic();
	m_gameInitialized = true;
	return true;
}

void Game::SystemsUpdate()
{
	m_audioSystem->AudioSystemUpdate();
	InputManager::GetInstance().ProcessCommands();
}

void Game::GameUpdate()
{
	SystemsUpdate();
	m_stateManager->Update(m_timer.DeltaTime());
}

void Game::DrawScene()
{
	RenderManager::GetInstance().ClearScene();
	RenderManager::GetInstance().BeginScene();
	m_stateManager->Execute();
	RenderManager::GetInstance().EndScene();
	editorConsole->Draw();
	RenderManager::GetInstance().PresentScene();
}

void Game::GameRun()
{
	m_timer.Tick();

	if (m_gameInitialized)
	{
		if (!m_appPaused)
		{
			GameUpdate();
			DrawScene();
			CalculateFrameStats();
		}
		else
		{
			Sleep(100);
		}
	}
}

void Game::CalculateFrameStats()
{
	// Code computes the average frames per second, and also the 
	// average time it takes to render one frame.  These stats 
	// are appended to the window caption bar.

#if	WIN32 && _DEBUG
	static int frameCnt = 0;
	static float timeElapsed = 0.0f;
	static string windowTitle = "Blood Noir 1.0";

	frameCnt++;

	// Compute averages over one second period.
	if ((m_timer.TotalTime() - timeElapsed) >= 1.0f)
	{
		float fps = (float)frameCnt; // fps = frameCnt / 1
		float mspf = 1000.0f / fps;

		string output = windowTitle +
			" FPS: " + to_string(fps) +
			" Frame Time: " + to_string(mspf) +
			" (ms)";

		SetWindowText(m_window, output.c_str());

		// Reset for next average.
		frameCnt = 0;
		timeElapsed += 1.0f;
	}
#endif
}

void Game::KeyboardProcess(UINT message, WPARAM wParam, LPARAM lParam)
{
	Keyboard::ProcessMessage(message, wParam, lParam);
}

void Game::CharactersInput(WPARAM wParam)
{
	if (editorConsole != NULL)
		editorConsole->TextInput(wParam);
}

void Game::MouseProcess(UINT message, WPARAM wParam, LPARAM lParam)
{
	Mouse::ProcessMessage(message, wParam, lParam);
}

void Game::OnActivated()
{
	InputManager::GetInstance().GamePadResume();
	m_timer.Start();
	m_appPaused = false;
}

void Game::OnDeactivated()
{
	InputManager::GetInstance().GamePadSuspend();
	m_timer.Stop();
	m_appPaused = true;
}

void Game::OnSuspending()
{
	InputManager::GetInstance().GamePadSuspend();
	m_timer.Stop();
	m_appPaused = true;
}

void Game::OnResuming()
{
	InputManager::GetInstance().GamePadResume();
	m_timer.Start();
	m_appPaused = false;
}

void Game::OnWindowSizeChanged(int width, int height)
{
	m_gameWidth = max(width, 1);
	m_gameHeight = max(height, 1);
	RenderManager::GetInstance().OnWindowSizeChange();
}