#pragma once
/*=====================================================================================
$File: Actor.h
$Date: September 18, 2016
$Creator: Jamie Cooper
$Notice: (C) Copyright 2015 by Punch Drunk Squirrel Games LLC. All Rights Reserved.
=====================================================================================*/
#ifndef ACTOR_H
#define ACTOR_H
#include "Includes.h"
#include "RenderManager.h"
#include "InputManager.h"
#include "Sprite.h"

enum MoveDirection
{
	NotMoving,
	MoveLeft,
	MoveRight,
	MoveUp,
	MoveDown
};

class Actor
{
protected:
	RenderManager *m_graphicSystem;
	InputManager *m_inputSystem;
	Sprite m_characterSprite;

	string m_characterName;
	int m_attack;
	int m_defense;
	int m_hitPoints;

public:
	Actor();
	Actor(RenderManager *graphics, InputManager *input);
	~Actor();
	void BuildCharacter(int a, int d, int hp);
	void MoveCharacter(MoveDirection direction = NotMoving);
	void UpdateCharacter(float deltaTime);
	void DrawCharacter();
};
#endif // !ACTOR_H