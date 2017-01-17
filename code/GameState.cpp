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

BannerParadeState::BannerParadeState(function<void(StateTypes)> funcPoint, string filename)
{
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
			RenderManager::GetInstance().AddTexture(filename, texture);
		}
	}
	inFile.close();

	map<string, bool>::iterator bannerIterator;
	bannerIterator = m_banners.begin();
	m_currentBanner = bannerIterator->first;
}

void BannerParadeState::InputCallBack(bool pressed, GameActions action)
{
	// TODO(Jamie): Add input to banner parade
}

void BannerParadeState::SetupInput()
{
	InputManager::GetInstance().ClearFunctionPointers();
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
	SimpleMath::Rectangle screenRect(0, 0, RenderManager::GetInstance().GetGameWidth(), RenderManager::GetInstance().GetGameHeight());
	RenderManager::GetInstance().RenderObject(m_currentBanner, screenRect);
}

MainMenuState::MainMenuState()
{
}

MainMenuState::MainMenuState(function<void(StateTypes)> funcPoint, string filename)
{
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
		RenderManager::GetInstance().AddTexture(file, texture);

		getline(inFile, file);
		getline(inFile, texture);

		m_pressAnyKey = texture;
		RenderManager::GetInstance().AddTexture(file, texture);

		for (int i = 0; i < 8; i++)
		{
			getline(inFile, file);
			getline(inFile, texture);

			m_menuTextures.push_back(texture);
			RenderManager::GetInstance().AddTexture(file, texture);
		}
	}
	inFile.close();
}

void MainMenuState::InputCallBack(bool pressed, GameActions action)
{
	if (pressed && action == ActionUp)
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

	else if (pressed && action == ActionDown)
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

	if (pressed && action == ActionAccept)
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

void MainMenuState::SetupInput()
{
	InputManager::GetInstance().ClearFunctionPointers();
	function<void(bool, GameActions)> inputFunctionPointer = bind(&MainMenuState::InputCallBack, this, placeholders::_1, placeholders::_2);
	InputManager::GetInstance().AddKeyboardActionBinding(ActionAccept, inputFunctionPointer);
	InputManager::GetInstance().AddKeyboardActionBinding(ActionUp, inputFunctionPointer);
	InputManager::GetInstance().AddKeyboardActionBinding(ActionDown, inputFunctionPointer);
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
		SimpleMath::Rectangle screenRect(0, 0, RenderManager::GetInstance().GetGameWidth(), RenderManager::GetInstance().GetGameHeight());
		vector<string>::iterator textureIterator;
		float startX = (float)(RenderManager::GetInstance().GetGameWidth() / 3) + 150;
		float startY = (float)(RenderManager::GetInstance().GetGameHeight() / 2);
		float currX = startX;
		float currY = startY;
		int height = RenderManager::GetInstance().getTextureDesc(m_renderTextures.at(0).c_str()).Height;

		RenderManager::GetInstance().RenderObject(m_mainMenuBackground, screenRect);
		if (anyKeyPressed)
		{
			for (textureIterator = m_renderTextures.begin(); textureIterator != m_renderTextures.end(); textureIterator++)
			{
				RenderManager::GetInstance().RenderObject((textureIterator)->c_str(), Vector2(currX, currY));
				currY += height;
			}
		}
		else
		{
			RenderManager::GetInstance().RenderObject(m_pressAnyKey, Vector2((float)(RenderManager::GetInstance().GetGameWidth() / 3), startY));
		}
	}
}

NewGameState::NewGameState()
{
}

NewGameState::NewGameState(function<void(StateTypes)> funcPoint, string filename)
{
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
			RenderManager::GetInstance().AddTexture(filename, resourceName);
		}
		getline(inFile, mapFileName);
	}
	inFile.close();

	testWorld = new World();
	testWorld->InitializeWorld(mapFileName);
	testWorld->BuildWorld();
}

void NewGameState::InputCallBack(bool pressed, GameActions action)
{
	//Test code not staying
	if (pressed)
		m_stateChange(MainMenu);
}

void NewGameState::SetupInput()
{
	InputManager::GetInstance().ClearFunctionPointers();
	
	//Test code not staying this way
	function<void(bool, GameActions)> funcPoint = bind(&NewGameState::InputCallBack, this, placeholders::_1, placeholders::_2);
	InputManager::GetInstance().AddKeyboardActionBinding(SystemTest, funcPoint);

	function<void(bool, GameActions)> closeGame = bind(&World::CloseGame, testWorld, placeholders::_1, placeholders::_2);
	InputManager::GetInstance().AddKeyboardActionBinding(SystemExitEarly, closeGame);

	function<void(bool, GameActions)> moveMap = bind(&World::MoveWorld, testWorld, placeholders::_1, placeholders::_2);
	InputManager::GetInstance().AddKeyboardActionBinding(ActionLeft, moveMap);
	InputManager::GetInstance().AddKeyboardActionBinding(ActionRight, moveMap);

	SetupCharacter();
}

void NewGameState::SetupCharacter()
{
	testCharacter = new Actor();
	testCharacterSprite = new Sprite();

	// NONE of this will stay this way simply testing that character can display on screen and move around properly.
	D3D11_TEXTURE2D_DESC charDesc = RenderManager::GetInstance().getTextureDesc("TestCharacter");
	testCharacterSprite->BuildSprite(charDesc.Width, charDesc.Height, 12, 8, 1, "TestCharacter");
	testCharacterSprite->AddAction(12, 14, 0.15, "moveLeft");
	testCharacterSprite->AddAction(24, 26, 0.15, "moveRight");
	testCharacterSprite->AddAction(36, 38, 0.15, "moveUp");
	testCharacterSprite->AddAction(0, 2, 0.15, "moveDown");
	testCharacterSprite->AddAction(0, 0, 0.15, "notMoving");
	testCharacterSprite->SetCurrentFrame(0);
	testCharacter->BuildActor(100, 100, 100, 100, new PlayerGraphicsComponent, new PlayerPhysicsCompoonent, new PlayerInputComponent, testCharacterSprite);
	testCharacter->SetActorPosition(Vector2(100, 100));
	testCharacter->SetActorCurrentWorld(testWorld);

	function<void(bool, GameActions)> moveActor = bind(&Actor::MoveActor, testCharacter, placeholders::_1, placeholders::_2);
	InputManager::GetInstance().AddKeyboardActionBinding(DirectionMoveLeft, moveActor);
	InputManager::GetInstance().AddKeyboardActionBinding(DirectionMoveRight, moveActor);
	InputManager::GetInstance().AddKeyboardActionBinding(DirectionMoveUp, moveActor);
	InputManager::GetInstance().AddKeyboardActionBinding(DirectionMoveDown, moveActor);
}

void NewGameState::Update(float delta)
{
	testWorld->UpdateWorld(delta);
	testCharacter->Update(delta);
}

void NewGameState::Execute()
{
	testWorld->DrawWorld();
	testCharacter->DrawActor();
}