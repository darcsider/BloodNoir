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
#include "Item.h"

class World;			// foward delcare World class
class GlobalWorld;		// forward declare Global World class

// enumeration that defines the types of SpawnPoints
enum SpawnPointType
{
	SpawnNone = 0,			// for testing purposes
	SpawnPlayer	= 1,		// can be used to spawn the player Actor at this location
	SpawnPlayerStart = 2,	// this is a player spawn point that defines the first location to start at
	SpawnNPC = 3,			// can be used to spawn a NPC Actor at this location
	SpawnEnemy = 4,			// can be used to spawn an Enemy Actor at this location
	SpawnBoss = 5			// can be used to spawn a Boss Actor at this location
};

// enumeration that defines the types of Triggers
enum TriggerType
{
	TriggerNone = 0,			// Default value of triggers if not set
	WorldToWorld = 1,			// Move from one world to another world
	SectionToBuilding = 2,		// Move from a WorldSection to a Building
	BuildingToSection = 3,		// Move from a Building to a WorldSection
	SectionToAlley = 4,			// Move from a WorldSection to an Alley
	AlleyToSection = 5,			// Move from an Alley to a WorldSection
	AlleyToBuilding = 6,		// Move from an Alley to a Building
	BuildingFloorToFloor = 7,	// Move from one Building floor to another
	TutorialTrigger = 8,		// Tutorial event to be fired
	CutScene = 9				// CutScene event to be fired
};

class SpawnPoint
{
	protected:
		SpawnPointType	m_pointType;	// what type of spawn point is this
		Vector2			m_spawnPoint;	// spawn point world coordinate

	public:
		// constructor does nothing right now
		SpawnPoint() { }
		// destructor does nothing right now
		~SpawnPoint() { }
		// build spawn point
		void BuildSpawnPoint(SpawnPointType type, Vector2 point)
		{
			m_pointType = type;
			m_spawnPoint = point;
		}
		// get the type of spawn point this one is
		SpawnPointType GetSpawnPointType() { return m_pointType; }
		// set the type of spawn point this one is
		void SetSpawnPointType(SpawnPointType type) { m_pointType = type; }
		// get the spawn point world location
		Vector2 GetSpawnPointWorld() { return m_spawnPoint; }
		// set the spawn point world location
		void SetSpawnPointWorld(Vector2 point) { m_spawnPoint = point; }
};

// may become a full class as Item's get more complicated as I build the class for them
class Object
{
	protected:
		string	m_objectID;			// the ID for this object
		Vector2	m_locationPosition;	// what is the world relative position
		Item*	m_item;				// what Item is this object associated with
									// (will be fleshed out more when Item class is written)

	public:
		// Construct the Object from the textfile that is passed in
		void BuildObject(string textFile);
		// get the Object's ID
		string GetObjectID() { return m_objectID; }
		// set the Object's ID
		void SetObjectID(string newID)
		{
			if (!newID.empty())
			{
				m_objectID = newID;
			}
		}
};

// May add more to this potentially adding methods to it also this struct may have to move
// to the global map handler class in the future so that moving from world to world is easier
// by passing the trigger back up the chain
class Trigger
{
	protected:
		TriggerType		m_triggerType;			// what type of trigger is this
		string			m_worldID;				// what is the ID for the world this trigger starts out on
		string			m_locationID;			// what is the ID for the location(WorldSection, Building, Alley)
		Vector2			m_locationPosition;		// what is the world relative psotion of the beginning location
		string			m_destinationWorldID;	// what is the destindation World ID in case this is a move from World To World
		string			m_destinationID;		// what is the ID for the destination(WorldSection, Building, Alley)
		Vector2			m_destinationPosition;	// what is the world relative position of the destination location

	public:
		// Construct the Trigger from the textfile that is passed in
		void BuildTrigger(string textFile);
		// get the TriggerType
		TriggerType GetTriggerType() { return m_triggerType; }
		// set the TriggerType
		void SetTriggerType(TriggerType newType) { m_triggerType = newType; }
		// get the starting WorldID
		string GetStartWorldID() { return m_worldID; }
		// set the starting WorldID
		void SetStartWorldID(string newID)
		{
			if (!newID.empty())
			{
				m_worldID = newID;
			}
		}
		// get the starting locationID(WorldSection, Building, Alley)
		string GetStartLocationID() { return m_locationID; }
		// set the starting locationID(WorldSection, Building, Alley)
		void SetStartingLocationID(string newID)
		{
			if (!newID.empty())
			{
				m_locationID = newID;
			}
		}
		// get the starting location
		Vector2 GetStartingLocation() { return m_locationPosition; }
		// set the starting location
		void SetStartingLocation(Vector2 position) { m_locationPosition = position; }
		// get the destination WorldID
		string GetDestinationWorldID() { return m_destinationWorldID; }
		// set the destination WorldID
		void SetDestinationWorldID(string newID)
		{
			if (!newID.empty())
			{
				m_destinationWorldID = newID;
			}
		}
		// get the destination locationID(WorldSection, Building, Alley)
		string GetDestinationLocationID() { return m_destinationID; }
		// set the destination locationID(WorldSection, Building, Alley)
		void SetDestinationLocationID(string newID)
		{
			if (!newID.empty())
			{
				m_destinationID = newID;
			}
		}
		// get the destination location
		Vector2 GetDestinationLocation() { return m_destinationPosition; }
		// set the destination location
		void SetDestinationLocation(Vector2 position) { m_destinationPosition = position; }
};

class Layer
{
	protected:
		string	m_layerID;					// the ID of this layer
		string	m_textureName;				// the name of the texture for the layer
		Vector2 m_layerDimensions;			// Dimensions for the layer x=width y=height
		Vector2 m_currentPosition;			// the current position to start grabbing from texture
		Vector2	m_layerVelocity;			// the velocity that the layer should be moving at
		RECT	m_sourceRectangle;			// rectangle that defines what part of texture to draw to screen
		RECT	m_destinationRectangle;		// rectangle that defines where to draw the layer to the screen
		bool	m_autoScroll;				// does this layer scroll on its own?
		int		m_layerSpeed;				// what speed should the layer move at

	public:
		// get the ID for the current layer
		string GetLayerID() { return m_layerID; }
		// set the ID for the current layer
		void SetLayerID(string newID)
		{
			if (!newID.empty())
			{
				m_layerID = newID;
			}
		}
		// get the texture name for the tilemap
		string GetTextureName() { return m_textureName; }
		// set the texture name for the tilemap
		void SetTextureName(string texture)
		{
			if (!texture.empty())
			{
				m_textureName = texture;
			}
		}
		// get the dimensions of the layer x=width y=height
		Vector2 GetLayerDimensions() { return m_layerDimensions; }
		// set the dimensions of the layer
		void SetLayerDimensions(Vector2 dimensions) { m_layerDimensions = dimensions; }
		// get the current position to draw at for the layer
		Vector2 GetLayerPosition() { return m_currentPosition; }
		// set the current position to draw at for the layer
		void SetLayerPosition(Vector2 position) { m_currentPosition = position; }
		// get the velocity for the layer
		Vector2 GetLayerVelocity() { return m_layerVelocity; }
		// set the velocity for the layer
		void SetLayerVelocity(Vector2 velocity) { m_layerVelocity = velocity; }
		// get the sourceRectangle for the layer
		RECT GetSourceRectangle() { return m_sourceRectangle; }
		// set the sourceRectangle for the layer
		void SetSourceRectangle(RECT source) { m_sourceRectangle = source; }
		// get the sourceRectangle for the layer
		RECT GetDestinationRectangle() { return m_destinationRectangle; }
		// set the sourceRectangle for the layer
		void SetDestinationRectangle(RECT source) { m_destinationRectangle = source; }
		// get the autoScroll setting
		bool GetAutoScroll() { return m_autoScroll; }
		// set the autoScroll setting
		void SetAutoScroll(bool scroll) { m_autoScroll = scroll; }
		// get the layers scroll speed
		int GetScrollSpeed() { return m_layerSpeed; }
		// set the layers scroll speed
		void SetScrollSpeed(int speed)
		{
			if (speed > 0)
			{
				m_layerSpeed = speed;
			}
		}
};

class BuildingFloor
{
	protected:
		int					m_floorNumber;		// what is the floor Number
		string				m_buildingID;		// what is the ID for the building this Floor belongs to
		vector<Layer>		m_floorLayers;		// vector of Layers for the Floor
		vector<Object>		m_floorObjects;		// vector of Objects that are placed on this floor
		vector<Trigger>		m_floorTriggers;	// vector of Triggers for the current floor
		vector<int>			m_collissionMap;	// vector of Integer's that defines moveable area
		vector<SpawnPoint>	m_spawnPoints;		// vector of SpawnPoint's that define where actors can be spawned

	public:
		// constructor currentrly does nothing
		BuildingFloor();
		// destructor clears data
		~BuildingFloor();
		// Build the floor from text file
		void BuildFloor(int floorNumber, string textFile);
		// get the floorNumber for the current floor
		int GetFloorNumber() { return m_floorNumber; }
		// set the floorNumber for the current floor
		void SetFloorNumber(int floor)
		{
			if (floor > -1)
			{
				m_floorNumber = floor;
			}
		}
		// get the buildingID for the current floor
		string GetBuildingID() { return m_buildingID; }
		// set the buildingID for the current floor
		void SetBuildingID(string newID)
		{
			if (!newID.empty())
			{
				m_buildingID = newID;
			}
		}
		// add Layer to FloorLayers
		void AddLayerToFloor(Layer newLayer)
		{
			m_floorLayers.push_back(newLayer);
		}
		// add Object to FloorObjects
		void AddObjectToFloor(Object newObject)
		{
			m_floorObjects.push_back(newObject);
		}
		// add Trigger to FloorTriggers
		void AddTriggerToFloor(Trigger newTrigger)
		{
			m_floorTriggers.push_back(newTrigger);
		}
		// get the value stored at index location in collission Map
		int CollissionMapValue(int index)
		{
			return m_collissionMap.at(index);
		}
		// get the spawn point at the location
		SpawnPoint GetSpawnPoint(Vector2 position);
		// get the player start spawn point location
		Vector2 GetPlayerStartSpawnPoint();
		// add a new SpawnPoint to SpawnPoints
		void AddSpawnPoint(SpawnPoint spawnPoint)
		{
			m_spawnPoints.push_back(spawnPoint);
		}
		// update the floor
		void UpdateFloor(float timeDelta);
		// render the floor
		void RenderFloor();
		// chage the velocity of the active area Section, Building or Alley
		void ChangeVelocity(Vector2 velocity);
		// Check the current location to see if the player can move onto it
		bool CheckPointForCollission(Vector2 position);
		// Check the current location to see if there is a trigger at that location
		Trigger CheckPointForTrigger(Vector2 position);
};

class Alley
{
	protected:
		string				m_AlleyID;			// the ID for the Alley way
		string				m_sectionID;		// what WorldSection is this Alley attached to
		string				m_buildingID;		// what building is this Alley attached to for entering
		World*				m_currentWorld;		// the World that the Alley is attached to for changeing locations
		vector<Layer>		m_alleyLayers;		// vector of Layers for the Alley
		vector<Object>		m_alleyObjects;		// vector of Objects found in this Alley
		vector<Trigger>		m_alleyTriggers;	// vector of Triggers found in this Alley
		bool				m_isActive;			// bool that tells if the player is currently in an Alley
		vector<int>			m_collissionMap;	// vector of Integer's that defines moveable area
		vector<SpawnPoint>	m_spawnPoints;		// vector of SpawnPoint's that define where actors can be spawned

	public:
		// Constructor for Alley no logic done here
		Alley();
		// Destructor for Alley
		~Alley();
		// get the Alley's ID
		string GetAlleyID() { return m_AlleyID; }
		// set the Alley's ID
		void SetAlleyID(string newID)
		{
			if (!newID.empty())
			{
				m_AlleyID = newID;
			}
		}
		// get the Section ID that the Alley is attached to
		string GetSectionID() { return m_sectionID; }
		// set the Section ID that the Alley is attached to
		void SetSectionID(string newID)
		{
			if (!newID.empty())
			{
				m_sectionID = newID;
			}
		}
		// get the Building that the Alley is also attached to
		string GetAlleyBuildingID() { return m_buildingID; }
		// set the Building that the Alley is attached to
		void SetBuildingID(string newID)
		{
			if (!newID.empty())
			{
				m_buildingID = newID;
			}
		}
		// get the value for is this Alley currently Active
		bool GetIsActive() { return m_isActive; }
		// set the value for is this Alley currently Active
		void SetIsActive(bool active) { m_isActive = active; }
		// get the value stored at index location in collission Map
		int CollissionMapValue(int index)
		{
			return m_collissionMap.at(index);
		}
		// get the spawn point at the location
		SpawnPoint GetSpawnPoint(Vector2 position);
		// get the player start spawn point location
		Vector2 GetPlayerStartSpawnPoint();
		// add a new SpawnPoint to SpawnPoints
		void AddSpawnPoint(SpawnPoint spawnPoint)
		{
			m_spawnPoints.push_back(spawnPoint);
		}
		// Build the Alley from the data pulled from the textfile
		void BuildAlley(string textFile, World *world);
		// Update the Alley based on the change in timeDelta animate things, and move the alley if motion
		void UpdateAlley(float timeDelta);
		// Display the Alley based on current values
		void RenderAlley();
		// Check the current location to see if the player can move onto it
		bool CheckPointForCollission(Vector2 position);
		// Check the current location to see if there is a trigger at that location
		Trigger CheckPointForTrigger(Vector2 position);
		// chage the velocity of the active area Section, Building or Alley
		void ChangeVelocity(Vector2 velocity);
};


class Building
{
	protected:
		string					 m_buildingID;		// what is the ID for this instance of a Building
		string					 m_sectionID;		// what is the ID for the WorldSection this building belongs to
		World*					 m_currentWorld;	// pointer for the current World used for handling triggers
		map<int, BuildingFloor*> m_buildingFloors;	// map of the floors for the building
		BuildingFloor*			 m_currentFloor;	// the current floor of the building being rendered and updated
		bool					 m_isActive;		// bool that tells if the player is currently in a Building

	public:
		// Constructor for the building no logic happens here
		Building();
		// Destructor for the Building
		~Building();
		// get the BuildingID for the current Building
		string GetBuildingID() { return m_buildingID; }
		// set the BuildingID for the current Building
		void SetBuildingID(string newID)
		{
			if (!newID.empty())
			{
				m_buildingID = newID;
			}
		}
		// get the SectionID that the Building belongs to
		string GetSectionID() { return m_sectionID; }
		// set the SectionID that the Building belongs to
		void SetSectionID(string newID)
		{
			if (!newID.empty())
			{
				m_sectionID = newID;
			}
		}
		// get the value for is this Building currently Active
		bool GetIsActive() { return m_isActive; }
		// set the value for is this Building currently Active
		void SetIsActive(bool active) { m_isActive = active; }
		// build the Building from the data pulled from the textFile
		void BuildBuilding(string textFile, World *world);
		// update the current Building based on change in timeDelta
		void UpdateBuilding(float timeDelta);
		// render the current Building in it's current state
		void RenderBuilding();
		// get the current floor of the Building
		BuildingFloor* GetCurrentFloor() { return m_currentFloor; }
		// set the current floor of the Building
		void SetCurrentFloor(int floorID);
		// get the player start spawn point location
		Vector2 GetPlayerStartSpawnPoint();
		// Check the current location to see if the player can move onto it
		bool CheckPointForCollission(Vector2 position);
		// Check the current location to see if there is a trigger at that location
		Trigger CheckPointForTrigger(Vector2 position);
		// chage the velocity of the active area Section, Building or Alley
		void ChangeVelocity(Vector2 velocity);
};

class WorldSection
{
	protected:
		string				m_sectionID;		// what is the ID for this WorldSection
		string				m_worldID;			// what is the ID for the World this WorldSection belongs to
		World*				m_currentWorld;		// pointer for the current World used for handling Triggers
		vector<Layer>		m_sectionLayers;	// vector of Layers for the current Section
		vector<Object>		m_sectionObjects;	// vector of Objects for the current Section
		vector<Trigger>		m_sectionTriggers;	// vector of Triggers for the current Section
		bool				m_isActive;			// bool that tells if the player is currently in a WorldSection
		vector<int>			m_collissionMap;	// vector of Integer's that defines moveable area
		vector<SpawnPoint>	m_spawnPoints;		// vector of SpawnPoint's that define where actors can be spawned

	public:
		// Constructor for the WorldSection no logic happens here
		WorldSection();
		// Destructor for the WorldSection
		~WorldSection();
		// get the ID for the current WorldSection
		string GetSectionID() { return m_sectionID; }
		// set the ID for the current WorldSection
		void SetSectionID(string newID)
		{
			if (!newID.empty())
			{
				m_sectionID = newID;
			}
		}
		// get the ID for the World for the current WorldSection
		string GetWorldID() { return m_worldID; }
		// set the ID for the World for the current WorldSection
		void SetWorldID(string newID)
		{
			if (!newID.empty())
			{
				m_worldID = newID;
			}
		}
		// get the value for is this WorldSection currently Active
		bool GetIsActive() { return m_isActive; }
		// set the value for is this WorldSection currently Active
		void SetIsActive(bool active) { m_isActive = active; }
		// get the value stored at index location in collission Map
		int CollissionMapValue(int index)
		{
			return m_collissionMap.at(index);
		}
		// get the spawn point at the location
		SpawnPoint GetSpawnPoint(Vector2 position);
		// get the player start spawn point location
		Vector2 GetPlayerStartSpawnPoint();
		// add a new SpawnPoint to SpawnPoints
		void AddSpawnPoint(SpawnPoint spawnPoint)
		{
			m_spawnPoints.push_back(spawnPoint);
		}
		// Build the current WorldSection based on the data pulled from textFile
		void BuildSection(string textFile, World *world);
		// if the WorldSection is currently Active then update based on change to deltaTime
		void UpdateSection(float timeDelta);
		// render the current WorldSection in its present state
		void RenderSection();
		// Check the current location to see if the player can move onto it
		bool CheckPointForCollission(Vector2 position);
		// Check the current location to see if there is a trigger at that location
		Trigger CheckPointForTrigger(Vector2 position);
		// chage the velocity of the active area Section, Building or Alley
		void ChangeVelocity(Vector2 velocity);
};

class World
{
	protected:
		string						m_worldID;				// the ID for the current instance of a World
		string						m_worldTextFile;		// what is the textFile for constructing this World
		map<string, WorldSection*>	m_worldSections;		// map of WorldSections keyed to their SectionID
		map<string, Building*>		m_worldBuildings;		// map of Buildings keyed to their BuildingID
		map<string, Alley*>			m_worldAlleys;			// map of Alleys keyed to their AlleyID
		WorldSection*				m_currentWorldSection;	// what is the current WorldSection
		Building*					m_currentBuilding;		// what is the current Building
		Alley*						m_currentAlley;			// what is the current Alley
		GlobalWorld*				m_GLWorld;				// pointer to GlobalWorld Manger to change
															// the World based on Triggers

	public:
		// Constructor for the current World instance no logic in here
		World();
		// Destructor for the current World
		~World();
		// get the current Worlds ID
		string GetWorldID() { return m_worldID; }
		// set the current Worlds ID
		void SetWorldID(string newID)
		{
			if (!newID.empty())
			{
				m_worldID = newID;
			}
		}
		// get the player start spawn point location
		Vector2 GetPlayerStartSpawnPoint();
		// Build the World and all its Sections, Buildings, and Alleys
		void BuildWorld(string textFile, GlobalWorld* GLWorld);
		// Update the World and the current Section, Building, or Alley
		void UpdateWorld(float timeDelta);
		// Render the World and the current Section, Building, or Alley
		void RenderWorld();
		// change the current Section to the SectionID passed in
		void SetCurrentSection(string sectionID);
		// change the current Building to the BuildingID passed in
		void SetCurrentBuilding(string buildingID);
		// change the current Alley to the AlleyID passed in
		void SetCurrentAlley(string alleyID);
		// chage the velocity of the active area Section, Building or Alley
		void ChangeVelocity(Vector2 velocity);
		// method for handling all Triggers if its a change to a Section, Building or Alley it is
		// handled internally if not then it is passed up the chain of command and possibly to an
		// event handler
		void TriggerEventHandling(Trigger trigger);
		// Check the current location to see if the player can move onto it
		bool CheckPointForCollission(Vector2 position);
		// Check the current location to see if there is a trigger at that location
		Trigger CheckPointForTrigger(Vector2 position);
		// here only for testing purposes will NOT BE STAYING
		void CloseGame(bool pressed, GameActions action)
		{
			if (pressed)
				PostQuitMessage(0);
		}
};

class GlobalWorld
{
	protected:
		map<string, World*>	m_gameWorld;		// map of all the Worlds for the game
		World*				m_currentWorld;		// what is the current World for the game

	public:
		// Constructor for the GlobalWorld Manager no logic in here
		GlobalWorld();
		// Destructor for the GlobalWorld Manager
		~GlobalWorld();
		// get the currentWorld
		World* GetCurrentWorld() { return m_currentWorld; }
		// Build all the worlds for the game based on data from textFile
		void BuildWorlds(string textFile);
		// Change the current World
		void ChangeWorld(string worldID);
		// Update the currentWorld
		void UpdateWorld(float timeDelta);
		// Render the currentWorld
		void RenderWorld();
};
#endif //!WORLD_H