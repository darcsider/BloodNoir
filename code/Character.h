#pragma once
/*=====================================================================================
$File: Character.h
$Date: September 18, 2016
$Creator: Jamie Cooper
$Notice: (C) Copyright 2015 by Punch Drunk Squirrel Games LLC. All Rights Reserved.
=====================================================================================*/
#ifndef CHARACTER_H
#define CHARACTER_H
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

class Character
{
	protected:
		RenderManager *m_graphicSystem;
		InputManager *m_inputSystem;
		string m_characterName;
		int m_attack;
		int m_defense;
		int m_hitPoints;
		Sprite m_characterSprite;

	public:
		Character();
		Character(RenderManager *graphics, InputManager *input);
		~Character();
		void BuildCharacter(int a, int d, int hp);
		void MoveCharacter(MoveDirection direction = NotMoving);
		void UpdateCharacter(float deltaTime);
		void DrawCharacter();
};
#endif // !CHARACTER_H