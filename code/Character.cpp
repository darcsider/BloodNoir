/*=====================================================================================
$File: Character.cpp
$Date: September 18, 2016
$Creator: Jamie Cooper
$Notice: (C) Copyright 2015 by Punch Drunk Squirrel Games LLC. All Rights Reserved.
=====================================================================================*/
#include "Character.h"

Character::Character()
{
}

Character::Character(RenderManager * graphics, InputManager *input)
{
	m_graphicSystem = graphics;
	m_inputSystem = input;
}

Character::~Character()
{

}

void Character::BuildCharacter(int a, int d, int hp)
{
	m_attack = a;
	m_defense = d;
	m_hitPoints = hp;
}

void Character::MoveCharacter(MoveDirection direction)
{
	switch (direction)
	{
	case NotMoving:
		m_characterSprite.SetCurrentAction("notMoving");
		break;
	case MoveLeft:
		m_characterSprite.SetCurrentAction("moveLeft");
		break;
	case MoveRight:
		m_characterSprite.SetCurrentAction("moveRight");
		break;
	case MoveUp:
		m_characterSprite.SetCurrentAction("moveUp");
		break;
	case MoveDown:
		m_characterSprite.SetCurrentAction("moveDown");
		break;
	default:
		m_characterSprite.SetCurrentAction("notMoving");
		break;
	}
}

void Character::UpdateCharacter(float deltaTime)
{
	m_characterSprite.UpdateSprite(deltaTime);
}

void Character::DrawCharacter()
{
	m_graphicSystem->RenderObject(m_characterSprite.GetSpriteTexture(), m_characterSprite.GetSpriteRectangle(), m_characterSprite.GetSpritePosition(), m_characterSprite.GetTintColor());
}