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
	m_graphicSystem = make_unique<DX11RenderManager>();
	editorConsole = NULL;
#if DEBUG || _DEBUG
	m_fpsOn = true;
#endif
}

Game::~Game()
{
	m_graphicSystem.release();
	m_input.release();
	m_audioSystem.release();
}

void Game::InitializeResources()
{
	int numberOfTextures;
	int numberOfMaps;
	string tempString;
	string filename;
	string resourceName;
	ifstream inFile("..\\Resources.txt");
	wstring_convert<codecvt_utf8<wchar_t>> converter;
	wstring output;

	if (inFile)
	{
		getline(inFile, tempString);
		numberOfTextures = atoi(tempString.c_str());

		for (int i = 0; i < numberOfTextures; i++)
		{
			getline(inFile, filename);
			getline(inFile, resourceName);
			output = converter.from_bytes(filename);
			m_graphicSystem->AddTexture(output, resourceName);
		}
		getline(inFile, tempString);
		numberOfMaps = atoi(tempString.c_str());

		for (int i = 0; i < numberOfMaps; i++)
		{
			getline(inFile, filename);
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
	m_gameWidth = max(width, 1);
	m_gameHeight = max(height, 1);

	if (!m_graphicSystem->InitializeGraphics(m_window, width, height))
		return false;

	m_input = make_unique<WinInput>();
	m_input->InitializeInput();

	testMap.InitializeMap(m_graphicSystem.get(), m_input.get(), m_gameWidth, m_gameHeight);
	InitializeResources();


	// this crap will go away once I have completed building the other systems related to the game state
	Command *testCommand = new Command();
	testCommand->setKeyboardKeyBinding(Keyboard::Keys::Escape);
	testCommand->setGamePadButton(Back);

	function<void(bool)> funcPoint = bind(&Game::CloseGame, this, placeholders::_1);
	testCommand->setCallbackFunction(funcPoint);

	m_input->AddCommand(testCommand);

	m_audioSystem = make_unique<WinAudio>();
	m_audioSystem->InitializeAudio();

	//function<void(bool)> funcPoint2 = bind(&Game::TestSound, this, placeholders::_1);
	//m_input->AddKeyboardInput(Keyboard::Keys::A, funcPoint2);
	
	// EditorConsole will be going away from the game class as soon as I complete writing the level editor class
	// then I will be moving it over to there, I will then figure out how to change the Console class in a way
	// that other systems can subscribe to the Console class and can setup their own commands that the console
	// will recognize and process.
	editorConsole = new Console();
	editorConsole->initialize(m_graphicSystem.get(), m_input.get(), Vector2(0,0), m_gameWidth, 200, XMFLOAT4(0,0,0,0.8));
	m_audioSystem->SetBackgroundMusic("..\\Music\\Electro.wav");
	m_audioSystem->AddSoundEffect("heli", "..\\Music\\heli.wav");

	//m_audioSystem->StartBackgroundMusic();
	m_gameInitialized = true;
	return true;
}

void Game::SystemsUpdate()
{
	m_audioSystem->AudioEngineUpdate();
	m_input->HandleInput();
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
	//m_gamePad->Resume();
	m_timer.Start();
	m_appPaused = false;
}

void Game::OnDeactivated()
{
	//m_gamePad->Suspend();
	m_timer.Stop();
	m_appPaused = true;
}

void Game::OnSuspending()
{
	//m_gamePad->Suspend();
	m_timer.Stop();
	m_appPaused = true;
}

void Game::OnResuming()
{
	//m_gamePad->Resume();
	m_timer.Start();
	m_appPaused = false;
}

void Game::OnWindowSizeChanged(int width, int height)
{
	m_gameWidth = max(width, 1);
	m_gameHeight = max(height, 1);
	m_graphicSystem->CreateResources();
}