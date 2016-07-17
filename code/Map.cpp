/*=====================================================================================
$File: Map.cpp
$Date: February 8, 2016
$Creator: Jamie Cooper
$Notice: (C) Copyright 2015 by Punch Drunk Squirrel Games LLC. All Rights Reserved.
=====================================================================================*/
#include "Map.h"

Map::Map()
{
}

Map::~Map()
{
}

void Map::InitializeMap(RenderManager *graphics, InputManager *input, string mapTextFile)
{
	m_graphicSystem = graphics;
	m_input = input;
	m_mapFileName = mapTextFile;
}

void Map::BuildMap()
{
	int numberOfSections = 0;
	string tempString;

	ifstream inFile(m_mapFileName);
	if (inFile)
	{
		getline(inFile, m_mapName);
		getline(inFile, tempString);
		numberOfSections = atoi(tempString.c_str());
		for (int i = 0; i < numberOfSections; i++)
		{
			getline(inFile, tempString);
			MapSection newSection;
			newSection.BuildMapSection(m_graphicSystem, tempString);
			m_mapSections.push_back(newSection);
		}
	}
	inFile.close();
	SetCurrentMapSection(0);
}

void Map::SetupInput()
{
	function<void(bool)> funcPoint = bind(&Map::CloseGame, this, placeholders::_1);
	m_input->AddKeyboardCommand(Keyboard::Keys::Escape, funcPoint);
	m_input->AddGamePadButtonCommand(Back, funcPoint);

	function<void(bool)> funcPointLeft = bind(&Map::MoveMapLeft, this, placeholders::_1);
	m_input->AddKeyboardCommand(Keyboard::Keys::Left, funcPointLeft);
	m_input->AddGamePadDpadCommand(Left, funcPointLeft);

	function<void(bool)> funcPointRight = bind(&Map::MoveMapRight, this, placeholders::_1);
	m_input->AddKeyboardCommand(Keyboard::Keys::Right, funcPointRight);
	m_input->AddGamePadDpadCommand(Right, funcPointRight);
}

void Map::MoveMapLeft(bool move)
{
	if (move)
		m_currentSection.UpdateVelocity(-1);
	else
		m_currentSection.UpdateVelocity(0);
}

void Map::MoveMapRight(bool move)
{
	if (move)
		m_currentSection.UpdateVelocity(1);
	else
		m_currentSection.UpdateVelocity(0);
}

void Map::UpdateMap(float timeDelta)
{
	m_currentSection.UpdateMapSection(timeDelta);
}

void Map::DrawMap()
{
	m_currentSection.DrawMapSection();
}

void Map::SetCurrentMapSection(int mapSection)
{
	m_currentSection = m_mapSections[mapSection];
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

				getline(inFile, tempString);
				if (tempString == "Money")
					tempObject.m_objectType = Money;
				else if (tempString == "LorePiece")
					tempObject.m_objectType = LorePiece;
				else if (tempString == "Upgrade")
					tempObject.m_objectType = Upgrade;

				tempFloor.m_floorObjects.push_back(tempObject);
			}

			getline(inFile, tempString);
			numberOfTriggers = atoi(tempString.c_str());
			for (int trigger = 0; trigger < numberOfTriggers; trigger++)
			{
				getline(inFile, tempTrigger.m_name);
				getline(inFile, tempString);
				if (tempString == "BuildingTrigger")
					tempTrigger.m_triggerType = BuildingTrigger;
				else if (tempString == "BlockTrigger")
					tempTrigger.m_triggerType = BlockTrigger;
				else if (tempString == "FloorTrigger")
					tempTrigger.m_triggerType = FloorTrigger;

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

void MapSection::BuildMapSection(RenderManager *graphics, string fileName)
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
			tempLayer.m_sourceRectangle.top = 0;
			tempLayer.m_sourceRectangle.left = 0;
			tempLayer.m_sourceRectangle.bottom = 1080;
			tempLayer.m_sourceRectangle.right = 1920;
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


			getline(inFile, tempString);
			if (tempString == "Money")
				tempObject.m_objectType = Money;
			else if (tempString == "LorePiece")
				tempObject.m_objectType = LorePiece;
			else if (tempString == "Upgrade")
				tempObject.m_objectType = Upgrade;

			m_objects.push_back(tempObject);
		}

		getline(inFile, tempString);
		m_numberOfTriggers = atoi(tempString.c_str());
		for (int trigger = 0; trigger < m_numberOfTriggers; trigger++)
		{
			getline(inFile, tempTrigger.m_name);
			getline(inFile, tempString);
			if (tempString == "BuildingTrigger")
				tempTrigger.m_triggerType = BuildingTrigger;
			else if (tempString == "BlockTrigger")
				tempTrigger.m_triggerType = BlockTrigger;
			else if (tempString == "FloorTrigger")
				tempTrigger.m_triggerType = FloorTrigger;

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
		if (m_triggerPoints[triggers].m_triggerType == BuildingTrigger)
		{
			tempBuilding.BuildBuilding(m_triggerPoints[triggers].m_name);
			m_buildings.push_back(tempBuilding);
		}
	}
}

void MapSection::UpdateVelocity(int value)
{
	vector<SectionLayer>::iterator layerIterator;

	for (layerIterator = m_layers.begin(); layerIterator != m_layers.end(); layerIterator++)
	{
		if (!(layerIterator)->m_autoScroll)
			(layerIterator)->m_velocity = (layerIterator)->m_scrollSpeed * value;
	}
}

void MapSection::UpdateMapSection(float delta)
{
	vector<SectionLayer>::iterator layerIterator;

	for (layerIterator = m_layers.begin(); layerIterator != m_layers.end(); layerIterator++)
	{
		(layerIterator)->m_sourceRectangle.left += (LONG)((layerIterator)->m_velocity * delta);
		(layerIterator)->m_sourceRectangle.right += (LONG)((layerIterator)->m_velocity * delta);

		if (!(layerIterator)->m_autoScroll)
		{
			if ((layerIterator)->m_sourceRectangle.left < 0)
			{
				(layerIterator)->m_sourceRectangle.left = 0;
				(layerIterator)->m_sourceRectangle.right = (layerIterator)->m_sourceRectangle.left + m_graphicSystem->GetGameWidth();
			}

			if ((layerIterator)->m_sourceRectangle.right >(layerIterator)->m_width)
			{
				(layerIterator)->m_sourceRectangle.right = (layerIterator)->m_width;
				(layerIterator)->m_sourceRectangle.left = (layerIterator)->m_sourceRectangle.right - m_graphicSystem->GetGameWidth();
			}
		}
		else
		{
			if ((layerIterator)->m_sourceRectangle.right > (layerIterator)->m_width)
			{
				(layerIterator)->m_sourceRectangle.left = 0;
				(layerIterator)->m_sourceRectangle.right = m_graphicSystem->GetGameWidth();
			}
		}
	}
}

void MapSection::DrawMapSection()
{
	vector<SectionLayer>::iterator layerIterator;
	vector<Object>::iterator objectIterator;

	for (layerIterator = m_layers.begin(); layerIterator != m_layers.end(); layerIterator++)
	{
		m_graphicSystem->RenderObject((layerIterator)->m_textureName, (layerIterator)->m_sourceRectangle);
	}

	for (objectIterator = m_objects.begin(); objectIterator != m_objects.end(); objectIterator++)
	{
		m_graphicSystem->RenderObject((objectIterator)->m_textureName, (objectIterator)->m_position);
	}
}