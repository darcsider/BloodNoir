/*=====================================================================================
$File: Sprite.cpp
$Date: July 5, 2016
$Creator: Jamie Cooper
$Notice: (C) Copyright 2015 by Punch Drunk Squirrel Games LLC. All Rights Reserved.
=====================================================================================*/
#include "Sprite.h"

Sprite::Sprite()
{

}

Sprite::Sprite(RenderManager *graphics)
{
	m_graphicSystem = graphics;
}

Sprite::~Sprite()
{

}

void Sprite::AddAction(int start, int end, float frameD, string name)
{
	AnimatedAction tempAction{ start, end, 0, frameD };
	m_actions.insert(pair<string, AnimatedAction>(name, tempAction));
}

void Sprite::BuildSprite(int w, int h, int cols, int rows, float scale, float startx, float starty, string filename, string name, const XMVECTORF32& color)
{
	D3D11_TEXTURE2D_DESC tempDesc;

	m_graphicSystem->AddTexture(filename, name);
	tempDesc = m_graphicSystem->getTextureDesc(name);

	m_spriteInfo.width = w;
	m_spriteInfo.height = h;
	m_spriteInfo.scale = scale;
	m_spriteInfo.locX = startx;
	m_spriteInfo.locY = starty;
	m_spriteInfo.texture = name;
	m_spriteInfo.columns = cols;
	m_spriteInfo.rows = rows;
	m_spriteInfo.sourceRect.left = (m_currentAction.currentFrame % m_spriteInfo.columns) * m_spriteInfo.width;
	m_spriteInfo.sourceRect.right = m_spriteInfo.sourceRect.left + m_spriteInfo.width;
	m_spriteInfo.sourceRect.top = (m_currentAction.currentFrame / m_spriteInfo.columns) * m_spriteInfo.height;
	m_spriteInfo.sourceRect.bottom = m_spriteInfo.sourceRect.top + m_spriteInfo.height;

}

void Sprite::SetCurrentAction(string name)
{
	auto actionIndex = m_actions.find(name);

	if (actionIndex != m_actions.end())
		m_currentAction = actionIndex->second;
}