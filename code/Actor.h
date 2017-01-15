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
#include "World.h"

enum MoveDirection
{
	NotMoving,
	MoveLeft,
	MoveRight,
	MoveUp,
	MoveDown
};

class GraphicsComponent;
class PhysicsComponent;
class InputComponent;

class Actor
{
	protected:
		Sprite *m_sprite;

		string m_characterName;
		int m_hitPoints;
		int m_attack;
		int m_defense;
		float m_actorSpeed;
		Vector2 m_velocity;
		Vector2 m_position;

		GraphicsComponent *m_graphics;
		PhysicsComponent *m_physics;
		InputComponent *m_input;

	public:
		Actor();
		~Actor();
		void BuildActor(int hp, int attack, int defense, float speed, GraphicsComponent *graphics, PhysicsComponent *physics, InputComponent *input, Sprite *sprite);
		void ChangeActorDirection(MoveDirection direction = NotMoving);
		void UpdateActor(float deltaTime);
		void UpdateActorVelocity(int value);
		void DrawActor();
		void MoveActor(bool pressed, GameActions action);

		Sprite* GetActorSprite();
		Vector2 GetActorPosition();
		void SetActorPosition(Vector2 position);
		Vector2 GetActorVelocity();
		void SetActorVelocity(Vector2 velocity);
};

class GraphicsComponent
{
	public:
		virtual void update(Actor &actor, RenderManager *graphics) = 0;
};

class PlayerGraphicsComponent : public GraphicsComponent
{
	public:
		virtual void update(Actor &actor, RenderManager *graphics)
		{
			graphics->RenderObject(actor.GetActorSprite()->GetSpriteTexture(), actor.GetActorSprite()->GetSpriteRectangle(), actor.GetActorPosition());
		}
};

class PhysicsComponent
{
	public:
		virtual void update(Actor &actor, World &world) = 0;
};

class PlayerPhysicsCompoonent : public PhysicsComponent
{
	public:
		virtual void update(Actor &actor, World &world)
		{

		}
};

class InputComponent
{
	public:
		virtual void update() = 0;
};

class PlayerInputComponent : public InputComponent
{
	public:
		virtual void update()
		{

		}
};
#endif // !ACTOR_H