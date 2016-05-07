#pragma once
/*=====================================================================================
$File: GameTiming.h
$Date: January 25, 2016
$Creator: Jamie Cooper
$Notice: (C) Copyright 2015 by Punch Drunk Squirrel Games LLC. All Rights Reserved.
=====================================================================================*/
#ifndef GAMETIMING_H
#define GAMETIMING_H
#include "Includes.h"
class GameTiming
{
	public:
		GameTiming();

		float TotalTime()const;  // in seconds
		float DeltaTime()const; // in seconds

		void Reset(); // Call before message loop.
		void Start(); // Call when unpaused.
		void Stop();  // Call when paused.
		void Tick();  // Call every frame.

	private:
		double mSecondsPerCount;
		double mDeltaTime;

		__int64 mBaseTime;
		__int64 mPausedTime;
		__int64 mStopTime;
		__int64 mPrevTime;
		__int64 mCurrTime;

		bool mStopped;
};
#endif