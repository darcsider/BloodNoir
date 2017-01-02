/*=====================================================================================
$File: Actor.cpp
$Date: September 18, 2016
$Creator: Jamie Cooper
$Notice: (C) Copyright 2015 by Punch Drunk Squirrel Games LLC. All Rights Reserved.
=====================================================================================*/
#include "Actor.h"

Actor::Actor()
{

}

Actor::Actor(RenderManager * graphics, InputManager *input)
{
	m_graphicSystem = graphics;
	m_inputSystem = input;
}

Actor::~Actor()
{

}

void Actor::BuildCharacter(int a, int d, int hp)
{
	m_attack = a;
	m_defense = d;
	m_hitPoints = hp;
}

void Actor::MoveCharacter(MoveDirection direction)
{
	switch (direction)
	{
		case NotMoving:
			m_sprite.SetCurrentAction("notMoving");
			break;
		case MoveLeft:
			m_sprite.SetCurrentAction("moveLeft");
			break;
		case MoveRight:
			m_sprite.SetCurrentAction("moveRight");
			break;
		case MoveUp:
			m_sprite.SetCurrentAction("moveUp");
			break;
		case MoveDown:
			m_sprite.SetCurrentAction("moveDown");
			break;
		default:
			m_sprite.SetCurrentAction("notMoving");
			break;
	}
}

void Actor::UpdateCharacter(float deltaTime)
{
	m_sprite.UpdateSprite(deltaTime);
}

void Actor::DrawCharacter()
{
	m_graphicSystem->RenderObject(m_sprite.GetSpriteTexture(), m_sprite.GetSpriteRectangle(), m_sprite.GetSpritePosition());
}