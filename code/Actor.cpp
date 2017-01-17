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
	m_input->update(*this);
	m_physics->update(*this, deltaTime, *m_currentWorld);
	m_sprite->UpdateSprite(deltaTime);
}

void Actor::DrawActor()
{
	m_graphics->update(*this, RenderManager::GetInstance());
}

void Actor::MoveActor(bool pressed, GameActions action)
{
	if (pressed)
	{
		switch (action)
		{
			case ActionUp:
				break;
			case ActionDown:
				break;
			case ActionLeft:
				break;
			case ActionRight:
				break;
			case ActionAccept:
				break;
			case ActionCancel:
				break;
			case CharacterAttack:
				break;
			case CharacterDefense:
				break;
			case DirectionMoveLeft:
				SetActorDirection(MoveLeft);
				break;
			case DirectionMoveRight:
				SetActorDirection(MoveRight);
				break;
			case DirectionMoveUp:
				SetActorDirection(MoveUp);
				break;
			case DirectionMoveDown:
				SetActorDirection(MoveDown);
				break;
			case SystemTest:
				break;
			case SystemExitEarly:
				break;
			case SystemConsole:
				break;
			default:
				SetActorDirection(NotMoving);
				break;
		}
	}
	else
	{
		SetActorDirection(NotMoving);
	}
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
	m_currentDirection = direction;
}

void Actor::SetActorCurrentWorld(World * world)
{
	m_currentWorld = world;
}

Sprite * Actor::GetActorSprite()
{
	return m_sprite;
}

MoveDirection Actor::GetActorDirection()
{
	return m_currentDirection;
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

GraphicsComponent * Actor::GetGraphicsComponent()
{
	return m_graphics;
}

void Actor::SetGraphicsComponent(GraphicsComponent * graphics)
{
	m_graphics = graphics;
}

PhysicsComponent * Actor::GetPhysicsComponent()
{
	return m_physics;
}

void Actor::SetPhysicsComponent(PhysicsComponent * physics)
{
	m_physics = physics;
}

InputComponent * Actor::GetInputComponent()
{
	return m_input;
}

void Actor::SetInputComponent(InputComponent * input)
{
	m_input = input;
}