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
		Sprite m_sprite;

		string m_characterName;
		int m_hitPoints;
		float m_actorSpeed;
		Vector2 m_velocity;
		Vector2 m_position;

	public:
		Actor();
		Actor(RenderManager *graphics, InputManager *input);
		~Actor();
		void BuildActor(int hp, float speed);
		void MoveActor(MoveDirection direction = NotMoving);
		void UpdateActor(float deltaTime);
		void UpdateActorVelocity(int value);
		void DrawActor();

		void MoveActorLeft(bool pressed);
		void MoveActorRight(bool pressed);
		void MoveActorUp(bool pressed);
		void MoveActorDown(bool pressed);
};
#endif // !ACTOR_H