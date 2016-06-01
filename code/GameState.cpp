/*=====================================================================================
$File: GameState.cpp
$Date: May 18, 2016
$Creator: Jamie Cooper
$Notice: (C) Copyright 2015 by Punch Drunk Squirrel Games LLC. All Rights Reserved.
=====================================================================================*/
#include "GameState.h"

BannerParade::BannerParade()
{
	m_currentBanner = 0;
}

BannerParade::~BannerParade()
{

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

void BannerParade::Update()
{
	if (m_changeBanner)
	{
		if (m_skipBanner[m_currentBanner])
		{
			m_currentBanner++;
			m_textureNames.erase(m_textureNames.begin() + m_currentBanner);
			m_skipBanner.erase(m_skipBanner.begin() + m_currentBanner);
			m_nextBanner = true;
		}
	}

	if (m_nextBanner)
	{
		m_currentBanner++;
		m_nextBanner = false;
	}


}