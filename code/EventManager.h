#pragma once
/*=====================================================================================
$File: EventManager.h
$Date: May 15, 2016
$Creator: Jamie Cooper
$Notice: (C) Copyright 2015 by Punch Drunk Squirrel Games LLC. All Rights Reserved.
=====================================================================================*/
#ifndef EVENTMANAGER_H
#define EVENTMANAGER_H
#include "Includes.h"
#include "GameTiming.h"
#include "DX11RenderManager.h"

class Event
{
	protected:
		int m_eventID;

	public:
		virtual ~Event() { }
		virtual void SetCallBack(function<void()> funcPoint) = 0;
		virtual void ExecuteCallBack() = 0;
		virtual bool Execute() = 0;
};

class TimedEvent : public Event
{
	protected:
		double m_timeLimit;
		GameTiming m_timer;
		double m_currentTime;

	public:
		function<void()> callBack;
		TimedEvent() {}
		TimedEvent(int id, double time);
		~TimedEvent() {}
		virtual bool Execute();

		void SetCallBack(function<void()> funcPoint)
		{
			callBack = funcPoint;
		}

		void ExecuteCallBack()
		{
			callBack();
		}
};

class EventManager
{
	protected:
		vector<Event*> m_events;
		static int m_eventID;
	
	public:
		EventManager() 
		{
		}

		~EventManager() {}

		void ProcessEvents();
		void AddEvent(Event *event);
		void AddEvent(double timeLimit);
};
#endif