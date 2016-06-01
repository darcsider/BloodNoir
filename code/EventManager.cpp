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

DrawEvent::DrawEvent(int id, string object, Vector2 position, DX11RenderManager *tempGraphics)
{
	m_eventID = id;
	m_drawObject = object;
	m_position = position;
	m_graphics = tempGraphics;
}

DrawEvent::~DrawEvent()
{
	delete m_graphics;
	m_eventID = 0;
	m_drawObject = "";
}

bool DrawEvent::Execute()
{
	m_graphics->DrawObject(m_drawObject, m_position);
	return true;
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

void EventManager::AddEvent(string object)
{
	DrawEvent *drawEvent = new DrawEvent(++m_eventID, object, Vector2(0,0), m_graphicSystem);
	m_events.push_back(drawEvent);
}