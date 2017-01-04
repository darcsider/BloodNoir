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

void Actor::BuildActor(int hp, float speed)
{
	m_hitPoints = hp;
	m_actorSpeed = speed;
}

void Actor::MoveActor(MoveDirection direction)
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

void Actor::UpdateActor(float deltaTime)
{
	m_sprite.UpdateSprite(deltaTime);
	m_position.x += m_velocity.x * deltaTime;
	m_position.y += m_velocity.y * deltaTime;
}

void Actor::UpdateActorVelocity(int value)
{
	m_velocity.x = m_actorSpeed * value;
	m_velocity.y = m_actorSpeed * value;
}

void Actor::DrawActor()
{
	m_graphicSystem->RenderObject(m_sprite.GetSpriteTexture(), m_sprite.GetSpriteRectangle(), m_position);
}

void Actor::MoveActorLeft(bool pressed)
{
	if (pressed)
		m_velocity.x = -1;
	else
		m_velocity.x = 0;
}

void Actor::MoveActorRight(bool pressed)
{
	if (pressed)
		m_velocity.x = 1;
	else
		m_velocity.x = 0;
}

void Actor::MoveActorUp(bool pressed)
{
	if (pressed)
		m_velocity.y = -1;
	else
		m_velocity.x = 0;
}

void Actor::MoveActorDown(bool pressed)
{
	if (pressed)
		m_velocity.y = 1;
	else
		m_velocity.x = 0;
}