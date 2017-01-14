/*=====================================================================================
$File: World.cpp
$Date: February 8, 2016
$Creator: Jamie Cooper
$Notice: (C) Copyright 2015 by Punch Drunk Squirrel Games LLC. All Rights Reserved.
=====================================================================================*/
#include "World.h"

World::World()
{
}

World::~World()
{
}

void World::InitializeWorld(RenderManager *graphics, InputManager *input, string worldTextFile)
{
	m_graphicSystem = graphics;
	m_input = input;
	m_worldFileName = worldTextFile;
}

void World::BuildWorld()
{
	int numberOfSections = 0;
	string tempString;

	ifstream inFile(m_worldFileName);
	if (inFile)
	{
		getline(inFile, m_worldName);
		getline(inFile, tempString);
		numberOfSections = atoi(tempString.c_str());
		for (int i = 0; i < numberOfSections; i++)
		{
			getline(inFile, tempString);
			WorldSection newSection;
			newSection.BuildWorldSection(m_graphicSystem, tempString);
			m_worldSections.push_back(newSection);
		}
	}
	inFile.close();
	SetCurrentWorldSection(0);
}

void World::MoveWorld(bool move, GameActions action)
{
	if (move && action == ActionLeft)
		m_currentSection.UpdateVelocity(-1);
	else if (move && action == ActionRight)
		m_currentSection.UpdateVelocity(1);
	else
		m_currentSection.UpdateVelocity(0);
}

Vector2 World::CheckCollission(Vector2 position, Vector2 velocity, float movementSpeed)
{
	if (((position.x + movementSpeed) * velocity.x) < 0)
		position.x = 0;
	
	if (((position.x + movementSpeed) * velocity.x) > m_gameWidth)
		position.x = m_gameWidth;

	if (((position.y + movementSpeed) * velocity.y) < 0)
		position.y = 0;

	if (((position.y + movementSpeed) * velocity.y) > m_gameHeight)
		position.y = m_gameHeight;

	return position;
}

TriggerType World::CheckTriggerCollision(Vector2 position)
{
	return TriggerType();
}

void World::UpdateWorld(float timeDelta)
{
	m_currentSection.UpdateWorldSection(timeDelta);
}

void World::DrawWorld()
{
	m_currentSection.DrawWorldSection();
}

void World::SetCurrentWorldSection(int mapSection)
{
	m_currentSection = m_worldSections[mapSection];
}

void Building::BuildBuilding(string fileName)
{
	int columns = 0;
	int rows = 0;
	int numberOfObjects = 0;
	int numberOfTriggers = 0;
	Object tempObject;
	TriggerPoint tempTrigger;
	BuildingFloor tempFloor;
	string tempString;

	ifstream inFile(fileName);
	if (inFile)
	{
		getline(inFile, m_textureName);
		getline(inFile, tempString);
		columns = atoi(tempString.c_str());
		getline(inFile, tempString);
		rows = atoi(tempString.c_str());
		getline(inFile, tempString);
		m_numberOfFloors = atoi(tempString.c_str());
		for (int floor = 0; floor < m_numberOfFloors; floor++)
		{
			tempFloor.m_columns = columns;
			tempFloor.m_rows = rows;

			for (int row = 0; row < rows; row++)
			{
				getline(inFile, tempString);
				istringstream ss(tempString);

				while (ss)
				{
					string s2;
					if (!getline(ss, s2, ','))
						break;

					tempFloor.m_tileSet.push_back(atoi(s2.c_str()));
				}
			}

			getline(inFile, tempString);
			numberOfObjects = atoi(tempString.c_str());
			for (int object = 0; object < numberOfObjects; object++)
			{
				getline(inFile, tempObject.m_name);
				getline(inFile, tempObject.m_textureName);
				getline(inFile, tempString);
				tempObject.m_tileIndex = atoi(tempString.c_str());

				getline(inFile, tempString);
				istringstream ss(tempString);

				string s2;

				getline(ss, s2, ',');
				tempObject.m_position.x = (float)atof(s2.c_str());
				getline(ss, s2, ',');
				tempObject.m_position.y = (float)atof(s2.c_str());
				tempFloor.m_floorObjects.push_back(tempObject);
			}

			getline(inFile, tempString);
			numberOfTriggers = atoi(tempString.c_str());
			for (int trigger = 0; trigger < numberOfTriggers; trigger++)
			{
				getline(inFile, tempTrigger.m_name);
				getline(inFile, tempString);
				if (tempString == "BuildingFloorToFloor")
					tempTrigger.m_triggerType = BuildingFloorToFloor;
				else if (tempString == "Tutorial")
					tempTrigger.m_triggerType = TutorialTrigger;
				else if (tempString == "CutScene")
					tempTrigger.m_triggerType = CutScene;

				getline(inFile, tempString);
				istringstream ss(tempString);

				string s2;
				getline(ss, s2, ',');
				tempTrigger.m_mapPosition.x = (float)atof(s2.c_str());
				getline(ss, s2, ',');
				tempTrigger.m_mapPosition.y = (float)atof(s2.c_str());


				getline(inFile, tempString);
				istringstream ss2(tempString);

				getline(ss2, s2, ',');
				tempTrigger.m_moveToPosition.x = (float)atof(s2.c_str());
				getline(ss2, s2, ',');
				tempTrigger.m_moveToPosition.y = (float)atof(s2.c_str());

				tempFloor.m_floorTriggers.push_back(tempTrigger);
			}

			m_floors.push_back(tempFloor);
		}
	}
	inFile.close();
}

void Building::DrawBuilding()
{

}

void WorldSection::BuildWorldSection(RenderManager *graphics, string fileName)
{
	string tempString;
	SectionLayer tempLayer;
	Object tempObject;
	TriggerPoint tempTrigger;
	Building tempBuilding;
	D3D11_TEXTURE2D_DESC textDesc;
	m_graphicSystem = graphics;

	ifstream inFile(fileName);
	if (inFile)
	{
		getline(inFile, tempString);
		m_numberOfLayers = atoi(tempString.c_str());
		for (int layer = 0; layer < m_numberOfLayers; layer++)
		{
			getline(inFile, tempLayer.m_textureName);
			getline(inFile, tempString);
			tempLayer.m_scrollSpeed = (float)atof(tempString.c_str());
			getline(inFile, tempString);
			if (tempString == "TRUE")
				tempLayer.m_autoScroll = true;
			else
				tempLayer.m_autoScroll = false;
			tempLayer.m_sourceRectangle.x = 0;
			tempLayer.m_sourceRectangle.y = 0;
			tempLayer.m_sourceRectangle.width = m_gameWidth;
			tempLayer.m_sourceRectangle.height = m_gameHeight;
			textDesc = m_graphicSystem->getTextureDesc(tempLayer.m_textureName);
			tempLayer.m_width = textDesc.Width;
			tempLayer.m_height = textDesc.Height;
			if (tempLayer.m_autoScroll)
				tempLayer.m_velocity = tempLayer.m_scrollSpeed;
			else
				tempLayer.m_velocity = 0;

			m_layers.push_back(tempLayer);
		}

		getline(inFile, tempString);
		m_numberOfObjects = atoi(tempString.c_str());
		for (int object = 0; object < m_numberOfObjects; object++)
		{
			getline(inFile, tempObject.m_name);
			getline(inFile, tempObject.m_textureName);
			getline(inFile, tempString);
			tempObject.m_tileIndex = atoi(tempString.c_str());

			getline(inFile, tempString);
			istringstream ss(tempString);

			string s2;

			getline(ss, s2, ',');
			tempObject.m_position.x = (float)atof(s2.c_str());
			getline(ss, s2, ',');
			tempObject.m_position.y = (float)atof(s2.c_str());
			m_objects.push_back(tempObject);
		}

		getline(inFile, tempString);
		m_numberOfTriggers = atoi(tempString.c_str());
		for (int trigger = 0; trigger < m_numberOfTriggers; trigger++)
		{
			getline(inFile, tempTrigger.m_name);
			getline(inFile, tempString);
			if (tempString == "WorldToWorld")
				tempTrigger.m_triggerType = WorldToWorld;
			else if (tempString == "WorldToBuilding")
				tempTrigger.m_triggerType = WorldToBuilding;
			else if (tempString == "Tutorial")
				tempTrigger.m_triggerType = TutorialTrigger;
			else if (tempString == "CutScene")
				tempTrigger.m_triggerType = CutScene;

			getline(inFile, tempString);
			istringstream ss(tempString);

			string s2;
			getline(ss, s2, ',');
			tempTrigger.m_mapPosition.x = (float)atof(s2.c_str());
			getline(ss, s2, ',');
			tempTrigger.m_mapPosition.y = (float)atof(s2.c_str());

			getline(inFile, tempString);
			istringstream ss2(tempString);

			getline(ss2, s2, ',');
			tempTrigger.m_moveToPosition.x = (float)atof(s2.c_str());
			getline(ss2, s2, ',');
			tempTrigger.m_moveToPosition.y = (float)atof(s2.c_str());

			m_triggerPoints.push_back(tempTrigger);
		}
	}
	inFile.close();

	for (int triggers = 0; triggers < m_numberOfTriggers; triggers++)
	{
		if (m_triggerPoints[triggers].m_triggerType == WorldToBuilding)
		{
			tempBuilding.BuildBuilding(m_triggerPoints[triggers].m_name);
			m_buildings.push_back(tempBuilding);
		}
	}
}

void WorldSection::UpdateVelocity(int value)
{
	vector<SectionLayer>::iterator layerIterator;

	for (layerIterator = m_layers.begin(); layerIterator != m_layers.end(); layerIterator++)
	{
		if (!(layerIterator)->m_autoScroll)
			(layerIterator)->m_velocity = (layerIterator)->m_scrollSpeed * value;
	}
}

void WorldSection::UpdateWorldSection(float delta)
{
	vector<SectionLayer>::iterator layerIterator;

	for (layerIterator = m_layers.begin(); layerIterator != m_layers.end(); layerIterator++)
	{
		(layerIterator)->m_sourceRectangle.x += (LONG)((layerIterator)->m_velocity * delta);
		(layerIterator)->m_sourceRectangle.width += (LONG)((layerIterator)->m_velocity * delta);

		if (!(layerIterator)->m_autoScroll)
		{
			if ((layerIterator)->m_sourceRectangle.x < 0)
			{
				(layerIterator)->m_sourceRectangle.x = 0;
				(layerIterator)->m_sourceRectangle.width = (layerIterator)->m_sourceRectangle.y + m_graphicSystem->GetGameWidth();
			}

			if ((layerIterator)->m_sourceRectangle.width >(layerIterator)->m_width)
			{
				(layerIterator)->m_sourceRectangle.width = (layerIterator)->m_width;
				(layerIterator)->m_sourceRectangle.x = (layerIterator)->m_sourceRectangle.width - m_graphicSystem->GetGameWidth();
			}
		}
		else
		{
			if ((layerIterator)->m_sourceRectangle.width > (layerIterator)->m_width)
			{
				(layerIterator)->m_sourceRectangle.x = 0;
				(layerIterator)->m_sourceRectangle.width = m_graphicSystem->GetGameWidth();
			}
		}
	}
}

TriggerType WorldSection::CheckCollision(Vector2 position)
{
	SimpleMath::Rectangle triggerVolume;
	vector<TriggerPoint>::iterator triggerIterator;

	for (triggerIterator = m_triggerPoints.begin(); triggerIterator != m_triggerPoints.end(); triggerIterator++)
	{
		triggerVolume.x = triggerIterator->m_mapPosition.x - 2;
		triggerVolume.y = triggerIterator->m_mapPosition.y - 2;
		triggerVolume.width = triggerIterator->m_mapPosition.x + 2;
		triggerVolume.height = triggerIterator->m_mapPosition.y + 2;

		if (triggerVolume.Contains(position))
			return triggerIterator->m_triggerType;
	}
	return TriggerNone;
}

void WorldSection::DrawWorldSection()
{
	vector<SectionLayer>::iterator layerIterator;
	vector<Object>::iterator objectIterator;

	for (layerIterator = m_layers.begin(); layerIterator != m_layers.end(); layerIterator++)
	{
		m_graphicSystem->RenderObject((layerIterator)->m_textureName, (layerIterator)->m_sourceRectangle, Vector2(0.0, 0.0));
	}

	for (objectIterator = m_objects.begin(); objectIterator != m_objects.end(); objectIterator++)
	{
		m_graphicSystem->RenderObject((objectIterator)->m_textureName, (objectIterator)->m_position);
	}
}