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
#include "RenderManager.h"

struct SpriteData
{
	int width;
	int height;
	int columns;
	int rows;
	Vector2 position;
	float scale;
	SimpleMath::Rectangle sourceRect;
	string texture;
};

struct AnimatedAction
{
	int startFrame;
	int endFrame;
	int currentFrame;
	float frameDelay;
	float animationTimer;
};

class Sprite
{
	protected:
		SpriteData m_spriteInfo;
		map<string, AnimatedAction> m_actionAnimations;
		AnimatedAction m_currentAction;
		RenderManager *m_graphicSystem;
		
	public:
		Sprite();
		Sprite(RenderManager *graphics);
		~Sprite();
		void UpdateSprite(float timeDelta);
		void AddAction(int start, int end, float frameD, string name);
		void BuildSprite(int w, int h, int cols, int rows, float scale, float startx, float starty, string filename, string name);
		void SetCurrentAction(string name);
		void SetCurrentFrame(int frame);
		void SetSourceRectangle();

		string GetSpriteTexture()
		{
			return m_spriteInfo.texture;
		}

		int GetSpriteWidth()
		{
			return m_spriteInfo.width;
		}

		int GetSpriteHeight()
		{
			return m_spriteInfo.height;
		}

		Vector2 GetSpritePosition()
		{
			return m_spriteInfo.position;
		}

		SimpleMath::Rectangle GetSpriteRectangle()
		{
			return m_spriteInfo.sourceRect;
		}
};
#endif