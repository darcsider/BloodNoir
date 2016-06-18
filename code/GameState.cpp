/*=====================================================================================
$File: GameState.cpp
$Date: May 18, 2016
$Creator: Jamie Cooper
$Notice: (C) Copyright 2015 by Punch Drunk Squirrel Games LLC. All Rights Reserved.
=====================================================================================*/
#include "GameState.h"

BannerParade::BannerParade(DX11RenderManager *graphics, InputHandler *input, string file)
{
	m_graphics = graphics;
	m_input = input;
	m_fileName = file;

	m_currentBanner = 0;
	BuildBanners();
}

void BannerParade::InputSetup()
{
	m_input->ClearCommands();

	Command *skipCommand = new Command();
	skipCommand->setKeyboardKeyBinding(Keyboard::Keys::Enter);

	function<void(bool)> functionPointer = bind(&BannerParade::InputCallBack, this, placeholders::_1);
	skipCommand->setCallbackFunction(functionPointer);
}

void BannerParade::BuildBanners()
{
	int numberOfBanners = 0;
	string tempString;
	ifstream inFile(m_fileName);

	if (inFile)
	{
		getline(inFile, tempString);
		numberOfBanners = atoi(tempString.c_str());

		for (int i = 0; i < numberOfBanners; i++)
		{
			getline(inFile, tempString);
			m_textureNames.push_back(tempString);
			
			getline(inFile, tempString);
			if (tempString == "TRUE")
				m_skipBanner.push_back(true);
			else if (tempString == "FALSE")
				m_skipBanner.push_back(false);
		}
	}
	inFile.close();
}

BannerParade::~BannerParade()
{
	delete m_graphics;
	m_textureNames.clear();
	m_skipBanner.clear();
}

void BannerParade::InputCallBack(bool pressedOrReleased)
{
	if (pressedOrReleased)
	{
		m_changeBanner = true;
	}
}

void BannerParade::SetCallback(function<void(StateTypes)> point)
{
	m_callBack = point;
}

void BannerParade::TimerCallBack()
{
	if (m_textureNames.size() > 0)
	{
		m_nextBanner = true;
		m_textureNames.erase(m_textureNames.begin() + m_currentBanner);
		m_skipBanner.erase(m_skipBanner.begin() + m_currentBanner);
		m_currentBanner++;
	}
}

void BannerParade::Update()
{
	if (m_textureNames.size() > 0)
	{
		if (m_changeBanner)
		{
			if (m_skipBanner[m_currentBanner])
			{
				m_textureNames.erase(m_textureNames.begin() + m_currentBanner);
				m_skipBanner.erase(m_skipBanner.begin() + m_currentBanner);
				m_nextBanner = true;
			}
		}

		if (m_nextBanner)
		{
			m_currentBanner++;
			m_nextBanner = false;
			m_changeBanner = false;
		}

		m_graphics->DrawObject(m_textureNames[m_currentBanner], Vector2(0.0, 0.0));
	}
	else
	{
		m_callBack(MainMenu);
	}
}