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
		World *m_currentWorld;

		GraphicsComponent *m_graphics;
		PhysicsComponent *m_physics;
		InputComponent *m_input;

	public:
		Actor();
		~Actor();
		void BuildActor(int hp, int attack, int defense, float speed, GraphicsComponent *graphics, PhysicsComponent *physics, InputComponent *input, Sprite *sprite);
		void Update(float deltaTime);
		void DrawActor();
		void MoveActor(bool pressed, GameActions action);

		void SetActorDirection(MoveDirection direction = NotMoving);
		void SetActorCurrentWorld(World *world);
		Sprite* GetActorSprite();
		Vector2 GetActorPosition();
		void SetActorPosition(Vector2 position);
		Vector2 GetActorVelocity();
		void SetActorVelocity(Vector2 velocity);
		float GetActorMovementSpeed();
		void SetActorMovementSpeed(float speed);
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
		virtual void update(Actor &actor, float timeDelta, World &world) = 0;
};

class PlayerPhysicsCompoonent : public PhysicsComponent
{
	public:
		virtual void update(Actor &actor, float timeDelta, World &world)
		{
			Vector2 velocity = actor.GetActorVelocity();
			Vector2 position = actor.GetActorPosition();
			position.x = velocity.x * timeDelta;
			position.y = velocity.y * timeDelta;
			position = world.CheckCollission(position, velocity, actor.GetActorMovementSpeed());
			actor.SetActorPosition(position);
		}
};

class InputComponent
{
	public:
		virtual void update(Actor &actor, bool pressed, GameActions action) = 0;
};

class PlayerInputComponent : public InputComponent
{
	public:
		virtual void update(Actor &actor, bool pressed, GameActions action)
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
						actor.SetActorVelocity(Vector2(-1, actor.GetActorVelocity().y));
						actor.SetActorDirection(MoveLeft);
						break;
					case DirectionMoveRight:
						actor.SetActorVelocity(Vector2(1, actor.GetActorVelocity().y));
						actor.SetActorDirection(MoveRight);
						break;
					case DirectionMoveUp:
						actor.SetActorVelocity(Vector2(actor.GetActorVelocity().x, -1));
						actor.SetActorDirection(MoveUp);
						break;
					case DirectionMoveDown:
						actor.SetActorVelocity(Vector2(actor.GetActorVelocity().x, 1));
						actor.SetActorDirection(MoveDown);
						break;
					case SystemTest:
						break;
					case SystemExitEarly:
						break;
					case SystemConsole:
						break;
					default:
						actor.SetActorVelocity(Vector2(0, 0));
						actor.SetActorDirection(NotMoving);
						break;
				}
			}
		}
};
#endif // !ACTOR_H