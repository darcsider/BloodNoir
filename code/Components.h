#pragma once
/*=====================================================================================
$File: Components.h
$Date: January 5, 2017
$Creator: Jamie Cooper
$Notice: (C) Copyright 2015 by Punch Drunk Squirrel Games LLC. All Rights Reserved.
=====================================================================================*/
#ifndef COMPONENTS_H
#define COMPONENTS_H
#include "Includes.h"
#include "Actor.h"
#include "World.h"
#include "RenderManager.h"
#include "InputManager.h"

class GraphicsComponent
{
	protected:

	public:
		void update(Actor *actor, RenderManager *graphics);
};

class PhysicsComponent
{
	protected:
		SimpleMath::Rectangle m_rectangle;

	public:
		void update(Actor *actor, World *world);
};

class InputComponent
{
	protected:

	public:
		virtual ~InputComponent() {}
		virtual void update(Actor *actor, bool pressed, GameActions action) = 0;
};

class PlayerInputComponent : public InputComponent
{
	protected:

	public:
		virtual void update(Actor *actor, bool pressed, GameActions action);
};
#endif // !COMPONENTS_H