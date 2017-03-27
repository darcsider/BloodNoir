/*=====================================================================================
$File: World.cpp
$Date: February 8, 2016
$Creator: Jamie Cooper
$Notice: (C) Copyright 2015 by Punch Drunk Squirrel Games LLC. All Rights Reserved.
=====================================================================================*/
#include "World.h"

void Object::BuildObject(string textFile)
{
	// TODO build this method and Item class while I'm at it
}

void Trigger::BuildTrigger(string textFile)
{
	string		tempString;				// temp string used in file reading
	ifstream	inFile(textFile);		// file reader variable

	// check to see if the file can be open successfully if not then do nothing
	if (inFile)
	{
		// Read in a line and check the value to see what type of trigger this is
		getline(inFile, tempString);
		if (tempString == "WorldToWorld")
		{
			m_triggerType = WorldToWorld;
		}
		else if (tempString == "SectionToBuilding")
		{
			m_triggerType = SectionToBuilding;
		}
		else if (tempString == "BuildingToSection")
		{
			m_triggerType = BuildingToSection;
		}
		else if (tempString == "SectionToAlley")
		{
			m_triggerType = SectionToAlley;
		}
		else if (tempString == "AlleyToSection")
		{
			m_triggerType = AlleyToSection;
		}
		else if (tempString == "AlleyToBuilding")
		{
			m_triggerType = AlleyToBuilding;
		}
		else if (tempString == "BuildingFloorToFloor")
		{
			m_triggerType = BuildingFloorToFloor;
		}
		else if (tempString == "TutorialTrigger")
		{
			m_triggerType = TutorialTrigger;
		}
		else if (tempString == "CutScene")
		{
			m_triggerType = CutScene;
		}
		// Read in a line and place it into worldID
		getline(inFile, m_worldID);
		// Read in a line and place it into locationID
		getline(inFile, m_locationID);

		// Read in a line and seperate its values and place into location Position
		getline(inFile, tempString);
		{
			istringstream ss(tempString);

			string s2;
			getline(ss, s2, ',');
			m_locationPosition.x = atoi(tempString.c_str());
			getline(ss, s2, ',');
			m_locationPosition.y = atoi(tempString.c_str());
		}

		// Read in a line and place it into destinationWorldID
		getline(inFile, m_destinationWorldID);
		// Read in a line and place it into destinationID
		getline(inFile, m_destinationID);

		// Read in a line and seperate its values and place into destination Position
		getline(inFile, tempString);
		{
			istringstream ss(tempString);

			string s2;
			getline(ss, s2, ',');
			m_destinationPosition.x = atoi(tempString.c_str());
			getline(ss, s2, ',');
			m_destinationPosition.y = atoi(tempString.c_str());
		}
	}
	inFile.close();
}

BuildingFloor::BuildingFloor()
{
	m_floorLayers.clear();
	m_floorObjects.clear();
	m_floorTriggers.clear();
}

BuildingFloor::~BuildingFloor()
{
	m_floorLayers.clear();
	m_floorObjects.clear();
	m_floorTriggers.clear();
}

void BuildingFloor::BuildFloor(int floorNumber, string textFile)
{
	int				numberOfLayers = 0;		// temp variable to hold the number of layers
	int				numberOfObjects = 0;	// temp variable to hold the number of objects
	int				numberOfTriggers = 0;	// temp variable to hold the number of triggers
	int				numberOfSpawns = 0;		// temp variable to hold the number of SpawnPoints
	Layer			tempLayer;				// temp Layer to hold data
	Object			tempObject;				// temp Object to hold data
	Trigger			tempTrigger;			// temp Trigger to hold data
	string			tempString;				// temp String to hold data read in from textFile
	SpawnPoint		tempSpawn;				// temp SpawnPoint to hold a single spawn point
	Vector2			tempPoint;				// temp Vector2 to hold worldSpawnPoint
	ifstream		inFile(textFile);		// file reader variable

	// Set the floor number that is passed in
	SetFloorNumber(floorNumber);

	// check to see if the file can be open successfully if not then do nothing
	if (inFile)
	{
		// Read in a line and convert it to an Integer and place it into numberOfSpawns
		getline(inFile, tempString);
		numberOfSpawns = atoi(tempString.c_str());

		for (int spawnCount = 0; spawnCount < numberOfSpawns; spawnCount++)
		{
			// read a line in and set the SpawnPointType based on its value
			getline(inFile, tempString);
			if (tempString == "SpawnPlayer")
			{
				tempSpawn.SetSpawnPointType(SpawnPlayer);
			}
			if (tempString == "SpawnPlayerStart")
			{
				tempSpawn.SetSpawnPointType(SpawnPlayerStart);
			}
			if (tempString == "SpawnNPC")
			{
				tempSpawn.SetSpawnPointType(SpawnNPC);
			}
			if (tempString == "SpawnEnemy")
			{
				tempSpawn.SetSpawnPointType(SpawnEnemy);
			}
			if (tempString == "SpawnBoss")
			{
				tempSpawn.SetSpawnPointType(SpawnBoss);
			}

			// Read in a line and seperate its values and place into SpawnWorldPosition
			getline(inFile, tempString);
			istringstream ss(tempString);

			string s2;
			getline(ss, s2, ',');
			tempPoint.x = atoi(tempString.c_str());
			getline(ss, s2, ',');
			tempPoint.y = atoi(tempString.c_str());

			tempSpawn.SetSpawnPointWorld(tempPoint);
			// add the spawn point to the container
			AddSpawnPoint(tempSpawn);
		}

		// Read in a line and convert to Integer and place into numberOfLayers
		getline(inFile, tempString);
		numberOfLayers = atoi(tempString.c_str());

		for (int layerCount = 0; layerCount < numberOfLayers; layerCount++)
		{
			// Read in a line and place into layer ID
			getline(inFile, tempString);
			tempLayer.SetLayerID(tempString);
			// Read in a line and place into layer textureName
			getline(inFile, tempString);
			tempLayer.SetTextureName(tempString);
			// Read in a line and check its value and set auto scroll accordingly
			getline(inFile, tempString);
			if (tempString == "TRUE")
			{
				tempLayer.SetAutoScroll(true);
			}
			else
			{
				tempLayer.SetAutoScroll(false);
			}
			// Read in a line and convert to Integer and set it to the layerSpeed
			getline(inFile, tempString);
			tempLayer.SetScrollSpeed(atoi(tempString.c_str()));

			// get the texture desc of the texture for the layer then fill in the texutre width and height with its info
			D3D11_TEXTURE2D_DESC textureDesc = RenderManager::GetInstance().getTextureDesc(tempLayer.GetTextureName());
			tempLayer.SetLayerDimensions(Vector2((float)textureDesc.Width, (float)textureDesc.Height));

			// set starting velocity at 0
			tempLayer.SetLayerVelocity(Vector2(0.0, 0.0));
			// build starting destination RECT and set it
			RECT tempRect;
			tempRect.left = 0;
			tempRect.top = 0;
			tempRect.right = RenderManager::GetInstance().GetGameWidth();
			tempRect.bottom = RenderManager::GetInstance().GetGameHeight();
			tempLayer.SetDestinationRectangle(tempRect);

			// get the player spawn point for the area
			Vector2 position = GetPlayerStartSpawnPoint();
			// Vector2 to hold the planned current starting draw position
			Vector2 currentPosition;
			// set the x coordinate equal to the player spawn point minus 32 pixels to
			// start grabbing from just to the left of the player spawn point
			currentPosition.x = position.x - 32.0;

			// check to see how close the player spawn point is to 0
			// if its not to close to zero then set the y to the player spawn point
			// minus the game height
			if ((position.y - RenderManager::GetInstance().GetGameHeight()) > 0)
			{
				currentPosition.y = position.y - RenderManager::GetInstance().GetGameHeight();
			}
			// if its to close to the top then set to start drawing at 0
			// this would most likely be an issue if the area being rendered is the size of the screen
			else
			{
				currentPosition.y = 0.0;
			}

			// set the draw position for the layer
			tempLayer.SetLayerPosition(currentPosition);

			// calculate the source rectangle
			tempRect.left = currentPosition.x;
			tempRect.top = currentPosition.y;
			tempRect.right = RenderManager::GetInstance().GetGameWidth();
			tempRect.bottom = RenderManager::GetInstance().GetGameHeight();

			// and set the source rectangle for the layer
			tempLayer.SetSourceRectangle(tempRect);

			// add the tempLayer to the Layers vector
			AddLayerToFloor(tempLayer);
		}

		// loop through grabbing line by line of the collission map data
		// and adding it to the collission map vector
		for (int row = 0; row < (m_floorLayers.back().GetLayerDimensions().y / 32); row++)
		{
			getline(inFile, tempString);
			istringstream ss(tempString);

			while (ss)
			{
				string s2;
				if (!getline(ss, s2, ','))
					break;

				m_collissionMap.push_back(atoi(s2.c_str()));
			}
		}

		// Loop through the next several lines grabbing the value
		// passing the value to the BuildObject method and on return
		// adding the Object to the FloorObjects vector
		getline(inFile, tempString);
		numberOfObjects = atoi(tempString.c_str());
		for (int objects = 0; objects < numberOfObjects; objects++)
		{
			getline(inFile, tempString);
			tempObject.BuildObject(tempString);
			AddObjectToFloor(tempObject);
		}
		// Loop through the next several lines grabbing the value
		// passing the value to the BuildObject method and on return
		// adding the Object to the FloorTriggers vector
		getline(inFile, tempString);
		numberOfTriggers = atoi(tempString.c_str());
		for (int triggers = 0; triggers < numberOfTriggers; triggers++)
		{
			getline(inFile, tempString);
			tempTrigger.BuildTrigger(tempString);
			AddTriggerToFloor(tempTrigger);
		}
	}

	// close file when done
	inFile.close();
}

void BuildingFloor::UpdateFloor(float deltaTime)
{
	vector<Layer>::iterator	layerIterator;		// iterator to traverse the vector of layers
	Vector2					currentPosition;	// Vector2 to hold the current draw position

	// loop through the layers of the vector
	for (layerIterator = m_floorLayers.begin(); layerIterator != m_floorLayers.end(); layerIterator++)
	{
		// get the current position used to define where to start grabbing from texture
		currentPosition = layerIterator->GetLayerPosition();
		// check to see if this is an auto scroll layer
		if (!layerIterator->GetAutoScroll())
		{
			// check to see if the current position x value is not to large to grab more if not then increase its value
			if (currentPosition.x + RenderManager::GetInstance().GetGameWidth() < layerIterator->GetLayerDimensions().x)
			{
				currentPosition.x += layerIterator->GetScrollSpeed() * layerIterator->GetLayerVelocity().x * deltaTime;
			}
			// if it is beyond the limit of the layer then max it out to the limits
			if (currentPosition.x + RenderManager::GetInstance().GetGameWidth() > layerIterator->GetLayerDimensions().x)
			{
				currentPosition.x = layerIterator->GetLayerDimensions().x - RenderManager::GetInstance().GetGameWidth();
			}
			// maue sure if scrolling the other direction it doesn't grab from beyond the other side
			if (currentPosition.x < 0)
			{
				currentPosition.x = 0;
			}
			// check to see if the current position x value is not to large to grab more if not then increase its value
			if (currentPosition.y + RenderManager::GetInstance().GetGameHeight() < layerIterator->GetLayerDimensions().y)
			{
				currentPosition.y += layerIterator->GetScrollSpeed() * layerIterator->GetLayerVelocity().y * deltaTime;
			}
			// if it is beyond the limit of the layer then max it out to the limits
			if (currentPosition.y + RenderManager::GetInstance().GetGameHeight() > layerIterator->GetLayerDimensions().y)
			{
				currentPosition.y = layerIterator->GetLayerDimensions().y - RenderManager::GetInstance().GetGameHeight();
			}
			// maue sure if scrolling the other direction it doesn't grab from beyond the other side
			if (currentPosition.y < 0)
			{
				currentPosition.y = 0;
			}
		}
		// if it is an auto scroll layer just update the x position
		else
		{
			currentPosition.x += layerIterator->GetScrollSpeed() * layerIterator->GetLayerVelocity().x * deltaTime;
		}

		layerIterator->SetLayerPosition(currentPosition);
	}
}

void BuildingFloor::RenderFloor()
{
	vector<Layer>::iterator	layerIterator;		// iterator to traverse the vector of layers
	Vector2					position(0.0, 0.0);	// used for calculating the position to draw each tile
	Vector2					currentPosition;	// the current layer's drawPosition
	RECT					sourceRectangle;	// the sourceRectangle for grabbing from the texture
	RECT					destRectangle;		// the destinationRectangle for drawing to a part of the screen

	// loop through the layers of the vector
	for (layerIterator = m_floorLayers.begin(); layerIterator != m_floorLayers.end(); layerIterator++)
	{
		// grab the current draw position for the current layer
		currentPosition = layerIterator->GetLayerPosition();
		// if the layer is not an auto scroll layer
		if (!layerIterator->GetAutoScroll())
		{
			// set the source rectangle equal to startin at the current draw position and grab a full screen's worth
			sourceRectangle.left = currentPosition.x;
			sourceRectangle.top = currentPosition.y;
			sourceRectangle.right = RenderManager::GetInstance().GetGameWidth();
			sourceRectangle.bottom = RenderManager::GetInstance().GetGameHeight();

			// set the destination rectangle equal to the whole screen
			destRectangle.left = 0;
			destRectangle.top = 0;
			destRectangle.right = RenderManager::GetInstance().GetGameWidth();
			destRectangle.bottom = RenderManager::GetInstance().GetGameHeight();

			// send a render request to graphics system
			RenderManager::GetInstance().RenderObject(layerIterator->GetTextureName(), sourceRectangle, destRectangle);
		}
		// if the layer is an auto scroll layer
		else
		{
			// check to see if the current draw position will cause it to grab beyond the texture dimensions
			// one thing to note in case I need to revisit this auto scroll is currently only planned to scroll on x axis
			if (currentPosition.x + RenderManager::GetInstance().GetGameWidth() > layerIterator->GetLayerDimensions().x)
			{
				// if it will grab more than the screen dimensions
				// then setup 2 draw calls and set the destination rectangle and source rectangle accordingly
				
				// destination rectangle for first draw call is the far left and top of screen
				// to the texture dimensions limits in x and the size of the screen in y
				destRectangle.left = 0;
				destRectangle.top = 0;
				destRectangle.right = (layerIterator->GetLayerDimensions().x - currentPosition.x);
				destRectangle.bottom = RenderManager::GetInstance().GetGameHeight();

				// the source rectangle is where the current draw position is all
				// the way to the end of the texture dimensions
				sourceRectangle.left = currentPosition.x;
				sourceRectangle.top = currentPosition.y;
				sourceRectangle.right = layerIterator->GetLayerDimensions().x;
				sourceRectangle.bottom = RenderManager::GetInstance().GetGameHeight();

				// draw the texture once at this position
				RenderManager::GetInstance().RenderObject(layerIterator->GetTextureName(), sourceRectangle, destRectangle);

				// then setup the new destination rectangle to be right where the first draw ends
				destRectangle.left = (layerIterator->GetLayerDimensions().x - currentPosition.x);
				destRectangle.top = 0;
				destRectangle.right = RenderManager::GetInstance().GetGameWidth();
				destRectangle.bottom = RenderManager::GetInstance().GetGameHeight();

				// and start grabbing the texture at the beginning of the texture and only grab
				// to the end of the screen's worth of texture
				sourceRectangle.left = 0;
				sourceRectangle.top = currentPosition.y;
				sourceRectangle.right = (RenderManager::GetInstance().GetGameWidth() - (layerIterator->GetLayerDimensions().x - currentPosition.x));
				sourceRectangle.bottom = RenderManager::GetInstance().GetGameHeight();

				// draw the texture a second time at this position
				RenderManager::GetInstance().RenderObject(layerIterator->GetTextureName(), sourceRectangle, destRectangle);
			}
			// if thelayer is auto scroll but not past the edge of the screen
			else
			{
				// then the destination rectangle and source rectangle are the
				// same as if it wasn't an auto scroll layer
				destRectangle.left = 0;
				destRectangle.top = 0;
				destRectangle.right = RenderManager::GetInstance().GetGameWidth();
				destRectangle.bottom = RenderManager::GetInstance().GetGameHeight();

				sourceRectangle.left = currentPosition.x;
				sourceRectangle.top = currentPosition.y;
				sourceRectangle.right = RenderManager::GetInstance().GetGameWidth();
				sourceRectangle.bottom = RenderManager::GetInstance().GetGameHeight();

				// and draw the texture to the screen at the present position
				RenderManager::GetInstance().RenderObject(layerIterator->GetTextureName(), sourceRectangle, destRectangle);
			}
		}
	}
}

// change the velocity of all layers that are not auto scroll
void BuildingFloor::ChangeVelocity(Vector2 velocity)
{
	vector<Layer>::iterator layerIterator;	// iterator to traverse the vector of layers

	// loop through all layers for vector of Layers
	for (layerIterator = m_floorLayers.begin(); layerIterator != m_floorLayers.end(); layerIterator++)
	{
		if (!layerIterator->GetAutoScroll())
		{
			// change the velocity of all Layers both directions (x, and y)
			layerIterator->SetLayerVelocity(velocity);
		}
	}
}

// Check the point to see if world position is a moveable spot or not
// true = moveable false = not moveable
bool BuildingFloor::CheckPointForCollission(Vector2 screenPosition)
{
	Vector2		worldPosition;		// used to convert the screenPosition passed in to a worldPosition
	int			worldColumns;		// used to define how many "columns" are in the world
	int			worldRows;			// used to define how many "rows" are in the world
	int			collissionIndex;	// the world position converted to an index
	int			indexValue;			// what is the value located at index location in collission map
	Layer		lastLayer;			// reference to last layer which is the layer that will define visually
									// where the player can move and not where stairs, and doors, etc... are

	// grab reference
	lastLayer = m_floorLayers.back();

	// figure out how many 32 x 32 pixel squares the world is made up of
	worldColumns = lastLayer.GetLayerDimensions().x / 32;
	worldRows = lastLayer.GetLayerDimensions().y / 32;

	// convert the screenPosition to a worldPosition
	worldPosition.x = lastLayer.GetLayerPosition().x + screenPosition.x;
	worldPosition.y = lastLayer.GetLayerPosition().y + screenPosition.y;

	// calculate what location in collission map we are looking for
	collissionIndex = worldColumns * ((int)(worldPosition.y / 32) - 1) + (int)(worldPosition.x / 32);

	// get what the value is at the location in the collission map
	indexValue = CollissionMapValue(collissionIndex);

	// check the value from the collission map if its 0 then moveable
	if (indexValue == 0)
	{
		return true;
	}
	// any other value means not movable so deny the actor movement
	else
	{
		return false;
	}

	return false;
}

// check the point to see if the world position is a trigger
// if its a trigger return the Trigger
Trigger BuildingFloor::CheckPointForTrigger(Vector2 screenPosition)
{
	vector<Trigger>::iterator	triggerIterator;// iterator to traverse the triggers
	Vector2						worldPosition;	// used to convert the screenPosition passed in to a worldPosition
	int							worldColumns;	// used to define how many "columns" are in the world
	int							worldRows;		// used to define how many "rows" are in the world
	Trigger						returnTrigger;	// trigger to be returned
	bool						triggerFound;	// bool that tells if a trigger was found at that location
	Layer						lastLayer;		// reference to last layer which is the layer that will define visually
												// where the player can move and not where stairs, and doors, etc... are

	// set triggerFound to default at false
	triggerFound = false;

	// grab reference
	lastLayer = m_floorLayers.back();

	// figure out how many 32 x 32 pixel squares the world is made up of
	worldColumns = lastLayer.GetLayerDimensions().x / 32;
	worldRows = lastLayer.GetLayerDimensions().y / 32;

	// convert the screenPosition to a worldPosition
	worldPosition.x = lastLayer.GetLayerPosition().x + screenPosition.x;
	worldPosition.y = lastLayer.GetLayerPosition().y + screenPosition.y;

	// loop through the triggers for the floor and see if there is a trigger with a starting location at the
	// world position that was passed in
	for (triggerIterator = m_floorTriggers.begin(); triggerIterator != m_floorTriggers.end(); triggerIterator++)
	{
		// if a match is found then set the trigger to be returned equal to the matching trigger
		if (triggerIterator->GetStartingLocation() == worldPosition)
		{
			returnTrigger = (*triggerIterator);
			triggerFound = true;
			break;
		}
	}

	// if no trigger was found then set the return trigger's type to none
	if (!triggerFound)
	{
		returnTrigger.SetTriggerType(TriggerNone);
	}

	// finally return the trigger to the calling method
	return returnTrigger;
}

// return the spwan point at the specific world location
SpawnPoint BuildingFloor::GetSpawnPoint(Vector2 position)
{
	vector<SpawnPoint>::iterator	spawnPointIterator;	// used to traverse vector of spawnPoints
	SpawnPoint						spawnPoint;			// the spawn point to return
	bool							matchFound;			// did we find a spawn point at this location

														// set default value
	matchFound = false;

	// loop through spawnPoints and fine one that matches the world location
	for (spawnPointIterator = m_spawnPoints.begin(); spawnPointIterator != m_spawnPoints.end(); spawnPointIterator++)
	{
		if (spawnPointIterator->GetSpawnPointWorld() == position)
		{
			spawnPoint = (*spawnPointIterator);
			matchFound = true;
			break;
		}
	}

	// if no spawn point was found that matches the location then set the spawn
	// type equal to spawnnone to denote that no spawn point was found
	if (!matchFound)
	{
		spawnPoint.SetSpawnPointType(SpawnNone);
	}

	// return the spawn point
	return spawnPoint;
}

// return the location of the player start of the floor
Vector2 BuildingFloor::GetPlayerStartSpawnPoint()
{
	vector<SpawnPoint>::iterator	spawnPointIterator;	// used to traverse vector of spawnPoints
	Vector2							worldPosition;		// world position of the spawnPoint

	// loop through the spawn points to find the player start spawn point for the current location
	for (spawnPointIterator = m_spawnPoints.begin(); spawnPointIterator != m_spawnPoints.end(); spawnPointIterator++)
	{
		// once found set the world position equal to that points world position
		if (spawnPointIterator->GetSpawnPointType() == SpawnPlayerStart)
		{
			worldPosition = spawnPointIterator->GetSpawnPointWorld();
		}
	}

	// return the spawn point
	return worldPosition;
}

Alley::Alley()
{
	m_alleyLayers.clear();
	m_alleyObjects.clear();
	m_alleyTriggers.clear();
}

Alley::~Alley()
{
	m_alleyLayers.clear();
	m_alleyObjects.clear();
	m_alleyTriggers.clear();
}

void Alley::BuildAlley(string textFile, World *world)
{
	int			numberOfLayers = 0;		// temp variable to hold the number of layers
	int			numberOfObjects = 0;	// temp variable to hold the number of objects
	int			numberOfTriggers = 0;	// temp variable to hold the number of triggers
	int			numberOfSpawns = 0;		// temp variable to hold the number of spawn Points
	Layer		tempLayer;				// temp layer to hold data
	Object		tempObject;				// temp object to hold data
	Trigger		tempTrigger;			// temp trigger to hold data
	SpawnPoint	tempSpawn;				// temp spawnPoint to hold data
	string		tempString;				// temp string used in file reading
	Vector2		tempPoint;				// temp Vector2 to hold position
	ifstream	inFile(textFile);		// file reader variable

	m_currentWorld = world;

	// check to see if the file can be open successfully if not then do nothing
	if (inFile)
	{
		// Read in a line and place in AlleyID
		getline(inFile, m_AlleyID);
		// Read in a line and convert it to an Integer and place it into numberOfSpawns
		getline(inFile, tempString);
		numberOfSpawns = atoi(tempString.c_str());

		for (int spawnCount = 0; spawnCount < numberOfSpawns; spawnCount++)
		{
			// read a line in and set the SpawnPointType based on its value
			getline(inFile, tempString);
			if (tempString == "SpawnPlayer")
			{
				tempSpawn.SetSpawnPointType(SpawnPlayer);
			}
			if (tempString == "SpawnPlayerStart")
			{
				tempSpawn.SetSpawnPointType(SpawnPlayerStart);
			}
			if (tempString == "SpawnNPC")
			{
				tempSpawn.SetSpawnPointType(SpawnNPC);
			}
			if (tempString == "SpawnEnemy")
			{
				tempSpawn.SetSpawnPointType(SpawnEnemy);
			}
			if (tempString == "SpawnBoss")
			{
				tempSpawn.SetSpawnPointType(SpawnBoss);
			}

			// Read in a line and seperate its values and place into SpawnWorldPosition
			getline(inFile, tempString);
			istringstream ss(tempString);

			string s2;
			getline(ss, s2, ',');
			tempPoint.x = atoi(tempString.c_str());
			getline(ss, s2, ',');
			tempPoint.y = atoi(tempString.c_str());

			tempSpawn.SetSpawnPointWorld(tempPoint);
			// add the spawn point to the container
			AddSpawnPoint(tempSpawn);
		}

		// Read in a line and convert it to an Integer and place it into numberOfLayers
		getline(inFile, tempString);
		numberOfLayers = atoi(tempString.c_str());

		for (int layerCount = 0; layerCount < numberOfLayers; layerCount++)
		{
			// Read in a line and place into layer ID
			getline(inFile, tempString);
			tempLayer.SetLayerID(tempString);
			// Read in a line and place into layer textureName
			getline(inFile, tempString);
			tempLayer.SetTextureName(tempString);
			// Read in a line and check its value and set auto scroll accordingly
			getline(inFile, tempString);
			if (tempString == "TRUE")
			{
				tempLayer.SetAutoScroll(true);
			}
			else
			{
				tempLayer.SetAutoScroll(false);
			}
			// Read in a line and convert to Integer and set it to the layerSpeed
			getline(inFile, tempString);
			tempLayer.SetScrollSpeed(atoi(tempString.c_str()));
			// get the texture desc of the texture for the layer then fill in the texutre width and height with its info
			D3D11_TEXTURE2D_DESC textureDesc = RenderManager::GetInstance().getTextureDesc(tempLayer.GetTextureName());
			tempLayer.SetLayerDimensions(Vector2((float)textureDesc.Width, (float)textureDesc.Height));
			// set starting velocity at 0
			tempLayer.SetLayerVelocity(Vector2(0.0, 0.0));
			// build starting destination RECT and set it
			RECT tempRect;
			tempRect.left = 0;
			tempRect.top = 0;
			tempRect.right = RenderManager::GetInstance().GetGameWidth();
			tempRect.bottom = RenderManager::GetInstance().GetGameHeight();
			tempLayer.SetDestinationRectangle(tempRect);

			// get the player spawn point for the area
			Vector2 position = GetPlayerStartSpawnPoint();
			// Vector2 to hold the planned current starting draw position
			Vector2 currentPosition;
			// set the x coordinate equal to the player spawn point minus 32 pixels to
			// start grabbing from just to the left of the player spawn point
			currentPosition.x = position.x - 32.0;

			// check to see how close the player spawn point is to 0
			// if its not to close to zero then set the y to the player spawn point
			// minus the game height
			if ((position.y - RenderManager::GetInstance().GetGameHeight()) > 0)
			{
				currentPosition.y = position.y - RenderManager::GetInstance().GetGameHeight();
			}
			// if its to close to the top then set to start drawing at 0
			// this would most likely be an issue if the area being rendered is the size of the screen
			else
			{
				currentPosition.y = 0.0;
			}

			// set the draw position for the layer
			tempLayer.SetLayerPosition(currentPosition);

			// calculate the source rectangle
			tempRect.left = currentPosition.x;
			tempRect.top = currentPosition.y;
			tempRect.right = RenderManager::GetInstance().GetGameWidth();
			tempRect.bottom = RenderManager::GetInstance().GetGameHeight();

			// and set the source rectangle for the layer
			tempLayer.SetSourceRectangle(tempRect);
			// add the tempLayer to the AlleyLayers vector
			m_alleyLayers.push_back(tempLayer);
		}

		// loop through grabbing line by line of the collission map data
		// and adding it to the collission map vector
		for (int row = 0; row < (m_alleyLayers.back().GetLayerDimensions().y / 32); row++)
		{
			getline(inFile, tempString);
			istringstream ss(tempString);

			while (ss)
			{
				string s2;
				if (!getline(ss, s2, ','))
					break;

				m_collissionMap.push_back(atoi(s2.c_str()));
			}
		}

		// Read in a line and convert to an Integer and set it to the numberOfObjects
		getline(inFile, tempString);
		numberOfObjects = atoi(tempString.c_str());
		// Loop through the next several lines grabbing the value
		// passing the value to the BuildObject method and on return
		// adding the Object to the AlleyObjects vector
		for (int objects = 0; objects < numberOfObjects; objects++)
		{
			getline(inFile, tempString);
			tempObject.BuildObject(tempString);
			m_alleyObjects.push_back(tempObject);
		}
		// Read in a line and convert to an Integer and set it to the numberOfTriggers
		getline(inFile, tempString);
		numberOfTriggers = atoi(tempString.c_str());
		// Loop through the next several lines grabbing the value
		// passing the value to the BuildTrigger method and on return
		// adding the Object to the AlleyTriggers vector
		for (int triggers = 0; triggers < numberOfTriggers; triggers++)
		{
			getline(inFile, tempString);
			tempTrigger.BuildTrigger(tempString);
			m_alleyTriggers.push_back(tempTrigger);
		}
	}
	inFile.close();
	m_isActive = false;
}

void Alley::UpdateAlley(float deltaTime)
{
	vector<Layer>::iterator	layerIterator;		// iterator to traverse the vector of layers
	Vector2					currentPosition;	// Vector2 to hold the current draw position

												// check to see if the WorldSection is currently Active if so update if not do nothing
												// loop through the layers of the vector
	for (layerIterator = m_alleyLayers.begin(); layerIterator != m_alleyLayers.end(); layerIterator++)
	{
		// get the current position used to define where to start grabbing from texture
		currentPosition = layerIterator->GetLayerPosition();
		// check to see if this is an auto scroll layer
		if (!layerIterator->GetAutoScroll())
		{
			// check to see if the current position x value is not to large to grab more if not then increase its value
			if (currentPosition.x + RenderManager::GetInstance().GetGameWidth() < layerIterator->GetLayerDimensions().x)
			{
				currentPosition.x += layerIterator->GetScrollSpeed() * layerIterator->GetLayerVelocity().x * deltaTime;
			}
			// if it is beyond the limit of the layer then max it out to the limits
			if (currentPosition.x + RenderManager::GetInstance().GetGameWidth() > layerIterator->GetLayerDimensions().x)
			{
				currentPosition.x = layerIterator->GetLayerDimensions().x - RenderManager::GetInstance().GetGameWidth();
			}
			// maue sure if scrolling the other direction it doesn't grab from beyond the other side
			if (currentPosition.x < 0)
			{
				currentPosition.x = 0;
			}
			// check to see if the current position x value is not to large to grab more if not then increase its value
			if (currentPosition.y + RenderManager::GetInstance().GetGameHeight() < layerIterator->GetLayerDimensions().y)
			{
				currentPosition.y += layerIterator->GetScrollSpeed() * layerIterator->GetLayerVelocity().y * deltaTime;
			}
			// if it is beyond the limit of the layer then max it out to the limits
			if (currentPosition.y + RenderManager::GetInstance().GetGameHeight() > layerIterator->GetLayerDimensions().y)
			{
				currentPosition.y = layerIterator->GetLayerDimensions().y - RenderManager::GetInstance().GetGameHeight();
			}
			// maue sure if scrolling the other direction it doesn't grab from beyond the other side
			if (currentPosition.y < 0)
			{
				currentPosition.y = 0;
			}
		}
		// if it is an auto scroll layer just update the x position
		else
		{
			currentPosition.x += layerIterator->GetScrollSpeed() * layerIterator->GetLayerVelocity().x * deltaTime;
		}
		layerIterator->SetLayerPosition(currentPosition);
	}
}

void Alley::RenderAlley()
{
	vector<Layer>::iterator	layerIterator;		// iterator to traverse the vector of layers
	Vector2					position(0.0, 0.0);	// used for calculating the position to draw each tile
	Vector2					currentPosition;	// the current layer's drawPosition
	RECT					sourceRectangle;	// the sourceRectangle for grabbing from the texture
	RECT					destRectangle;		// the destinationRectangle for drawing to a part of the screen

	// loop through the layers of the vector
	for (layerIterator = m_alleyLayers.begin(); layerIterator != m_alleyLayers.end(); layerIterator++)
	{
		// grab the current draw position for the current layer
		currentPosition = layerIterator->GetLayerPosition();
		// if the layer is not an auto scroll layer
		if (!layerIterator->GetAutoScroll())
		{
			// set the source rectangle equal to startin at the current draw position and grab a full screen's worth
			sourceRectangle.left = currentPosition.x;
			sourceRectangle.top = currentPosition.y;
			sourceRectangle.right = RenderManager::GetInstance().GetGameWidth();
			sourceRectangle.bottom = RenderManager::GetInstance().GetGameHeight();

			// set the destination rectangle equal to the whole screen
			destRectangle.left = 0;
			destRectangle.top = 0;
			destRectangle.right = RenderManager::GetInstance().GetGameWidth();
			destRectangle.bottom = RenderManager::GetInstance().GetGameHeight();

			// send a render request to graphics system
			RenderManager::GetInstance().RenderObject(layerIterator->GetTextureName(), sourceRectangle, destRectangle);
		}
		// if the layer is an auto scroll layer
		else
		{
			// check to see if the current draw position will cause it to grab beyond the texture dimensions
			// one thing to note in case I need to revisit this auto scroll is currently only planned to scroll on x axis
			if (currentPosition.x + RenderManager::GetInstance().GetGameWidth() > layerIterator->GetLayerDimensions().x)
			{
				// if it will grab more than the screen dimensions
				// then setup 2 draw calls and set the destination rectangle and source rectangle accordingly

				// destination rectangle for first draw call is the far left and top of screen
				// to the texture dimensions limits in x and the size of the screen in y
				destRectangle.left = 0;
				destRectangle.top = 0;
				destRectangle.right = (layerIterator->GetLayerDimensions().x - currentPosition.x);
				destRectangle.bottom = RenderManager::GetInstance().GetGameHeight();

				// the source rectangle is where the current draw position is all
				// the way to the end of the texture dimensions
				sourceRectangle.left = currentPosition.x;
				sourceRectangle.top = currentPosition.y;
				sourceRectangle.right = layerIterator->GetLayerDimensions().x;
				sourceRectangle.bottom = RenderManager::GetInstance().GetGameHeight();

				// draw the texture once at this position
				RenderManager::GetInstance().RenderObject(layerIterator->GetTextureName(), sourceRectangle, destRectangle);

				// then setup the new destination rectangle to be right where the first draw ends
				destRectangle.left = (layerIterator->GetLayerDimensions().x - currentPosition.x);
				destRectangle.top = 0;
				destRectangle.right = RenderManager::GetInstance().GetGameWidth();
				destRectangle.bottom = RenderManager::GetInstance().GetGameHeight();

				// and start grabbing the texture at the beginning of the texture and only grab
				// to the end of the screen's worth of texture
				sourceRectangle.left = 0;
				sourceRectangle.top = currentPosition.y;
				sourceRectangle.right = (RenderManager::GetInstance().GetGameWidth() - (layerIterator->GetLayerDimensions().x - currentPosition.x));
				sourceRectangle.bottom = RenderManager::GetInstance().GetGameHeight();

				// draw the texture a second time at this position
				RenderManager::GetInstance().RenderObject(layerIterator->GetTextureName(), sourceRectangle, destRectangle);
			}
			// if thelayer is auto scroll but not past the edge of the screen
			else
			{
				// then the destination rectangle and source rectangle are the
				// same as if it wasn't an auto scroll layer
				destRectangle.left = 0;
				destRectangle.top = 0;
				destRectangle.right = RenderManager::GetInstance().GetGameWidth();
				destRectangle.bottom = RenderManager::GetInstance().GetGameHeight();

				sourceRectangle.left = currentPosition.x;
				sourceRectangle.top = currentPosition.y;
				sourceRectangle.right = RenderManager::GetInstance().GetGameWidth();
				sourceRectangle.bottom = RenderManager::GetInstance().GetGameHeight();

				// and draw the texture to the screen at the present position
				RenderManager::GetInstance().RenderObject(layerIterator->GetTextureName(), sourceRectangle, destRectangle);
			}
		}
	}
}

// change the velocity of all layers that are not auto scroll
void Alley::ChangeVelocity(Vector2 velocity)
{
	vector<Layer>::iterator layerIterator;	// iterator to traverse the vector of layers

	// loop through all layers for vector of Layers
	for (layerIterator = m_alleyLayers.begin(); layerIterator != m_alleyLayers.end(); layerIterator++)
	{
		if (!layerIterator->GetAutoScroll())
		{
			// change the velocity of all Layers both directions (x, and y)
			layerIterator->SetLayerVelocity(velocity);
		}
	}
}

// Check the point to see if world position is a moveable spot or not
// true = moveable false = not moveable
bool Alley::CheckPointForCollission(Vector2 screenPosition)
{
	Vector2		worldPosition;		// used to convert the screenPosition passed in to a worldPosition
	int			worldColumns;		// used to define how many "columns" are in the world
	int			worldRows;			// used to define how many "rows" are in the world
	int			collissionIndex;	// the world position converted to an index
	int			indexValue;			// what is the value located at index location in collission map
	Layer		lastLayer;			// reference to last layer which is the layer that will define visually
									// where the player can move and not where stairs, and doors, etc... are

	// grab reference
	lastLayer = m_alleyLayers.back();

	// figure out how many 32 x 32 pixel squares the world is made up of
	worldColumns = lastLayer.GetLayerDimensions().x / 32;
	worldRows = lastLayer.GetLayerDimensions().y / 32;

	// convert the screenPosition to a worldPosition
	worldPosition.x = lastLayer.GetLayerPosition().x + screenPosition.x;
	worldPosition.y = lastLayer.GetLayerPosition().y + screenPosition.y;

	// calculate what location in collission map we are looking for
	collissionIndex = worldColumns * ((int)(worldPosition.y / 32) - 1) + (int)(worldPosition.x / 32);

	// get what the value is at the location in the collission map
	indexValue = CollissionMapValue(collissionIndex);

	// check the value from the collission map if its 0 then moveable
	if (indexValue == 0)
	{
		return true;
	}
	// any other value means not movable so deny the actor movement
	else
	{
		return false;
	}

	return false;
}

// check the point to see if the world position is a trigger
// if its a trigger return the Trigger
Trigger Alley::CheckPointForTrigger(Vector2 screenPosition)
{
	vector<Trigger>::iterator	triggerIterator;// iterator to traverse the triggers
	Vector2						worldPosition;	// used to convert the screenPosition passed in to a worldPosition
	int							worldColumns;	// used to define how many "columns" are in the world
	int							worldRows;		// used to define how many "rows" are in the world
	Trigger						returnTrigger;	// trigger to be returned
	bool						triggerFound;	// bool that tells if a trigger was found at that location
	Layer						lastLayer;		// reference to last layer which is the layer that will define visually
												// where the player can move and not where stairs, and doors, etc... are

	// set triggerFound to default at false
	triggerFound = false;

	// grab reference
	lastLayer = m_alleyLayers.back();

	// figure out how many 32 x 32 pixel squares the world is made up of
	worldColumns = lastLayer.GetLayerDimensions().x / 32;
	worldRows = lastLayer.GetLayerDimensions().y / 32;

	// convert the screenPosition to a worldPosition
	worldPosition.x = lastLayer.GetLayerPosition().x + screenPosition.x;
	worldPosition.y = lastLayer.GetLayerPosition().y + screenPosition.y;

	// loop through the triggers for the floor and see if there is a trigger with a starting location at the
	// world position that was passed in
	for (triggerIterator = m_alleyTriggers.begin(); triggerIterator != m_alleyTriggers.end(); triggerIterator++)
	{
		// if a match is found then set the trigger to be returned equal to the matching trigger
		if (triggerIterator->GetStartingLocation() == worldPosition)
		{
			returnTrigger = (*triggerIterator);
			triggerFound = true;
			break;
		}
	}

	// if no trigger was found then set the return trigger's type to none
	if (!triggerFound)
	{
		returnTrigger.SetTriggerType(TriggerNone);
	}

	// finally return the trigger to the calling method
	return returnTrigger;
}

// return the spwan point at the specific world location
SpawnPoint Alley::GetSpawnPoint(Vector2 position)
{
	vector<SpawnPoint>::iterator	spawnPointIterator;	// used to traverse vector of spawnPoints
	SpawnPoint						spawnPoint;			// the spawn point to return
	bool							matchFound;			// did we find a spawn point at this location

	// set default value
	matchFound = false;

	// loop through spawnPoints and fine one that matches the world location
	for (spawnPointIterator = m_spawnPoints.begin(); spawnPointIterator != m_spawnPoints.end(); spawnPointIterator++)
	{
		if (spawnPointIterator->GetSpawnPointWorld() == position)
		{
			spawnPoint = (*spawnPointIterator);
			matchFound = true;
			break;
		}
	}

	// if no spawn point was found that matches the location then set the spawn
	// type equal to spawnnone to denote that no spawn point was found
	if (!matchFound)
	{
		spawnPoint.SetSpawnPointType(SpawnNone);
	}

	// return the spawn point
	return spawnPoint;
}

// return the location of the player start of the floor
Vector2 Alley::GetPlayerStartSpawnPoint()
{
	vector<SpawnPoint>::iterator	spawnPointIterator;	// used to traverse vector of spawnPoints
	Vector2							worldPosition;		// world position of the spawnPoint

	// loop through the spawn points to find the player start spawn point for the current location
	for (spawnPointIterator = m_spawnPoints.begin(); spawnPointIterator != m_spawnPoints.end(); spawnPointIterator++)
	{
		// once found set the world position equal to that points world position
		if (spawnPointIterator->GetSpawnPointType() == SpawnPlayerStart)
		{
			worldPosition = spawnPointIterator->GetSpawnPointWorld();
		}
	}

	// return the spawn point
	return worldPosition;
}

Building::Building()
{
	m_buildingFloors.clear();
}

Building::~Building()
{
	m_buildingFloors.clear();
}

void Building::BuildBuilding(string textFile, World *world)
{
	int				numberOfFloors = 0;		// temp variable to hold the nubmer of floors
	string			tempString;				// temp String to hold data read in from textFile
	SpawnPoint		tempSpawn;				// temp SpawnPoint to hold a single spawn point
	Vector2			tempPoint;				// temp Vector2 to hold worldSpawnPoint
	ifstream		inFile(textFile);		// file reader variable

	m_currentWorld = world;

	// check to see if the file can be open successfully if not then do nothing
	if (inFile)
	{
		// Read in a line and place in BuildingID
		getline(inFile, m_buildingID);
		// Read in a line and convert to an Integer and set it to the numberOfFloors
		getline(inFile, tempString);
		numberOfFloors = atoi(tempString.c_str());
		// loop through and build each floor
		for (int floors = 0; floors < numberOfFloors; floors++)
		{
			// temp BuildingFloor to hold data
			BuildingFloor*	tempFloor = new BuildingFloor();
			// Read in a line and pass it and the floor number to the floor to build itself
			getline(inFile, tempString);
			tempFloor->BuildFloor(floors, tempString);
			
			// done building the current floor add to floors
			m_buildingFloors.insert(pair<int, BuildingFloor*>(floors, tempFloor));
		}
	}
	inFile.close();
	m_isActive = false;
}

void Building::UpdateBuilding(float deltaTime)
{
	// if the Building is active then update the current floor
	if (m_isActive && m_currentFloor != nullptr)
	{
		m_currentFloor->UpdateFloor(deltaTime);
	}
}

void Building::RenderBuilding()
{
	// if the Building is active then update the current floor
	if (m_isActive && m_currentFloor != nullptr)
	{
		m_currentFloor->RenderFloor();
	}
}

void Building::SetCurrentFloor(int floorID)
{
	// check to see if there is a buildingFloor with that floorNumber
	auto floorIndex = m_buildingFloors.find(floorID);

	// if there was a match then set the currentFloor
	if (floorIndex != m_buildingFloors.end())
	{
		m_currentFloor = floorIndex->second;
	}
	// if not then set the currentFloor to nullptr
	else
	{
		m_currentFloor = nullptr;
	}
}

// change the velocity of all layers that are not auto scroll
void Building::ChangeVelocity(Vector2 velocity)
{
	if (m_isActive && m_currentFloor != nullptr)
	{
		m_currentFloor->ChangeVelocity(velocity);
	}
}

// Check the point to see if world position is a moveable spot or not
// true = moveable false = not moveable
bool Building::CheckPointForCollission(Vector2 screenPosition)
{
	if (m_isActive && m_currentFloor != nullptr)
	{
		return m_currentFloor->CheckPointForCollission(screenPosition);
	}
	return false;
}

// check the point to see if the world position is a trigger
// if its a trigger return the Trigger
Trigger Building::CheckPointForTrigger(Vector2 screenPosition)
{
	if (m_isActive && m_currentFloor != nullptr)
	{
		return m_currentFloor->CheckPointForTrigger(screenPosition);
	}
	return Trigger();
}

// return the location of the player start of the floor
Vector2 Building::GetPlayerStartSpawnPoint()
{
	if (m_isActive && m_currentFloor != nullptr)
	{
		return m_currentFloor->GetPlayerStartSpawnPoint();
	}

	return Vector2(0.0, 0.0);
}

WorldSection::WorldSection()
{
	// left blank for now
}

WorldSection::~WorldSection()
{
	m_sectionLayers.clear();
	m_sectionObjects.clear();
	m_sectionTriggers.clear();
}

void WorldSection::BuildSection(string textFile, World *world)
{
	int			numberOfLayers = 0;		// temp variable to hold the number of layers
	int			numberOfObjects = 0;	// temp variable to hold the number of objects
	int			numberOfTriggers = 0;	// temp variable to hold the number of triggers
	int			numberOfSpawns = 0;		// temp variable to hold the number of SpawnPoints
	Layer		tempLayer;				// temp layer to hold data
	Object		tempObject;				// temp object to hold data
	Trigger		tempTrigger;			// temp trigger to hold data
	SpawnPoint	tempSpawn;				// temp SpawnPoint to hold data
	Vector2		tempPoint;				// temp Vector2 to hold worldSpawnPoint Location
	string		tempString;				// temp string used in file reading
	ifstream	inFile(textFile);		// file reader variable

	m_currentWorld = world;

	// check to see if the file can be open successfully if not then do nothing
	if (inFile)
	{
		// Read in a line and place in AlleyID
		getline(inFile, m_sectionID);

		// Read in a line and convert it to an Integer and place it into numberOfSpawns
		getline(inFile, tempString);
		numberOfSpawns = atoi(tempString.c_str());

		for (int spawnCount = 0; spawnCount < numberOfSpawns; spawnCount++)
		{
			// read a line in and set the SpawnPointType based on its value
			getline(inFile, tempString);
			if (tempString == "SpawnPlayer")
			{
				tempSpawn.SetSpawnPointType(SpawnPlayer);
			}
			if (tempString == "SpawnPlayerStart")
			{
				tempSpawn.SetSpawnPointType(SpawnPlayerStart);
			}
			if (tempString == "SpawnNPC")
			{
				tempSpawn.SetSpawnPointType(SpawnNPC);
			}
			if (tempString == "SpawnEnemy")
			{
				tempSpawn.SetSpawnPointType(SpawnEnemy);
			}
			if (tempString == "SpawnBoss")
			{
				tempSpawn.SetSpawnPointType(SpawnBoss);
			}

			// Read in a line and seperate its values and place into SpawnWorldPosition
			getline(inFile, tempString);
			istringstream ss(tempString);

			string s2;
			getline(ss, s2, ',');
			tempPoint.x = atoi(tempString.c_str());
			getline(ss, s2, ',');
			tempPoint.y = atoi(tempString.c_str());

			tempSpawn.SetSpawnPointWorld(tempPoint);
			// add the spawn point to the container
			AddSpawnPoint(tempSpawn);
		}

		// Read in a line and convert it to an Integer and place it into numberOfLayers
		getline(inFile, tempString);
		numberOfLayers = atoi(tempString.c_str());

		for (int layerCount = 0; layerCount < numberOfLayers; layerCount++)
		{
			// Read in a line and place into layer ID
			getline(inFile, tempString);
			tempLayer.SetLayerID(tempString);
			// Read in a line and place into layer textureName
			getline(inFile, tempString);
			tempLayer.SetTextureName(tempString);
			// Read in a line and check its value and set auto scroll accordingly
			getline(inFile, tempString);
			if (tempString == "TRUE")
			{
				tempLayer.SetAutoScroll(true);
			}
			else
			{
				tempLayer.SetAutoScroll(false);
			}
			// Read in a line and convert to Integer and set it to the layerSpeed
			getline(inFile, tempString);
			tempLayer.SetScrollSpeed(atoi(tempString.c_str()));
			// get the texture desc of the texture for the layer then fill in the texutre width and height with its info
			D3D11_TEXTURE2D_DESC textureDesc = RenderManager::GetInstance().getTextureDesc(tempLayer.GetTextureName());
			tempLayer.SetLayerDimensions(Vector2((float)textureDesc.Width, (float)textureDesc.Height));
			// set starting velocity at 0
			tempLayer.SetLayerVelocity(Vector2(0.0, 0.0));
			// build starting destination RECT and set it
			RECT tempRect;
			tempRect.left = 0;
			tempRect.top = 0;
			tempRect.right = RenderManager::GetInstance().GetGameWidth();
			tempRect.bottom = RenderManager::GetInstance().GetGameHeight();
			tempLayer.SetDestinationRectangle(tempRect);
			
			// get the player spawn point for the area
			Vector2 position = GetPlayerStartSpawnPoint();
			// Vector2 to hold the planned current starting draw position
			Vector2 currentPosition;
			// set the x coordinate equal to the player spawn point minus 32 pixels to
			// start grabbing from just to the left of the player spawn point
			currentPosition.x = position.x - 32.0;

			// check to see how close the player spawn point is to 0
			// if its not to close to zero then set the y to the player spawn point
			// minus the game height
			if ((position.y - RenderManager::GetInstance().GetGameHeight()) > 0)
			{
				currentPosition.y = position.y - RenderManager::GetInstance().GetGameHeight();
			}
			// if its to close to the top then set to start drawing at 0
			// this would most likely be an issue if the area being rendered is the size of the screen
			else
			{
				currentPosition.y = 0.0;
			}

			// set the draw position for the layer
			tempLayer.SetLayerPosition(currentPosition);

			// calculate the source rectangle
			tempRect.left = currentPosition.x;
			tempRect.top = currentPosition.y;
			tempRect.right = RenderManager::GetInstance().GetGameWidth();
			tempRect.bottom = RenderManager::GetInstance().GetGameHeight();

			// and set the source rectangle for the layer
			tempLayer.SetSourceRectangle(tempRect);

			// add the tempLayer to the AlleyLayers vector
			m_sectionLayers.push_back(tempLayer);
		}

		// loop through grabbing line by line of the collission map data
		// and adding it to the collission map vector
		for (int row = 0; row < (m_sectionLayers.back().GetLayerDimensions().y / 32); row++)
		{
			getline(inFile, tempString);
			istringstream ss(tempString);

			while (ss)
			{
				string s2;
				if (!getline(ss, s2, ','))
					break;

				m_collissionMap.push_back(atoi(s2.c_str()));
			}
		}

		// Read in a line and convert to an Integer and set it to the numberOfObjects
		getline(inFile, tempString);
		numberOfObjects = atoi(tempString.c_str());
		// Loop through the next several lines grabbing the value
		// passing the value to the BuildObject method and on return
		// adding the Object to the AlleyObjects vector
		for (int objects = 0; objects < numberOfObjects; objects++)
		{
			getline(inFile, tempString);
			tempObject.BuildObject(tempString);
			m_sectionObjects.push_back(tempObject);
		}
		// Read in a line and convert to an Integer and set it to the numberOfTriggers
		getline(inFile, tempString);
		numberOfTriggers = atoi(tempString.c_str());
		// Loop through the next several lines grabbing the value
		// passing the value to the BuildTrigger method and on return
		// adding the Object to the AlleyTriggers vector
		for (int triggers = 0; triggers < numberOfTriggers; triggers++)
		{
			getline(inFile, tempString);
			tempTrigger.BuildTrigger(tempString);
			m_sectionTriggers.push_back(tempTrigger);
		}
	}
	inFile.close();
	m_isActive = false;
}

void WorldSection::UpdateSection(float deltaTime)
{
	vector<Layer>::iterator	layerIterator;		// iterator to traverse the vector of layers
	Vector2					currentPosition;	// Vector2 to hold the current draw position

	// loop through the layers of the vector
	for (layerIterator = m_sectionLayers.begin(); layerIterator != m_sectionLayers.end(); layerIterator++)
	{
		// get the current position used to define where to start grabbing from texture
		currentPosition = layerIterator->GetLayerPosition();
		// check to see if this is an auto scroll layer
		if (!layerIterator->GetAutoScroll())
		{
			// check to see if the current position x value is not to large to grab more if not then increase its value
			if (currentPosition.x + RenderManager::GetInstance().GetGameWidth() < layerIterator->GetLayerDimensions().x)
			{
				currentPosition.x += layerIterator->GetScrollSpeed() * layerIterator->GetLayerVelocity().x * deltaTime;
			}
			// if it is beyond the limit of the layer then max it out to the limits
			if (currentPosition.x + RenderManager::GetInstance().GetGameWidth() > layerIterator->GetLayerDimensions().x)
			{
				currentPosition.x = layerIterator->GetLayerDimensions().x - RenderManager::GetInstance().GetGameWidth();
			}
			// maue sure if scrolling the other direction it doesn't grab from beyond the other side
			if (currentPosition.x < 0)
			{
				currentPosition.x = 0;
			}
			// check to see if the current position x value is not to large to grab more if not then increase its value
			if (currentPosition.y + RenderManager::GetInstance().GetGameHeight() < layerIterator->GetLayerDimensions().y)
			{
				currentPosition.y += layerIterator->GetScrollSpeed() * layerIterator->GetLayerVelocity().y * deltaTime;
			}
			// if it is beyond the limit of the layer then max it out to the limits
			if (currentPosition.y + RenderManager::GetInstance().GetGameHeight() > layerIterator->GetLayerDimensions().y)
			{
				currentPosition.y = layerIterator->GetLayerDimensions().y - RenderManager::GetInstance().GetGameHeight();
			}
			// maue sure if scrolling the other direction it doesn't grab from beyond the other side
			if (currentPosition.y < 0)
			{
				currentPosition.y = 0;
			}
		}
		// if it is an auto scroll layer just update the x position
		else
		{
			currentPosition.x += layerIterator->GetScrollSpeed() * layerIterator->GetLayerVelocity().x * deltaTime;
		}

		layerIterator->SetLayerPosition(currentPosition);
	}
}

void WorldSection::RenderSection()
{
	vector<Layer>::iterator	layerIterator;		// iterator to traverse the vector of layers
	Vector2					position(0.0, 0.0);	// used for calculating the position to draw each tile
	Vector2					currentPosition;	// the current layer's drawPosition
	RECT					sourceRectangle;	// the sourceRectangle for grabbing from the texture
	RECT					destRectangle;		// the destinationRectangle for drawing to a part of the screen

	// loop through the layers of the vector
	for (layerIterator = m_sectionLayers.begin(); layerIterator != m_sectionLayers.end(); layerIterator++)
	{
		// grab the current draw position for the current layer
		currentPosition = layerIterator->GetLayerPosition();
		// if the layer is not an auto scroll layer
		if (!layerIterator->GetAutoScroll())
		{
			// set the source rectangle equal to startin at the current draw position and grab a full screen's worth
			sourceRectangle.left = currentPosition.x;
			sourceRectangle.top = currentPosition.y;
			sourceRectangle.right = RenderManager::GetInstance().GetGameWidth();
			sourceRectangle.bottom = RenderManager::GetInstance().GetGameHeight();

			// set the destination rectangle equal to the whole screen
			destRectangle.left = 0;
			destRectangle.top = 0;
			destRectangle.right = RenderManager::GetInstance().GetGameWidth();
			destRectangle.bottom = RenderManager::GetInstance().GetGameHeight();

			// send a render request to graphics system
			RenderManager::GetInstance().RenderObject(layerIterator->GetTextureName(), sourceRectangle, destRectangle);
		}
		// if the layer is an auto scroll layer
		else
		{
			// check to see if the current draw position will cause it to grab beyond the texture dimensions
			// one thing to note in case I need to revisit this auto scroll is currently only planned to scroll on x axis
			if (currentPosition.x + RenderManager::GetInstance().GetGameWidth() > layerIterator->GetLayerDimensions().x)
			{
				// if it will grab more than the screen dimensions
				// then setup 2 draw calls and set the destination rectangle and source rectangle accordingly

				// destination rectangle for first draw call is the far left and top of screen
				// to the texture dimensions limits in x and the size of the screen in y
				destRectangle.left = 0;
				destRectangle.top = 0;
				destRectangle.right = (layerIterator->GetLayerDimensions().x - currentPosition.x);
				destRectangle.bottom = RenderManager::GetInstance().GetGameHeight();

				// the source rectangle is where the current draw position is all
				// the way to the end of the texture dimensions
				sourceRectangle.left = currentPosition.x;
				sourceRectangle.top = currentPosition.y;
				sourceRectangle.right = layerIterator->GetLayerDimensions().x;
				sourceRectangle.bottom = RenderManager::GetInstance().GetGameHeight();

				// draw the texture once at this position
				RenderManager::GetInstance().RenderObject(layerIterator->GetTextureName(), sourceRectangle, destRectangle);

				// then setup the new destination rectangle to be right where the first draw ends
				destRectangle.left = (layerIterator->GetLayerDimensions().x - currentPosition.x);
				destRectangle.top = 0;
				destRectangle.right = RenderManager::GetInstance().GetGameWidth();
				destRectangle.bottom = RenderManager::GetInstance().GetGameHeight();

				// and start grabbing the texture at the beginning of the texture and only grab
				// to the end of the screen's worth of texture
				sourceRectangle.left = 0;
				sourceRectangle.top = currentPosition.y;
				sourceRectangle.right = (RenderManager::GetInstance().GetGameWidth() - (layerIterator->GetLayerDimensions().x - currentPosition.x));
				sourceRectangle.bottom = RenderManager::GetInstance().GetGameHeight();

				// draw the texture a second time at this position
				RenderManager::GetInstance().RenderObject(layerIterator->GetTextureName(), sourceRectangle, destRectangle);
			}
			// if thelayer is auto scroll but not past the edge of the screen
			else
			{
				// then the destination rectangle and source rectangle are the
				// same as if it wasn't an auto scroll layer
				destRectangle.left = 0;
				destRectangle.top = 0;
				destRectangle.right = RenderManager::GetInstance().GetGameWidth();
				destRectangle.bottom = RenderManager::GetInstance().GetGameHeight();

				sourceRectangle.left = currentPosition.x;
				sourceRectangle.top = currentPosition.y;
				sourceRectangle.right = RenderManager::GetInstance().GetGameWidth();
				sourceRectangle.bottom = RenderManager::GetInstance().GetGameHeight();

				// and draw the texture to the screen at the present position
				RenderManager::GetInstance().RenderObject(layerIterator->GetTextureName(), sourceRectangle, destRectangle);
			}
		}
	}
}

// change the velocity of all layers that are not auto scroll
void WorldSection::ChangeVelocity(Vector2 velocity)
{
	vector<Layer>::iterator layerIterator;	// iterator to traverse the vector of layers

	// loop through all layers for vector of Layers
	for (layerIterator = m_sectionLayers.begin(); layerIterator != m_sectionLayers.end(); layerIterator++)
	{
		if (!layerIterator->GetAutoScroll())
		{
			// change the velocity of all Layers both directions (x, and y)
			layerIterator->SetLayerVelocity(velocity);
		}
	}
}

// Check the point to see if world position is a moveable spot or not
// true = moveable false = not moveable
bool WorldSection::CheckPointForCollission(Vector2 screenPosition)
{
	Vector2		worldPosition;		// used to convert the screenPosition passed in to a worldPosition
	int			worldColumns;		// used to define how many "columns" are in the world
	int			worldRows;			// used to define how many "rows" are in the world
	int			collissionIndex;	// the world position converted to an index
	int			indexValue;			// what is the value located at index location in collission map
	Layer		lastLayer;			// reference to last layer which is the layer that will define visually
									// where the player can move and not where stairs, and doors, etc... are

	// grab reference
	lastLayer = m_sectionLayers.back();

	// figure out how many 32 x 32 pixel squares the world is made up of
	worldColumns = lastLayer.GetLayerDimensions().x / 32;
	worldRows = lastLayer.GetLayerDimensions().y / 32;

	// convert the screenPosition to a worldPosition
	worldPosition.x = lastLayer.GetLayerPosition().x + screenPosition.x;
	worldPosition.y = lastLayer.GetLayerPosition().y + screenPosition.y;

	// calculate what location in collission map we are looking for
	collissionIndex = worldColumns * ((int)(worldPosition.y / 32) - 1) + (int)(worldPosition.x / 32);

	// get what the value is at the location in the collission map
	indexValue = CollissionMapValue(collissionIndex);

	// check the value from the collission map if its 0 then moveable
	if (indexValue == 0)
	{
		return true;
	}
	// any other value means not movable so deny the actor movement
	else
	{
		return false;
	}

	return false;
}

// check the point to see if the world position is a trigger
// if its a trigger return the Trigger
Trigger WorldSection::CheckPointForTrigger(Vector2 screenPosition)
{
	vector<Trigger>::iterator	triggerIterator;// iterator to traverse the triggers
	Vector2						worldPosition;	// used to convert the screenPosition passed in to a worldPosition
	int							worldColumns;	// used to define how many "columns" are in the world
	int							worldRows;		// used to define how many "rows" are in the world
	Trigger						returnTrigger;	// trigger to be returned
	bool						triggerFound;	// bool that tells if a trigger was found at that location
	Layer						lastLayer;		// reference to last layer which is the layer that will define visually
												// where the player can move and not where stairs, and doors, etc... are

	// set triggerFound to default at false
	triggerFound = false;

	// grab reference
	lastLayer = m_sectionLayers.back();

	// figure out how many 32 x 32 pixel squares the world is made up of
	worldColumns = lastLayer.GetLayerDimensions().x / 32;
	worldRows = lastLayer.GetLayerDimensions().y / 32;

	// convert the screenPosition to a worldPosition
	worldPosition.x = lastLayer.GetLayerPosition().x + screenPosition.x;
	worldPosition.y = lastLayer.GetLayerPosition().y + screenPosition.y;

	// loop through the triggers for the floor and see if there is a trigger with a starting location at the
	// world position that was passed in
	for (triggerIterator = m_sectionTriggers.begin(); triggerIterator != m_sectionTriggers.end(); triggerIterator++)
	{
		// if a match is found then set the trigger to be returned equal to the matching trigger
		if (triggerIterator->GetStartingLocation() == worldPosition)
		{
			returnTrigger = (*triggerIterator);
			triggerFound = true;
			break;
		}
	}

	// if no trigger was found then set the return trigger's type to none
	if (!triggerFound)
	{
		returnTrigger.SetTriggerType(TriggerNone);
	}

	// finally return the trigger to the calling method
	return returnTrigger;
}

// return the spwan point at the specific world location
SpawnPoint WorldSection::GetSpawnPoint(Vector2 position)
{
	vector<SpawnPoint>::iterator	spawnPointIterator;	// used to traverse vector of spawnPoints
	SpawnPoint						spawnPoint;			// the spawn point to return
	bool							matchFound;			// did we find a spawn point at this location

														// set default value
	matchFound = false;

	// loop through spawnPoints and fine one that matches the world location
	for (spawnPointIterator = m_spawnPoints.begin(); spawnPointIterator != m_spawnPoints.end(); spawnPointIterator++)
	{
		if (spawnPointIterator->GetSpawnPointWorld() == position)
		{
			spawnPoint = (*spawnPointIterator);
			matchFound = true;
			break;
		}
	}

	// if no spawn point was found that matches the location then set the spawn
	// type equal to spawnnone to denote that no spawn point was found
	if (!matchFound)
	{
		spawnPoint.SetSpawnPointType(SpawnNone);
	}

	// return the spawn point
	return spawnPoint;
}

// return the location of the player start of the floor
Vector2 WorldSection::GetPlayerStartSpawnPoint()
{
	vector<SpawnPoint>::iterator	spawnPointIterator;	// used to traverse vector of spawnPoints
	Vector2							worldPosition;		// world position of the spawnPoint

														// loop through the spawn points to find the player start spawn point for the current location
	for (spawnPointIterator = m_spawnPoints.begin(); spawnPointIterator != m_spawnPoints.end(); spawnPointIterator++)
	{
		// once found set the world position equal to that points world position
		if (spawnPointIterator->GetSpawnPointType() == SpawnPlayerStart)
		{
			worldPosition = spawnPointIterator->GetSpawnPointWorld();
		}
	}

	// return the spawn point
	return worldPosition;
}

World::World()
{
	m_currentWorldSection = nullptr;
	m_currentBuilding = nullptr;
	m_currentAlley = nullptr;
}

World::~World()
{
	m_worldAlleys.clear();
	m_worldBuildings.clear();
	m_worldSections.clear();
}

void World::BuildWorld(string textFile, GlobalWorld* GLWorld)
{
	int			numberOfSections = 0;	// temp variable holds the number of Sections for this World
	int			numberOfBuildings = 0;	// temp variable holds the number of Buildings for this World
	int			numberOfAlleys = 0;		// temp variable holds the number of Alleys for this World
	string		tempString;				// temp variable holds a line read in from file
	ifstream	inFile(textFile);		// file reader variable

	// check to see if the file can be open successfully if not then do nothing
	if (inFile)
	{
		// Read a line in and place in WorldID
		getline(inFile, m_worldID);
		// Read a line in and convert to Integer and place into numberOfSections
		getline(inFile, tempString);
		numberOfSections = atoi(tempString.c_str());
		// Loop through read in and build all WorldSection data
		for (int sections = 0; sections < numberOfSections; sections++)
		{
			getline(inFile, tempString);
			WorldSection* newSection = new WorldSection();
			newSection->BuildSection(tempString, this);
			m_worldSections.insert(pair<string, WorldSection*>(newSection->GetSectionID(), newSection));
		}

		// Read a line in and convert to Integer and place into numberOfBuildings
		getline(inFile, tempString);
		numberOfBuildings = atoi(tempString.c_str());
		// Loop through read in and build all Buildings data
		for (int buildings = 0; buildings < numberOfBuildings; buildings++)
		{
			getline(inFile, tempString);
			Building* newBuilding = new Building();
			newBuilding->BuildBuilding(tempString, this);
			m_worldBuildings.insert(pair<string, Building*>(newBuilding->GetBuildingID(), newBuilding));
		}

		// Read a line in and convert to Integer and place into numberOfAlleys
		getline(inFile, tempString);
		numberOfAlleys = atoi(tempString.c_str());
		// Loop through read in and build all Alleys data
		for (int alleys = 0; alleys < numberOfAlleys; alleys++)
		{
			getline(inFile, tempString);
			Alley* newAlley = new Alley();
			newAlley->BuildAlley(tempString, this);
			m_worldAlleys.insert(pair<string, Alley*>(newAlley->GetAlleyID(), newAlley));
		}
	}

	inFile.close();
}

void World::UpdateWorld(float timeDelta)
{
	// check to see if the currentWorldSection is set and it is active if so update
	if (m_currentWorldSection != nullptr && m_currentWorldSection->GetIsActive())
	{
		m_currentWorldSection->UpdateSection(timeDelta);
	}

	// check to see if the currentBuilding is set if so update
	if (m_currentBuilding != nullptr)
	{
		m_currentBuilding->UpdateBuilding(timeDelta);
	}

	// check to see if the currentAlley is set if so update
	if (m_currentAlley != nullptr)
	{
		m_currentAlley->UpdateAlley(timeDelta);
	}
}

void World::RenderWorld()
{
	// check to see if the currentSection is set and is active if so render
	if (m_currentWorldSection != nullptr && m_currentWorldSection->GetIsActive())
	{
		m_currentWorldSection->RenderSection();
	}

	// check to see if the currentBuilding is set if so update
	if (m_currentBuilding != nullptr)
	{
		m_currentBuilding->RenderBuilding();
	}

	// check to see if the currentAlley is set if so update
	if (m_currentAlley != nullptr)
	{
		m_currentAlley->RenderAlley();
	}
}

// return the location of the player
Vector2 World::GetPlayerStartSpawnPoint()
{
	// check to see if the currentSection is set and is active if so render
	if (m_currentWorldSection != nullptr && m_currentWorldSection->GetIsActive())
	{
		return m_currentWorldSection->GetPlayerStartSpawnPoint();
	}

	// check to see if the currentBuilding is set if so update
	if (m_currentBuilding != nullptr)
	{
		return m_currentBuilding->GetPlayerStartSpawnPoint();
	}

	// check to see if the currentAlley is set if so update
	if (m_currentAlley != nullptr)
	{
		return m_currentAlley->GetPlayerStartSpawnPoint();
	}

	return Vector2(0.0, 0.0);
}

void World::SetCurrentSection(string sectionID)
{
	// check to see if a worldSection exists with the specific ID
	auto sectionIndex = m_worldSections.find(sectionID);

	// if there was a match then set the currentWorldSection
	if (sectionIndex != m_worldSections.end())
	{
		m_currentWorldSection = sectionIndex->second;
	}
	// if not then set currentWorldSection to nullptr
	else
	{
		m_currentWorldSection = nullptr;
	}
}

void World::SetCurrentBuilding(string buildingID)
{
	// check to see if a Building exists with the specific ID
	auto buildingIndex = m_worldBuildings.find(buildingID);

	// if there was a match then set the currentBuilding
	if (buildingIndex != m_worldBuildings.end())
	{
		m_currentBuilding = buildingIndex->second;
	}
	// if not then set currentBuilding to nullptr
	else
	{
		m_currentBuilding = nullptr;
	}
}

void World::SetCurrentAlley(string alleyID)
{
	// check to see if an Alley exists with the specific ID
	auto alleyIndex = m_worldAlleys.find(alleyID);

	// if there was a match then set the currentAlley
	if (alleyIndex != m_worldAlleys.end())
	{
		m_currentAlley = alleyIndex->second;
	}
	// if not then set currentAlley to nullptr
	else
	{
		m_currentAlley = nullptr;
	}
}

void World::TriggerEventHandling(Trigger trigger)
{
	// check to see if the trigger beginning ID and Destination ID are the same
	if (trigger.GetStartWorldID() == trigger.GetDestinationWorldID())
	{
		// if this trigger is contained within this world
		if (trigger.GetTriggerType() == SectionToBuilding)
		{
			// Moving from a section to the inside of a building
			m_currentWorldSection->SetIsActive(false);
			SetCurrentBuilding(trigger.GetDestinationLocationID());
		}
		if (trigger.GetTriggerType() == BuildingToSection)
		{
			// Moving from the inside of a building back out to a section
			m_currentWorldSection->SetIsActive(true);
			m_currentBuilding = nullptr;
		}
		if (trigger.GetTriggerType() == SectionToAlley)
		{
			// Moving from a section to an alley way next to a building
			m_currentWorldSection->SetIsActive(false);
			SetCurrentAlley(trigger.GetDestinationLocationID());
		}
		if (trigger.GetTriggerType() == AlleyToSection)
		{
			// Moving from the alley back out to the section
			m_currentWorldSection->SetIsActive(true);
			m_currentAlley = nullptr;
		}
		if (trigger.GetTriggerType() == AlleyToBuilding)
		{
			// Moving from the alley to the inside of the buildng it is beside
			m_currentWorldSection->SetIsActive(false);
			SetCurrentBuilding(trigger.GetDestinationLocationID());
			m_currentAlley = nullptr;
		}
		if (trigger.GetTriggerType() == BuildingFloorToFloor)
		{
			// Moving inside the building from floor to floor
			m_currentBuilding->SetCurrentFloor(atoi(trigger.GetDestinationLocationID().c_str()));
		}
		if (trigger.GetTriggerType() == TutorialTrigger)
		{
			// Tutorial trigger that will cause an event to be fired
		}
		if (trigger.GetTriggerType() == CutScene)
		{
			// CutScene trigger that will cause a cutscene event to be fired
		}
	}
	// if they are not equal then pass the destinationWorldID back up to GlobalWorld
	else if (trigger.GetStartWorldID() != trigger.GetDestinationWorldID())
	{
		// if the trigger is to move from this world to another world
		if (trigger.GetTriggerType() == WorldToWorld)
		{
			// call back into the GlobalWorld Handler class and change the current world
			m_GLWorld->ChangeWorld(trigger.GetDestinationWorldID());
		}
	}
}

// change the velocity of all layers that are not auto scroll
void World::ChangeVelocity(Vector2 velocity)
{
	if (m_currentWorldSection->GetIsActive() && m_currentWorldSection != nullptr)
	{
		m_currentWorldSection->ChangeVelocity(velocity);
	}

	else if (m_currentAlley->GetIsActive() && m_currentAlley != nullptr)
	{
		m_currentAlley->ChangeVelocity(velocity);
	}

	else if (m_currentBuilding->GetIsActive() && m_currentAlley != nullptr)
	{
		m_currentBuilding->ChangeVelocity(velocity);
	}
}

// Check the point to see if world position is a moveable spot or not
// true = moveable false = not moveable
bool World::CheckPointForCollission(Vector2 screenPosition)
{
	if (m_currentWorldSection->GetIsActive() && m_currentWorldSection != nullptr)
	{
		return m_currentWorldSection->CheckPointForCollission(screenPosition);
	}

	else if (m_currentAlley->GetIsActive() && m_currentAlley != nullptr)
	{
		return m_currentAlley->CheckPointForCollission(screenPosition);
	}

	else if (m_currentBuilding->GetIsActive() && m_currentAlley != nullptr)
	{
		return m_currentBuilding->CheckPointForCollission(screenPosition);
	}
	return false;
}

// check the point to see if the world position is a trigger
// if its a trigger return the Trigger
Trigger World::CheckPointForTrigger(Vector2 screenPosition)
{
	if (m_currentWorldSection->GetIsActive() && m_currentWorldSection != nullptr)
	{
		return m_currentWorldSection->CheckPointForTrigger(screenPosition);
	}

	else if (m_currentAlley->GetIsActive() && m_currentAlley != nullptr)
	{
		return m_currentAlley->CheckPointForTrigger(screenPosition);
	}

	else if (m_currentBuilding->GetIsActive() && m_currentAlley != nullptr)
	{
		return m_currentBuilding->CheckPointForTrigger(screenPosition);
	}
	return Trigger();
}

GlobalWorld::GlobalWorld()
{
	m_gameWorld.clear();
}

GlobalWorld::~GlobalWorld()
{
	m_gameWorld.clear();
}

void GlobalWorld::BuildWorlds(string textFile)
{
	int			numberOfWorlds = 0;	// temp variable to hold the number of Worlds
	string		tempString;			// temp variable to hold line read in from file
	ifstream	inFile(textFile);	// file reader variable
	
	// check to see if the file can be open successfully if not then do nothing
	if (inFile)
	{
		// Read a line in and convert to Integer and place into numberOfWorlds
		getline(inFile, tempString);
		numberOfWorlds = atoi(tempString.c_str());
		// Loop through several lines and use data to Build the Worlds
		for (int i = 0; i < numberOfWorlds; i++)
		{
			getline(inFile, tempString);
			World* newWorld = new World();
			newWorld->BuildWorld(tempString, this);
			m_gameWorld.insert(pair<string, World*>(newWorld->GetWorldID(), newWorld));
		}
	}
	inFile.close();
}

void GlobalWorld::ChangeWorld(string worldID)
{
	// check to see if the worldID passed in exists in the container
	auto worldIndex = m_gameWorld.find(worldID);

	// if it exists then change the currentWorld equal to the one that matches
	if (worldIndex != m_gameWorld.end())
	{
		m_currentWorld = worldIndex->second;
	}
	// otherwise set the currentWorld = nullptr
	else
	{
		m_currentWorld = nullptr;
	}
}

void GlobalWorld::UpdateWorld(float timeDelta)
{
	// update the current World
	m_currentWorld->UpdateWorld(timeDelta);
}

void GlobalWorld::RenderWorld()
{
	// render the currentWorld
	m_currentWorld->RenderWorld();
}