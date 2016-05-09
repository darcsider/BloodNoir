#pragma once
/*=====================================================================================
$File: Map.h
$Date: February 8, 2016
$Creator: Jamie Cooper
$Notice: (C) Copyright 2015 by Punch Drunk Squirrel Games LLC. All Rights Reserved.
=====================================================================================*/
#ifndef MAP_H
#define MAP_H
#include "Includes.h"
#include "DX11RenderManager.h"
#include "WinInput.h"
#pragma warning (disable : 4244)
#pragma warning (disable : 4018)

// The various object types available in the game
// Money used to travel faster and potentially other upgrades
// Lore Piece's progression and achievements are tied to this
// Upgrade pieces that will be used for upgrades in the game
enum ObjectType
{
	Money,
	LorePiece,
	Upgrade
};

// The various trigger point types available in the game
// Building defines a trigger point that will lead to the inside of a building
// Block defines a trigger point that will lead the character to another map section
// Floor will only be used inside of a building where the trigger point will move the
// player up or down a floor in the building.
enum TriggerType
{
	BuildingTrigger,
	BlockTrigger,
	FloorTrigger
};

// The Objects are made up of an object name, the tile index that defines
// the image that represents the actual object on screen
// the objects position on the screen and the object type
struct Object
{
	string		m_name;
	string		m_textureName;
	int			m_tileIndex;
	Vector2		m_position;
	ObjectType	m_objectType;
};

// a structure defining trigger points on a map, the trigger points define
// points on the map or a building that cause an event to happen in the game.
// The TriggerPoint m_name variable will hold the text file name if the trigger point
// is a building trigger.
struct TriggerPoint
{
	string			m_name;
	TriggerType		m_triggerType;
	Vector2			m_mapPosition;
	Vector2			m_moveToPosition;
};

// Map Layer defines the background layers of a map section, these are defined by pre baked images
// that are scrolled either automatically or when the player moves the main character.
struct SectionLayer
{
	string	m_textureName;
	float	m_scrollSpeed;
	float	m_velocity;
	bool	m_autoScroll;
	int		m_width;
	int		m_height;
	RECT	m_sourceRectangle;
};

// Building Floor is all the data related to a specific layer of a building
// the data is a texture name that defines which texture holds the tiles within the map of textures
// a vector of int values that define which tiles to use at each position on the screen
// the number of columns and rows that make up a specific tile map on the screen
struct BuildingFloor
{
	int						m_columns;
	int						m_rows;
	vector<int>				m_tileSet;
	vector<Object>			m_floorObjects;
	vector<TriggerPoint>	m_floorTriggers;
};

// the class defining a building it contains
// vector of tileLayer's to hold all the floors of the building
// vector of TriggerPoints to hold all of the trigger points for all floors
// vector of Objects containing all Objects of the building
// the number of floors the building has
// and the current floor number
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

// the class that defines the sections of a map
// this class used to be called levels because I said that a map
// was made up of 4 levels but that kept getting people confused
// when I called them levels so I have renamed it to MapSection
// The map is setup like a grid like a real city map, the player is
// walking on the street of a specific block of the city they can
// go left and right without loading anything, for the full width
// of the specific "block" if they go up at a street point it will
// send them to the next block north of that current one. They can
// also go down and go to the next block south of the current one.
// however it will be restricted to however many blocks belong to 
// that "map" or section of the city.
class MapSection
{
	protected:
		DX11RenderManager		*m_graphicSystem;
		int						m_numberOfLayers;
		int						m_numberOfObjects;
		int						m_numberOfTriggers;
		vector<SectionLayer>	m_layers;
		vector<Object>			m_objects;
		vector<TriggerPoint>	m_triggerPoints;
		vector<Building>		m_buildings;
		
	public:
		MapSection()
		{ }
		~MapSection()
		{ }

		void UpdateVelocity(int value);
		void BuildMapSection(DX11RenderManager *graphics, string fileName);
		void DrawMapSection();
		RECT UpdateMapSection(float delta);
};

// This is the map class it defines a section of the city that the player
// can search through and discover various enemies, bosses, and other stuff
// it encompasses a certain number of map sections, which encompass a set
// of buildings, and alleys for certain buildings.
class Map
{
	protected:
		DX11RenderManager	*m_graphicSystem;
		WinInput			*m_input;
		string				m_mapName;
		vector<MapSection>	m_mapSections;
		vector<string>		m_sectionNames;
		RECT				m_currentPosition;
		MapSection			m_currentSection;
		int					m_numberOfMapSections;
	
	public:
		Map();
		~Map();

		void InitializeMap(DX11RenderManager *graphics, WinInput *input, int screenWidth, int screenHeight);
		void BuildMap(string mapTextFile);
		void UpdateMap(float timeDelta);
		void DrawMap();
		void SetCurrentMapSection(int mapSection);
		void MoveMapLeft();
		void MoveMapRight();
};
#endif