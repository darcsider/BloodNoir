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
	std::string m_name;
	std::string m_textureName;
	int m_tileIndex;
	Vector2 m_position;
	ObjectType m_objectType;
};

// a structure defining trigger points on a map, the trigger points define
// points on the map or a building that cause an event to happen in the game.
// The TriggerPoint m_name variable will hold the text file name if the trigger point
// is a building trigger.
struct TriggerPoint
{
	std::string m_name;
	TriggerType m_triggerType;
	Vector2 m_mapPosition;
	Vector2 m_moveToPosition;
};

// Map Layer defines the background layers of a map section, these are defined by pre baked images
// that are scrolled either automatically or when the player moves the main character.
struct SectionLayer
{
	std::string m_textureName;
	float m_scrollSpeed;
	bool m_autoScroll;
	int m_width;
	int m_height;
	RECT m_sourceRectangle;
};

// Building Floor is all the data related to a specific layer of a building
// the data is a texture name that defines which texture holds the tiles within the map of textures
// a vector of int values that define which tiles to use at each position on the screen
// the number of columns and rows that make up a specific tile map on the screen
struct BuildingFloor
{
	int m_columns = 0;
	int m_rows = 0;
	std::vector<int> m_tileSet;
	std::vector<Object> m_floorObjects;
	std::vector<TriggerPoint> m_floorTriggers;
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
		std::string m_textureName;
		std::vector<BuildingFloor> m_floors;
		int m_numberOfFloors;
		int m_floorNumber;
		
		Building()
		{ }
		~Building()
		{ }

		void BuildBuilding(std::string fileName);
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
		DX11RenderManager *m_graphicSystem;
		int m_numberOfLayers;
		int m_numberOfObjects;
		int m_numberOfTriggers;
		std::vector<SectionLayer> m_layers;
		std::vector<Object> m_objects;
		std::vector<TriggerPoint> m_triggerPoints;
		std::vector<Building> m_buildings;
		
	public:
		MapSection()
		{ }
		~MapSection()
		{ }

		void BuildMapSection(DX11RenderManager *graphics, std::string fileName);
		void DrawMapSection();
		RECT UpdateMapSection(float delta, bool left, bool keyboardEntry);
};

// This is the map class it defines a section of the city that the player
// can search through and discover various enemies, bosses, and other stuff
// it encompasses a certain number of map sections, which encompass a set
// of buildings, and alleys for certain buildings.
class Map
{
	protected:
		DX11RenderManager *m_graphicSystem;
		Keyboard *m_keyboard;
		GamePad *m_gamePad;
		std::string m_mapName;
		std::vector<MapSection> m_mapSections;
		std::vector<std::string> m_sectionNames;
		RECT m_currentPosition;
		MapSection m_currentSection;
		int m_numberOfMapSections;
	
	public:
		Map();
		~Map();

		void InitializeMap(DX11RenderManager *graphics, Keyboard *keyboard, GamePad *gamePad, int screenWidth, int screenHeight);
		void BuildMap(std::string mapTextFile);
		void UpdateMap(float timeDelta);
		void DrawMap();
		void SetCurrentMapSection(int mapSection);
};
#endif