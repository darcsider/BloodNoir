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

void Sprite::AddAction(int start, int end, double frameD, string name)
{
	AnimatedAction tempAction{ start, end, 0, frameD };
	m_actionAnimations.insert(pair<string, AnimatedAction>(name, tempAction));
}

void Sprite::BuildSprite(int w, int h, int cols, int rows, float scale, string name)
{
	m_spriteInfo.width = w / cols;
	m_spriteInfo.height = h / rows;
	m_spriteInfo.scale = scale;
	m_spriteInfo.textureName = name;
	m_spriteInfo.columns = cols;
	m_spriteInfo.rows = rows;

	SetCurrentAction("notMoving");
	SetSourceRectangle();
}

void Sprite::SetCurrentAction(string name)
{
	auto actionIndex = m_actionAnimations.find(name);

	if (actionIndex != m_actionAnimations.end())
		m_currentAction = actionIndex->second;

	m_currentAction.currentFrame = m_currentAction.startFrame;
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
	m_spriteInfo.sourceRect.left = (m_currentAction.currentFrame % m_spriteInfo.columns) * m_spriteInfo.width;
	m_spriteInfo.sourceRect.right = m_spriteInfo.sourceRect.left + m_spriteInfo.width;
	m_spriteInfo.sourceRect.top = (m_currentAction.currentFrame / m_spriteInfo.columns) * m_spriteInfo.height;
	m_spriteInfo.sourceRect.bottom = m_spriteInfo.sourceRect.top + m_spriteInfo.height;
}