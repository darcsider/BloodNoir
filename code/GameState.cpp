/*=====================================================================================
$File: GameState.cpp
$Date: May 18, 2016
$Creator: Jamie Cooper
$Notice: (C) Copyright 2015 by Punch Drunk Squirrel Games LLC. All Rights Reserved.
=====================================================================================*/
#include "GameState.h"

BannerParadeState::BannerParadeState()
{
	// does nothing currently
}

BannerParadeState::BannerParadeState(function<void(StateTypes)> funcPoint, string filename)
{
	// set the state change function pointer
	m_stateChange = funcPoint;
	// set the delay of how long each banner should display before next one
	m_bannerDelay = 2.0f;
	// set the type of the state to Banner
	m_stateType = Banner;
	// Build all banners from text file
	BuildBanners(filename);
}

BannerParadeState::~BannerParadeState()
{
	// does nothing currently
}

StateTypes BannerParadeState::GetStateType()
{
	// return the state type
	return m_stateType;
}

void BannerParadeState::BuildBanners(string fileName)
{
	string tempString;		// string to hold line from text file
	string filename;		// string to hold filename of texture
	string texture;			// string to hold texture name of banner
	bool skip;				// bool holds if this banner can be skipped
	int numberOfBanners;	// how many banners are there

	ifstream inFile(fileName);
	if (inFile)
	{
		// read in a line
		getline(inFile, tempString);
		// convert it to an integer and set it to NumberOfBanners
		numberOfBanners = atoi(tempString.c_str());

		// loop through the banners adding each one to the container
		for (int i = 0; i < numberOfBanners; i++)
		{
			// read in the file name
			getline(inFile, filename);
			// read in the texture name
			getline(inFile, texture);
			// read in the skip option and set variable based on what its value is
			getline(inFile, tempString);
			if (tempString == "TRUE")
				skip = true;
			else
				skip = false;
			
			// insert the banner in the container and add the texture to the RenderManager container
			m_banners.insert(pair<string, bool>(texture, skip));
			RenderManager::GetInstance().AddTexture(filename, texture);
		}
	}
	inFile.close();

	// get the first banner in map and set currentBanner equal to it
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
	// clear input container to make sure no other input is currently set
	InputManager::GetInstance().ClearFunctionPointers();
}

void BannerParadeState::Update(float delta)
{
	// update bannerTimer with time change from timer
	m_bannerTimer += delta;

	// if the bannerTimer is greater than delay
	if (m_bannerTimer >= m_bannerDelay)
	{
		// clear bannerTimer
		m_bannerTimer -= m_bannerDelay;

		// check to see if the current banner is last banner or not
		auto bannerIndex = m_banners.find(m_currentBanner);
		// if its not the end then increment and grab next banner
		if (bannerIndex != m_banners.end())
		{
			bannerIndex++;
			if (bannerIndex != m_banners.end())
			{
				m_currentBanner = bannerIndex->first;
			}
			// if its the end of the banners then move the state to next state
			else
			{
				m_stateChange(MainMenu);
			}
		}
	}
}

void BannerParadeState::Execute()
{
	// draw the currentBanner to the screen
	RECT screenRect;
	screenRect.left = 0;
	screenRect.top = 0;
	screenRect.right = RenderManager::GetInstance().GetGameWidth();
	screenRect.bottom = RenderManager::GetInstance().GetGameHeight();
	RenderManager::GetInstance().RenderObject(m_currentBanner, screenRect);
}

MainMenuState::MainMenuState()
{
	// default constructor left blank for now
}

MainMenuState::MainMenuState(function<void(StateTypes)> funcPoint, string filename)
{
	// set the function pointer to change state
	m_stateChange = funcPoint;
	// set anykey to false
	anyKeyPressed = false;
	// the state is not initialized yet
	initialized = false;
	// set the state type to MainMenu
	m_stateType = MainMenu;
	// build the state with the text file passed in
	BuildMainMenu(filename);
}

MainMenuState::~MainMenuState()
{
	// destructor left blank for now
}

StateTypes MainMenuState::GetStateType()
{
	// return the state type for MainMenu
	return m_stateType;
}

void MainMenuState::BuildMainMenu(string filename)
{
	string file;		// filename for texture being read in
	string texture;		// texture name for texture being read in

	ifstream inFile(filename);
	if (inFile)
	{
		// read a line in and set to file
		getline(inFile, file);
		// read a line in and set to texture
		getline(inFile, texture);

		// set the mainmenubackground equal to first texture passed in
		m_mainMenuBackground = texture;
		// add the texture to the container in RenderManager
		RenderManager::GetInstance().AddTexture(file, texture);

		// read in the next texture
		getline(inFile, file);
		getline(inFile, texture);

		// set the current texture equal to pressanykey
		m_pressAnyKey = texture;
		// add the texture to the container in RenderManager
		RenderManager::GetInstance().AddTexture(file, texture);

		// read in remaining textures for the main menu
		for (int i = 0; i < 8; i++)
		{
			getline(inFile, file);
			getline(inFile, texture);

			m_menuTextures.push_back(texture);
			RenderManager::GetInstance().AddTexture(file, texture);
		}
	}
	inFile.close();
	// Main Menu in its current state will probably get some changes when we get further
	// along when I'm able to add a video to it and other things etc...
}

void MainMenuState::InputCallBack(bool pressed, GameActions action)
{
	// check for input and change the curerntly selected menu option
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
	// clear the current InputManager input to make sure only this input is in there
	InputManager::GetInstance().ClearFunctionPointers();
	// add input actions to the Input Manager
	function<void(bool, GameActions)> inputFunctionPointer = bind(&MainMenuState::InputCallBack, this, placeholders::_1, placeholders::_2);
	InputManager::GetInstance().AddKeyboardActionBinding(ActionAccept, inputFunctionPointer);
	InputManager::GetInstance().AddKeyboardActionBinding(ActionUp, inputFunctionPointer);
	InputManager::GetInstance().AddKeyboardActionBinding(ActionDown, inputFunctionPointer);
}

void MainMenuState::Update(float delta)
{
	// clear the current set of textures to render to screen at beginning of update
	m_renderTextures.clear();

	// depending on what menu option is selected set which textures to render to the screen
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
	// if the state has been initialized then start rendering to the screen
	if (initialized)
	{
		// set the Rectangle to draw to
		RECT screenRect;
		screenRect.left = 0;
		screenRect.top = 0;
		screenRect.right = RenderManager::GetInstance().GetGameWidth();
		screenRect.bottom = RenderManager::GetInstance().GetGameHeight();
		// calculate the starting x and y to begin rendering to and update accordingly
		vector<string>::iterator textureIterator;
		float startX = (float)(RenderManager::GetInstance().GetGameWidth() / 3) + 150;
		float startY = (float)(RenderManager::GetInstance().GetGameHeight() / 2);
		float currX = startX;
		float currY = startY;
		int height = RenderManager::GetInstance().getTextureDesc(m_renderTextures.at(0).c_str()).Height;

		// draw the background to the screen first
		RenderManager::GetInstance().RenderObject(m_mainMenuBackground, screenRect);
		// if input has already been received then start looping through the renderTextures and draw to screen
		if (anyKeyPressed)
		{
			for (textureIterator = m_renderTextures.begin(); textureIterator != m_renderTextures.end(); textureIterator++)
			{
				RenderManager::GetInstance().RenderObject((textureIterator)->c_str(), Vector2(currX, currY));
				currY += height;
			}
		}
		// if no key has been pressed yet just display background and press any key
		else
		{
			RenderManager::GetInstance().RenderObject(m_pressAnyKey, Vector2((float)(RenderManager::GetInstance().GetGameWidth() / 3), startY));
		}
	}
}

NewGameState::NewGameState()
{
	// default constructor left blank currently
}

NewGameState::NewGameState(function<void(StateTypes)> funcPoint, string filename)
{
	// set the state change function pointer
	m_stateChange = funcPoint;
	// set the filename for the state
	m_fileName = filename;
	// set the state to NewGame
	m_stateType = NewGame;

	// Build the new Game State
	BuildNewGameState();
}

NewGameState::~NewGameState()
{
}

StateTypes NewGameState::GetStateType()
{
	// return the state type for the state
	return m_stateType;
}

void NewGameState::BuildNewGameState()
{
	// NewGame State is not what it will be when final
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

	testWorld = new GlobalWorld();
	testWorld->BuildWorlds(mapFileName);
	testWorld->GetCurrentWorld()->SetCurrentSection("section01");
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
	testCharacter->SetActorPosition(Vector2(50, (float)(RenderManager::GetInstance().GetGameHeight() - 100)));
	testCharacter->SetActorCurrentWorld(testWorld->GetCurrentWorld());

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
	testWorld->RenderWorld();
	testCharacter->DrawActor();
}