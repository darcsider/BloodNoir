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
#include "Components.h"
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
		Sprite *m_sprite;

		string m_characterName;
		int m_hitPoints;
		float m_actorSpeed;
		Vector2 m_velocity;
		Vector2 m_position;
		
		GraphicsComponent *m_graphics;
		PhysicsComponent *m_physics;
		InputComponent *m_input;

	public:
		Actor();
		Actor(RenderManager *graphicsManager, InputManager *inputManager, GraphicsComponent *graphics, PhysicsComponent *physics, InputComponent *input, Sprite *sprite);
		~Actor();
		void BuildActor(int hp, float speed);
		void MoveActor(MoveDirection direction = NotMoving);
		void UpdateActor(float deltaTime);
		void UpdateActorVelocity(int value);
		void DrawActor();
		void MoveActor(bool pressed, GameActions action);

		Sprite* GetActorSprite()
		{
			return m_sprite;
		}

		Vector2 GetActorPosition()
		{
			return m_position;
		}

		void SetActorPosition(Vector2 position)
		{
			m_position = position;
		}

		Vector2 GetActorVelocity()
		{
			return m_velocity;
		}

		void SetActorVelocity(Vector2 velocity)
		{
			m_velocity = velocity;
		}
};
#endif // !ACTOR_H