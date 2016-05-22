/*=====================================================================================
$File: GameState.cpp
$Date: May 18, 2016
$Creator: Jamie Cooper
$Notice: (C) Copyright 2015 by Punch Drunk Squirrel Games LLC. All Rights Reserved.
=====================================================================================*/
#include "GameState.h"

BannerParade::BannerParade()
{

}

BannerParade::~BannerParade()
{

}

void BannerParade::InputCallBack(bool pressedOrReleased)
{
}

void BannerParade::StateChangeCallBack()
{
	if (m_remainingBanners > 0)
	{
		m_nextBanner = true;
		m_remainingBanners--;
	}
}

void BannerParade::Update()
{

}