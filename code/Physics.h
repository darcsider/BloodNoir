#pragma once
/*=====================================================================================
$File: Physics.h
$Date: December 27, 2016
$Creator: Jamie Cooper
$Notice: (C) Copyright 2015 by Punch Drunk Squirrel Games LLC. All Rights Reserved.
=====================================================================================*/
#ifndef PHYSICS_H
#define PHYSICS_H
#include "Includes.h"

class Physics
{
	protected:

	public:
		bool CollissionCheck(SimpleMath::Rectangle object1, SimpleMath::Rectangle object2);
		float DistanceBetweenCenter(SimpleMath::Rectangle object1, SimpleMath::Rectangle object2);
};
#endif // !PHYSICS_H