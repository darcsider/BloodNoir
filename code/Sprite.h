#pragma once
/*=====================================================================================
$File: Sprite.h
$Date: July 5, 2016
$Creator: Jamie Cooper
$Notice: (C) Copyright 2015 by Punch Drunk Squirrel Games LLC. All Rights Reserved.
=====================================================================================*/
#ifndef SPRITE_H
#define SPRITE_H
#include "Includes.h"

struct SpriteData
{
	int width;
	int height;
	int columns;
	int rows;
	float scale;
	RECT sourceRect;
	string textureName;
};

struct AnimatedAction
{
	int startFrame;
	int endFrame;
	int currentFrame;
	double frameDelay;
	double animationTimer;
};

class Sprite
{
	protected:
		SpriteData m_spriteInfo;
		map<string, AnimatedAction> m_actionAnimations;
		AnimatedAction m_currentAction;
		
	public:
		Sprite();
		~Sprite();
		void UpdateSprite(float timeDelta);
		void AddAction(int start, int end, double frameD, string name);
		void BuildSprite(int w, int h, int cols, int rows, float scale, string name);
		void SetCurrentAction(string name);
		void SetCurrentFrame(int frame);
		void SetSourceRectangle();

		string GetSpriteTexture()
		{
			return m_spriteInfo.textureName;
		}

		int GetSpriteWidth()
		{
			return m_spriteInfo.width;
		}

		int GetSpriteHeight()
		{
			return m_spriteInfo.height;
		}

		RECT GetSpriteRectangle()
		{
			return m_spriteInfo.sourceRect;
		}
};
#endif