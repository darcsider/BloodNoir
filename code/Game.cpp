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
	m_graphicSystem = std::make_unique<DX11RenderManager>();
	editorConsole = NULL;
#if DEBUG || _DEBUG
	m_fpsOn = true;
#endif
}

Game::~Game()
{
	m_gamePad.reset();
	m_gamePad.release();
	m_mouse.reset();
	m_mouse.release();
	m_keyboard.reset();
	m_keyboard.release();
	m_soundEffect.reset();
	m_soundEffect.release();
	m_audioEngine.reset();
	m_audioEngine.release();
	m_graphicSystem.release();
}

bool Game::DirectXTKInitialize()
{
	AUDIO_ENGINE_FLAGS audioFlags = AudioEngine_Default;
#ifdef _DEBUG
	audioFlags = audioFlags | AudioEngine_Debug;
#endif

	m_audioEngine.reset(new AudioEngine(audioFlags));
	
	m_backgroundMusic.reset(new SoundEffect(m_audioEngine.get(), L"..\\Music\\Electro.wav"));
	m_backgroundEffect = m_backgroundMusic->CreateInstance();

	m_soundEffect.reset(new SoundEffect(m_audioEngine.get(), L"..\\Music\\heli.wav"));
	m_effect = m_soundEffect->CreateInstance();

	m_keyboard = std::make_unique<Keyboard>();
	m_keyboardTracker.reset(new Keyboard::KeyboardStateTracker);
	m_mouse = std::make_unique<Mouse>();
	m_gamePad = std::make_unique<GamePad>();

	//m_backgroundEffect->Play(true);
	
	return true;
}

void Game::InitializeResources()
{
	int numberOfTextures;
	int numberOfMaps;
	std::string tempString;
	std::string filename;
	std::string resourceName;
	std::ifstream inFile("..\\Resources.txt");
	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	std::wstring output;

	if (inFile)
	{
		std::getline(inFile, tempString);
		numberOfTextures = std::atoi(tempString.c_str());

		for (int i = 0; i < numberOfTextures; i++)
		{
			std::getline(inFile, filename);
			std::getline(inFile, resourceName);
			output = converter.from_bytes(filename);
			m_graphicSystem->AddTexture(output, resourceName);
		}
		std::getline(inFile, tempString);
		numberOfMaps = std::atoi(tempString.c_str());

		for (int i = 0; i < numberOfMaps; i++)
		{
			std::getline(inFile, filename);
			testMap.BuildMap(filename);
		}
	}
	inFile.close();
	testMap.SetCurrentMapSection(0);
}

bool Game::GameInitialize(HWND window, int width, int height)
{
	m_timer.Reset();
	m_window = window;
	m_gameWidth = std::max(width, 1);
	m_gameHeight = std::max(height, 1);

	if (!m_graphicSystem->InitializeGraphics(m_window, width, height))
		return false;

	DirectXTKInitialize();
	testMap.InitializeMap(m_graphicSystem.get(), m_keyboard.get(), m_gamePad.get(), m_gameWidth, m_gameHeight);
	InitializeResources();

	editorConsole = new Console();
	editorConsole->initialize(m_graphicSystem.get(), Vector2(0,0), m_gameWidth, 200, XMFLOAT4(0,0,0,0.8));
	m_gameInitialized = true;
	return true;
}

void Game::SystemsUpdate()
{
	auto keyboardState = m_keyboard->GetState();
	m_keyboardTracker->Update(keyboardState);
	auto mouseState = m_mouse->GetState();
	auto gamePadState = m_gamePad->GetState(0);
	auto soundState = m_effect->GetState();

	if (m_keyboardTracker->pressed.Escape)
	{
		PostQuitMessage(0);
	}

	if (m_keyboardTracker->IsKeyReleased(Keyboard::OemTilde))
	{
		editorConsole->HideShow();
	}

	if (gamePadState.IsConnected())
	{
		if (gamePadState.IsBackPressed())
		{
			PostQuitMessage(0);
		}

		if (gamePadState.IsAPressed())
		{
			if (soundState == STOPPED)
				m_effect->Play();
		}
	}

	if (!m_audioEngine->Update())
	{
		// No audio device is active
		if (m_audioEngine->IsCriticalError())
		{
			MessageBox(NULL, "Something went really wrong audio is broken", "Audio Error", MB_OK);
			return;
		}
	}
}

void Game::GameUpdate()
{
	SystemsUpdate();
	testMap.UpdateMap(m_timer.DeltaTime());
}

void Game::DrawScene()
{
	m_graphicSystem->ClearScene();
	m_graphicSystem->BeginScene();
	//g_graphicSystem->DrawObject("background", Vector2(0.0f, 0.0f));
	//g_graphicSystem->DrawObject("clouds", Vector2(0.0f, 0.0f));
	testMap.DrawMap();
	m_graphicSystem->EndScene();
	editorConsole->Draw();
	m_graphicSystem->PresentScene();
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

	if (m_fpsOn)
	{
		static int frameCnt = 0;
		static float timeElapsed = 0.0f;
		static std::string windowTitle = "Blood Noir 1.0";

		frameCnt++;

		// Compute averages over one second period.
		if ((m_timer.TotalTime() - timeElapsed) >= 1.0f)
		{
			float fps = (float)frameCnt; // fps = frameCnt / 1
			float mspf = 1000.0f / fps;

			std::string output = windowTitle +
				" FPS: " + std::to_string(fps) +
				" Frame Time: " + std::to_string(mspf) +
				" (ms)";

			SetWindowText(m_window, output.c_str());

			// Reset for next average.
			frameCnt = 0;
			timeElapsed += 1.0f;
		}
	}
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
	m_gamePad->Resume();
	m_timer.Start();
	m_appPaused = false;
}

void Game::OnDeactivated()
{
	m_gamePad->Suspend();
	m_timer.Stop();
	m_appPaused = true;
}

void Game::OnSuspending()
{
	m_gamePad->Suspend();
	m_timer.Stop();
	m_appPaused = true;
}

void Game::OnResuming()
{
	m_gamePad->Resume();
	m_timer.Start();
	m_appPaused = false;
}

void Game::OnWindowSizeChanged(int width, int height)
{
	m_gameWidth = std::max(width, 1);
	m_gameHeight = std::max(height, 1);
	m_graphicSystem->CreateResources();
}