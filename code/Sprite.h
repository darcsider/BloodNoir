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
	float locX;
	float locY;
	float scale;
	RECT sourceRect;
	string texture;
};

struct AnimatedAction
{
	int startFrame;
	int endFrame;
	int currentFrame;
	float frameDelay;
};

class Sprite
{
	protected:
		SpriteData m_spriteInfo;
		map<string, AnimatedAction> m_actions;
		AnimatedAction m_currentAction;

	public:
		Sprite();
		~Sprite();
		void AddAction(int start, int end, string name);
		void BuildSprite(int w, int h, float scale, float startx, float starty, string filename, const XMVECTORF32& color = Colors::White);
		void SetCurrentAction(string name);
};
#endif