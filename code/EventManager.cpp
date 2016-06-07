/*=====================================================================================
$File: EventManager.cpp
$Date: May 15, 2016
$Creator: Jamie Cooper
$Notice: (C) Copyright 2015 by Punch Drunk Squirrel Games LLC. All Rights Reserved.
=====================================================================================*/
#include "EventManager.h"

int EventManager::m_eventID = 0;

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
	vector<Event*>::iterator eventIterator;

	for (eventIterator = m_events.begin(); eventIterator != m_events.end(); eventIterator++)
	{
		if ((*eventIterator)->Execute())
		{
			(*eventIterator)->ExecuteCallBack();
			m_events.erase(eventIterator);
		}
	}
}

void EventManager::AddEvent(Event *event)
{
	m_events.push_back(event);
}

void EventManager::AddEvent(double timeLimit)
{
	TimedEvent *tempEvent = new TimedEvent(++m_eventID, timeLimit);
	m_events.push_back(tempEvent);
}