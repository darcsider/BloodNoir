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

Actor::~Actor()
{

}

void Actor::BuildActor(int hp, int attack, int defense, float speed, GraphicsComponent *graphics, PhysicsComponent *physics, InputComponent *input, Sprite *sprite)
{
	m_hitPoints = hp;
	m_attack = attack;
	m_defense = defense;
	m_actorSpeed = speed;
	m_graphics = graphics;
	m_physics = physics;
	m_input = input;
	m_sprite = sprite;
}

void Actor::Update(float deltaTime)
{
	m_sprite->UpdateSprite(deltaTime);
	m_physics->update(*this, deltaTime, *m_currentWorld);
}

void Actor::DrawActor()
{
	m_graphics->update(*this, &RenderManager::GetInstance());
}

void Actor::MoveActor(bool pressed, GameActions action)
{
	m_input->update(*this, pressed, action);
}

void Actor::SetActorDirection(MoveDirection direction)
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

void Actor::SetActorCurrentWorld(World * world)
{
	m_currentWorld = world;
}

Sprite * Actor::GetActorSprite()
{
	if (m_sprite != NULL)
		return m_sprite;
}

Vector2 Actor::GetActorPosition()
{
	return m_position;
}

void Actor::SetActorPosition(Vector2 position)
{
	m_position = position;
}

Vector2 Actor::GetActorVelocity()
{
	return m_velocity;
}

void Actor::SetActorVelocity(Vector2 velocity)
{
	m_velocity = velocity;
}

float Actor::GetActorMovementSpeed()
{
	return m_actorSpeed;
}

void Actor::SetActorMovementSpeed(float speed)
{
	m_actorSpeed = speed;
}