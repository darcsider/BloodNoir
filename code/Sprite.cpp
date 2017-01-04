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

void Sprite::UpdateSprite(float timeDelta)
{
	if (m_currentAction.endFrame - m_currentAction.startFrame > 0)
	{
		m_currentAction.animationTimer += timeDelta;
		if (m_currentAction.animationTimer > m_currentAction.frameDelay)
		{
			m_currentAction.animationTimer -= m_currentAction.frameDelay;
			m_currentAction.currentFrame++;
			
			if (m_currentAction.currentFrame < m_currentAction.startFrame || m_currentAction.currentFrame > m_currentAction.endFrame)
				m_currentAction.currentFrame = m_currentAction.startFrame;

			SetSourceRectangle();
		}
	}
}

void Sprite::AddAction(int start, int end, float frameD, string name)
{
	AnimatedAction tempAction{ start, end, 0, frameD };
	m_actionAnimations.insert(pair<string, AnimatedAction>(name, tempAction));
}

void Sprite::BuildSprite(int w, int h, int cols, int rows, float scale, float startx, float starty, string filename, string name)
{
	D3D11_TEXTURE2D_DESC tempDesc;

	m_graphicSystem->AddTexture(filename, name);
	tempDesc = m_graphicSystem->getTextureDesc(name);

	m_spriteInfo.width = w;
	m_spriteInfo.height = h;
	m_spriteInfo.scale = scale;
	m_spriteInfo.texture = name;
	m_spriteInfo.columns = cols;
	m_spriteInfo.rows = rows;
}

void Sprite::SetCurrentAction(string name)
{
	auto actionIndex = m_actionAnimations.find(name);

	if (actionIndex != m_actionAnimations.end())
		m_currentAction = actionIndex->second;
}

void Sprite::SetCurrentFrame(int frame)
{
	if (frame > 0)
	{
		m_currentAction.currentFrame = frame;
		SetSourceRectangle();
	}
}

void Sprite::SetSourceRectangle()
{
	m_spriteInfo.sourceRect.x = (m_currentAction.currentFrame % m_spriteInfo.columns) * m_spriteInfo.width;
	m_spriteInfo.sourceRect.width = m_spriteInfo.sourceRect.x + m_spriteInfo.width;
	m_spriteInfo.sourceRect.y = (m_currentAction.currentFrame / m_spriteInfo.columns) * m_spriteInfo.height;
	m_spriteInfo.sourceRect.height = m_spriteInfo.sourceRect.y + m_spriteInfo.height;
}