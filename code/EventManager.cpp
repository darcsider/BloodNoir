/*=====================================================================================
$File: EventManager.cpp
$Date: May 15, 2016
$Creator: Jamie Cooper
$Notice: (C) Copyright 2015 by Punch Drunk Squirrel Games LLC. All Rights Reserved.
=====================================================================================*/
#include "EventManager.h"

TimedEvent::TimedEvent(int id, double time)
{
	m_eventID = id;
	m_timeLimit = time;
	m_timer.Reset();
}

bool TimedEvent::Execute()
{
	m_timer.Tick();
	
	m_currentTime += m_timer.DeltaTime();

	if (m_currentTime >= m_timeLimit)
		return true;

	return false;
}

void EventManager::ProcessEvents()
{
	for (int i = 0; i < m_events.size(); i++)
	{
		if (m_events[i]->Execute())
		{
			m_events[i]->ExecuteCallBack();
			m_events.erase(m_events.begin() + i);
		}
	}
}

void EventManager::AddEvent(Event *event)
{
	m_events.push_back(event);
}

void EventManager::AddEvent(double timeLimit)
{
	TimedEvent *tempEvent = new TimedEvent(1, timeLimit);

	m_events.push_back(tempEvent);
}