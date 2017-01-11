/*=====================================================================================
$File: Components.cpp
$Date: January 5, 2017
$Creator: Jamie Cooper
$Notice: (C) Copyright 2015 by Punch Drunk Squirrel Games LLC. All Rights Reserved.
=====================================================================================*/
#include "Components.h"

void GraphicsComponent::update(Actor *actor, RenderManager *graphics)
{
	graphics->RenderObject(actor->GetActorSprite()->GetSpriteTexture(), actor->GetActorSprite()->GetSpriteRectangle(), actor->GetActorPosition());
}

void PhysicsComponent::update(Actor *actor, World *world)
{

}

void PlayerInputComponent::update(Actor *actor, bool pressed, GameActions action)
{
	Vector2 velocity;
	if (pressed && action == DirectionMoveLeft)
		velocity.x = -1;
	else if (pressed && action == DirectionMoveRight)
		velocity.x = 1;
	else if (pressed && action == DirectionMoveUp)
		velocity.y = -1;
	else if (pressed && action == DirectionMoveDown)
		velocity.y = 1;
	else
	{
		velocity.x = 0;
		velocity.y = 0;
	}

	actor->SetActorVelocity(velocity);
}