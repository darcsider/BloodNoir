#pragma once
/*=====================================================================================
$File: World.h
$Date: February 8, 2016
$Creator: Jamie Cooper
$Notice: (C) Copyright 2015 by Punch Drunk Squirrel Games LLC. All Rights Reserved.
=====================================================================================*/
#ifndef	WORLD_H
#define WORLD_H
#include "Includes.h"
#include "RenderManager.h"
#include "InputManager.h"
#include <tinyxml2.h>

enum TriggerType
{
	TriggerNone = 0,
	WorldToWorld = 1,
	WorldToBuilding = 2,
	BuildingToWorld = 3,
	BuildingFloorToFloor = 4,
	TutorialTrigger = 5,
	CutScene = 6
};

struct Object
{
	string		m_name;
	string		m_textureName;
	int			m_tileIndex;
	Vector2		m_position;
	//ObjectType	m_objectType;
};

struct TriggerPoint
{
	string			m_name;
	TriggerType		m_triggerType;
	Vector2			m_mapPosition;
	Vector2			m_moveToPosition;
};

struct SectionLayer
{
	string					m_textureName;
	double					m_scrollSpeed;
	double					m_velocity;
	bool					m_autoScroll;
	int						m_width;
	int						m_height;
	SimpleMath::Rectangle	m_sourceRectangle;
	int GetWidth()
	{
		return m_width;
	}

	int GetHeight()
	{
		return m_height;
	}
};

struct BuildingFloor
{
	int						m_columns;
	int						m_rows;
	vector<int>				m_tileSet;
	vector<Object>			m_floorObjects;
	vector<TriggerPoint>	m_floorTriggers;
};

class Building
{
	public:
		string					m_textureName;
		vector<BuildingFloor>	m_floors;
		int						m_numberOfFloors;
		int						m_floorNumber;

		Building()
		{ }
		~Building()
		{ }

		void BuildBuilding(string fileName);
		void DrawBuilding();
};

class WorldSection
{
	protected:
		int						m_numberOfLayers;
		int						m_numberOfObjects;
		int						m_numberOfTriggers;
		vector<SectionLayer>	m_layers;
		vector<Object>			m_objects;
		vector<TriggerPoint>	m_triggerPoints;
		vector<Building>		m_buildings;

	public:
		WorldSection()
		{ }
		~WorldSection()
		{ }

		void UpdateVelocity(int value);
		void BuildWorldSection(string fileName);
		void DrawWorldSection();
		void UpdateWorldSection(float delta);
		TriggerType CheckCollision(Vector2 position);
};

class World
{
	protected:
		string					m_worldName;
		vector<WorldSection>	m_worldSections;
		WorldSection			m_currentSection;
		string					m_worldFileName;

	public:
		World();
		~World();
		void InitializeWorld(string worldTextFile);
		void BuildWorld();
		void UpdateWorld(float timeDelta);
		void DrawWorld();
		void SetCurrentWorldSection(int worldSection);
		void MoveWorld(bool move, GameActions action);
		Vector2 CheckCollission(Vector2 position, Vector2 velocity, float movementSpeed);
		TriggerType CheckTriggerCollision(Vector2 position);

		void CloseGame(bool pressed, GameActions action)
		{
			if (pressed)
				PostQuitMessage(0);
		}
};
#endif //!WORLD_H