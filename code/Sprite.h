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

// data structure of all data a sprite needs for it
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

// structure used to store all data related to a single animation
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
		SpriteData					m_spriteInfo;		// data needed for a sprite to animate
		map<string, AnimatedAction> m_actionAnimations; // collection of animationActions for walking, and attacking
		AnimatedAction				m_currentAction;	// the currently animating action
		
	public:
		// constructor left blank for now
		Sprite();
		// destructor left blank for now
		~Sprite();
		// update the sprites animating sequence
		void UpdateSprite(float timeDelta);
		// add an action to the collection of actions for the sprite
		void AddAction(int start, int end, double frameD, string name);
		// build the sprite initially
		void BuildSprite(int w, int h, int cols, int rows, float scale, string name);
		// set the current sprite action
		void SetCurrentAction(string name);
		// set the current frame of the animation for the current Action
		void SetCurrentFrame(int frame);
		// bulid the source rectangle or the sprite
		void SetSourceRectangle();

		// return the texture name for the sprite
		string GetSpriteTexture()
		{
			return m_spriteInfo.textureName;
		}

		// return the sprite's width
		int GetSpriteWidth()
		{
			return m_spriteInfo.width;
		}

		// return the sprite's height
		int GetSpriteHeight()
		{
			return m_spriteInfo.height;
		}

		// return the sprite's source Rctangle
		RECT GetSpriteRectangle()
		{
			return m_spriteInfo.sourceRect;
		}
};
#endif