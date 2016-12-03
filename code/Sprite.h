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
	float locX;
	float locY;
	float scale;
	RECT sourceRect;
	string texture;
	const XMVECTORF32& color;
	SpriteData() : color(Colors::White) {}
};

struct AnimatedAction
{
	int startFrame;
	int endFrame;
	int currentFrame;
	float frameDelay;
	Vector2 speedVector;
};

class Sprite
{
	protected:
		SpriteData m_spriteInfo;
		map<string, AnimatedAction> m_actions;
		AnimatedAction m_currentAction;
		RenderManager *m_graphicSystem;
		
	public:
		Sprite();
		Sprite(RenderManager *graphics);
		~Sprite();
		void AddAction(int start, int end, float frameD, string name);
		void BuildSprite(int w, int h, int cols, int rows, float scale, float startx, float starty, string filename, string name, const XMVECTORF32& color = Colors::White);
		void SetCurrentAction(string name);
};
#endif