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

Actor::Actor(RenderManager *graphicsManager, InputManager *inputManager, GraphicsComponent *graphics, PhysicsComponent *physics, InputComponent *input, Sprite *sprite)
{
	m_graphicSystem = graphicsManager;
	m_inputSystem = inputManager;
	m_graphics = graphics;
	m_physics = physics;
	m_input = input;
	m_sprite = sprite;
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
			m_sprite->SetCurrentAction("notMoving");
			break;
		case MoveLeft:
			m_sprite->SetCurrentAction("moveLeft");
			break;
		case MoveRight:
			m_sprite->SetCurrentAction("moveRight");
			break;
		case MoveUp:
			m_sprite->SetCurrentAction("moveUp");
			break;
		case MoveDown:
			m_sprite->SetCurrentAction("moveDown");
			break;
		default:
			m_sprite->SetCurrentAction("notMoving");
			break;
	}
}

void Actor::UpdateActor(float deltaTime)
{
	m_sprite->UpdateSprite(deltaTime);
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
	m_graphics->update(this, m_graphicSystem);
}

void Actor::MoveActor(bool pressed, GameActions action)
{
	if (pressed && action == DirectionMoveLeft)
		m_velocity.x = -1;
	else if (pressed && action == DirectionMoveRight)
		m_velocity.x = 1;
	else if (pressed && action == DirectionMoveUp)
		m_velocity.y = -1;
	else if (pressed && action == DirectionMoveDown)
		m_velocity.y = 1;
	else
	{
		m_velocity.x = 0;
		m_velocity.y = 0;
	}
}