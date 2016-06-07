/*=====================================================================================
$File: GameState.cpp
$Date: May 18, 2016
$Creator: Jamie Cooper
$Notice: (C) Copyright 2015 by Punch Drunk Squirrel Games LLC. All Rights Reserved.
=====================================================================================*/
#include "GameState.h"

BannerParade::BannerParade(DX11RenderManager *graphics, InputHandler *input)
{
	m_graphics = graphics;
	m_input = input;
	
	m_input->ClearCommands();

	Command *skipCommand = new Command();
	skipCommand->setKeyboardKeyBinding(Keyboard::Keys::Enter);
	skipCommand->setGamePadDpadBinding(Left);

	function<void(bool)> functionPointer = bind(&BannerParade::InputCallBack, this, placeholders::_1);
	skipCommand->setCallbackFunction(functionPointer);

	m_currentBanner = 0;
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

void BannerParade::StateChangeCallBack()
{
	if (m_textureNames.size() > 0)
	{
		m_nextBanner = true;
		m_textureNames.erase(m_textureNames.begin() + m_currentBanner);
		m_skipBanner.erase(m_skipBanner.begin() + m_currentBanner);
		m_currentBanner++;
	}
}

void BannerParade::AddBanner(string textureName, bool canSkip)
{
	m_textureNames.push_back(textureName);
	m_skipBanner.push_back(canSkip);
}

void BannerParade::Update()
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