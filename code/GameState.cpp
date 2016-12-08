/*=====================================================================================
$File: GameState.cpp
$Date: May 18, 2016
$Creator: Jamie Cooper
$Notice: (C) Copyright 2015 by Punch Drunk Squirrel Games LLC. All Rights Reserved.
=====================================================================================*/
#include "GameState.h"

BannerParadeState::BannerParadeState()
{

}

BannerParadeState::BannerParadeState(RenderManager * graphics, InputManager * input, function<void(StateTypes)> funcPoint, string filename)
{
	m_graphics = graphics;
	m_input = input;
	m_stateChange = funcPoint;
	m_bannerDelay = 2.0f;
	m_stateType = Banner;
	BuildBanners(filename);
}

BannerParadeState::~BannerParadeState()
{
}

StateTypes BannerParadeState::GetStateType()
{
	return m_stateType;
}

void BannerParadeState::BuildBanners(string fileName)
{
	string tempString;
	string filename;
	string texture;
	bool skip;
	int numberOfBanners;

	ifstream inFile(fileName);
	if (inFile)
	{
		getline(inFile, tempString);
		
		numberOfBanners = atoi(tempString.c_str());

		for (int i = 0; i < numberOfBanners; i++)
		{
			getline(inFile, filename);
			getline(inFile, texture);
			getline(inFile, tempString);
			if (tempString == "TRUE")
				skip = true;
			else
				skip = false;

			m_banners.insert(pair<string, bool>(texture, skip));
			m_graphics->AddTexture(filename, texture);
		}
	}
	inFile.close();

	map<string, bool>::iterator bannerIterator;
	bannerIterator = m_banners.begin();
	m_currentBanner = bannerIterator->first;
}

void BannerParadeState::InputCallBack(bool pressed)
{
	// TODO(Jamie): Add input to banner parade
}

void BannerParadeState::SetupInput()
{
	m_input->ClearCommands();
}

void BannerParadeState::Update(float delta)
{
	m_bannerTimer += delta;

	if (m_bannerTimer >= m_bannerDelay)
	{
		m_bannerTimer -= m_bannerDelay;

		auto bannerIndex = m_banners.find(m_currentBanner);
		if (bannerIndex != m_banners.end())
		{
			bannerIndex++;
			if (bannerIndex != m_banners.end())
			{
				m_currentBanner = bannerIndex->first;
			}
			else
			{
				m_stateChange(MainMenu);
			}
		}
	}
}

void BannerParadeState::Execute()
{
	RECT screenRect = { 0, 0, m_graphics->GetGameWidth(), m_graphics->GetGameHeight() };
	m_graphics->RenderObject(m_currentBanner, screenRect);
}

MainMenuState::MainMenuState()
{
}

MainMenuState::MainMenuState(RenderManager * graphics, InputManager * input, function<void(StateTypes)> funcPoint, string filename)
{
	m_graphics = graphics;
	m_input = input;
	m_stateChange = funcPoint;
	anyKeyPressed = false;
	initialized = false;
	m_stateType = MainMenu;
	BuildMainMenu(filename);
}

MainMenuState::~MainMenuState()
{
}

StateTypes MainMenuState::GetStateType()
{
	return m_stateType;
}

void MainMenuState::BuildMainMenu(string filename)
{
	string file;
	string texture;

	ifstream inFile(filename);
	if (inFile)
	{
		getline(inFile, file);
		getline(inFile, texture);

		m_mainMenuBackground = texture;
		m_graphics->AddTexture(file, texture);

		getline(inFile, file);
		getline(inFile, texture);

		m_pressAnyKey = texture;
		m_graphics->AddTexture(file, texture);

		for (int i = 0; i < 8; i++)
		{
			getline(inFile, file);
			getline(inFile, texture);

			m_menuTextures.push_back(texture);
			m_graphics->AddTexture(file, texture);
		}
	}
	inFile.close();
}

void MainMenuState::InputCallBack(bool pressed)
{
	if (pressed)
	{
		if (anyKeyPressed)
		{
			if (m_currentSelection == 0)
				m_stateChange(NewGame);

			if (m_currentSelection == 1)
				m_stateChange(LoadSave);

			if (m_currentSelection == 2)
				m_stateChange(Options);

			if (m_currentSelection == 3)
				m_stateChange(OnExit);
		}
		else
		{
			anyKeyPressed = true;
		}
	}
}

void MainMenuState::InputUpCallBack(bool pressed)
{
	if (pressed)
	{
		if (anyKeyPressed)
		{
			if (m_currentSelection == 0)
				m_currentSelection = 3;

			else
				m_currentSelection--;
		}
		else
		{
			anyKeyPressed = true;
		}
	}
}

void MainMenuState::InputDownCallBack(bool pressed)
{
	if (pressed)
	{
		if (anyKeyPressed)
		{
			if (m_currentSelection == 3)
				m_currentSelection = 0;
			else
				m_currentSelection++;
		}
		else
		{
			anyKeyPressed = true;
		}
	}
}

void MainMenuState::SetupInput()
{
	m_input->ClearCommands();

	function<void(bool)> funcPoint = bind(&MainMenuState::InputCallBack, this, placeholders::_1);
	//m_input->AddKeyboardCommand(Keyboard::Keys::Enter, funcPoint);
	//m_input->AddGamePadButtonCommand(A, funcPoint);

	function<void(bool)> funcPointUp = bind(&MainMenuState::InputUpCallBack, this, placeholders::_1);
	//m_input->AddKeyboardCommand(Keyboard::Keys::Up, funcPointUp);
	//m_input->AddGamePadDpadCommand(Up, funcPointUp);

	function<void(bool)> funcPointDown = bind(&MainMenuState::InputDownCallBack, this, placeholders::_1);
	//m_input->AddKeyboardCommand(Keyboard::Keys::Down, funcPointDown);
	//m_input->AddGamePadDpadCommand(Down, funcPointDown);

	m_input->AddKeyboardActionBinding(ActionAccept, funcPoint);
	m_input->AddKeyboardActionBinding(ActionUp, funcPointUp);
	m_input->AddKeyboardActionBinding(ActionDown, funcPointDown);
}

void MainMenuState::Update(float delta)
{
	m_renderTextures.clear();

	if (m_currentSelection == 0)
	{
		m_renderTextures.push_back(m_menuTextures.at(0));
		m_renderTextures.push_back(m_menuTextures.at(3));
		m_renderTextures.push_back(m_menuTextures.at(5));
		m_renderTextures.push_back(m_menuTextures.at(7));
	}
	else if (m_currentSelection == 1)
	{
		m_renderTextures.push_back(m_menuTextures.at(1));
		m_renderTextures.push_back(m_menuTextures.at(2));
		m_renderTextures.push_back(m_menuTextures.at(5));
		m_renderTextures.push_back(m_menuTextures.at(7));
	}
	else if (m_currentSelection == 2)
	{
		m_renderTextures.push_back(m_menuTextures.at(1));
		m_renderTextures.push_back(m_menuTextures.at(3));
		m_renderTextures.push_back(m_menuTextures.at(4));
		m_renderTextures.push_back(m_menuTextures.at(7));
	}
	else if (m_currentSelection == 3)
	{
		m_renderTextures.push_back(m_menuTextures.at(1));
		m_renderTextures.push_back(m_menuTextures.at(3));
		m_renderTextures.push_back(m_menuTextures.at(5));
		m_renderTextures.push_back(m_menuTextures.at(6));
	}
	initialized = true;
}

void MainMenuState::Execute()
{
	if (initialized)
	{
		RECT screenRect = { 0, 0, m_graphics->GetGameWidth(), m_graphics->GetGameHeight() };
		vector<string>::iterator textureIterator;
		float startX = (float)(m_graphics->GetGameWidth() / 3) + 150;
		float startY = (float)(m_graphics->GetGameHeight() / 2);
		float currX = startX;
		float currY = startY;
		int height = m_graphics->getTextureDesc(m_renderTextures.at(0).c_str()).Height;

		m_graphics->RenderObject(m_mainMenuBackground, screenRect);
		if (anyKeyPressed)
		{
			for (textureIterator = m_renderTextures.begin(); textureIterator != m_renderTextures.end(); textureIterator++)
			{
				m_graphics->RenderObject((textureIterator)->c_str(), Vector2(currX, currY));
				currY += height;
			}
		}
		else
		{
			m_graphics->RenderObject(m_pressAnyKey, Vector2((float)(m_graphics->GetGameWidth() / 3), startY));
		}
	}
}

NewGameState::NewGameState()
{
}

NewGameState::NewGameState(RenderManager * graphics, InputManager * input, function<void(StateTypes)> funcPoint, string filename)
{
	m_graphics = graphics;
	m_input = input;
	m_stateChange = funcPoint;
	m_fileName = filename;
	m_stateType = NewGame;

	BuildNewGameState();
}

NewGameState::~NewGameState()
{
}

StateTypes NewGameState::GetStateType()
{
	return m_stateType;
}

void NewGameState::BuildNewGameState()
{

	ifstream inFile(m_fileName);
	int numberOfTextures;
	string tempString;
	string filename;
	string resourceName;
	string mapFileName;

	if (inFile)
	{
		getline(inFile, tempString);
		numberOfTextures = atoi(tempString.c_str());

		for (int i = 0; i < numberOfTextures; i++)
		{
			getline(inFile, filename);
			getline(inFile, resourceName);
			m_graphics->AddTexture(filename, resourceName);
		}
		getline(inFile, mapFileName);
	}
	inFile.close();

	testMap = new Map();
	testMap->InitializeMap(m_graphics, m_input, mapFileName);
	testMap->BuildMap();
}

void NewGameState::InputCallBack(bool pressed)
{
	
}

void NewGameState::SetupInput()
{
	m_input->ClearCommands();

	testMap->SetupInput();
}

void NewGameState::Update(float delta)
{
	testMap->UpdateMap(delta);
}

void NewGameState::Execute()
{
	testMap->DrawMap();
}
