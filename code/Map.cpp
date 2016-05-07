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

void Map::InitializeMap(DX11RenderManager *graphics, Keyboard *keyboard, GamePad *gamePad, int screenWidth, int screenHeight)
{
	m_graphicSystem = graphics;
	m_keyboard = keyboard;
	m_gamePad = gamePad;
	m_currentPosition.top = 0;
	m_currentPosition.left = 0;
	m_currentPosition.right = screenWidth;
	m_currentPosition.bottom = screenHeight;
}

void Map::BuildMap(std::string mapTextFile)
{
	std::string tempString;

	std::ifstream inFile(mapTextFile);
	if (inFile)
	{
		std::getline(inFile, m_mapName);
		std::getline(inFile, tempString);
		m_numberOfMapSections = std::atoi(tempString.c_str());
		for (int i = 0; i < m_numberOfMapSections; i++)
		{
			std::getline(inFile, tempString);
			m_sectionNames.push_back(tempString);
		}
	}
	inFile.close();

	for (int i = 0; i < m_numberOfMapSections; i++)
	{
		MapSection newSection;
		newSection.BuildMapSection(m_graphicSystem, m_sectionNames[i]);
		m_mapSections.push_back(newSection);
	}
}

void Map::UpdateMap(float timeDelta)
{
	bool left;;
	bool keyboardEntry;
	left = false;
	keyboardEntry = false;
	auto gamePadState = m_gamePad->GetState(0);
	auto keyboardState = m_keyboard->GetState();

	if (keyboardState.Right || gamePadState.IsDPadRightPressed())
	{
		left = false;
		keyboardEntry = true;
	}
	else if (keyboardState.Left || gamePadState.IsDPadLeftPressed())
	{
		left = true;
		keyboardEntry = true;
	}

	m_currentPosition = m_currentSection.UpdateMapSection(timeDelta, left, keyboardEntry);
}

void Map::DrawMap()
{
	m_currentSection.DrawMapSection();
}

void Map::SetCurrentMapSection(int mapSection)
{
	m_currentSection = m_mapSections[mapSection];
}

void Building::BuildBuilding(std::string fileName)
{
	int columns = 0;
	int rows = 0;
	int numberOfObjects = 0;
	int numberOfTriggers = 0;
	Object tempObject;
	TriggerPoint tempTrigger;
	BuildingFloor tempFloor;
	std::string tempString;
	
	std::ifstream inFile(fileName);
	if (inFile)
	{
		std::getline(inFile, m_textureName);
		std::getline(inFile, tempString);
		columns = std::atoi(tempString.c_str());
		std::getline(inFile, tempString);
		rows = std::atoi(tempString.c_str());
		std::getline(inFile, tempString);
		m_numberOfFloors = std::atoi(tempString.c_str());
		for (int floor = 0; floor < m_numberOfFloors; floor++)
		{
			tempFloor.m_columns = columns;
			tempFloor.m_rows = rows;

			for (int row = 0; row < rows; row++)
			{
				std::getline(inFile, tempString);
				std::istringstream ss(tempString);

				while (ss)
				{
					std::string s2;
					if (!std::getline(ss, s2, ','))
						break;

					tempFloor.m_tileSet.push_back(std::atoi(s2.c_str()));
				}
			}

			std::getline(inFile, tempString);
			numberOfObjects = std::atoi(tempString.c_str());
			for (int object = 0; object < numberOfObjects; object++)
			{
				std::getline(inFile, tempObject.m_name);
				std::getline(inFile, tempObject.m_textureName);
				std::getline(inFile, tempString);
				tempObject.m_tileIndex = std::atoi(tempString.c_str());

				std::getline(inFile, tempString);
				std::istringstream ss(tempString);

				std::string s2;

				std::getline(ss, s2, ',');
				tempObject.m_position.x = std::atof(s2.c_str());
				std::getline(ss, s2, ',');
				tempObject.m_position.y = std::atof(s2.c_str());

				std::getline(inFile, tempString);
				if (tempString == "Money")
					tempObject.m_objectType = Money;
				else if (tempString == "LorePiece")
					tempObject.m_objectType = LorePiece;
				else if (tempString == "Upgrade")
					tempObject.m_objectType = Upgrade;

				tempFloor.m_floorObjects.push_back(tempObject);
			}

			std::getline(inFile, tempString);
			numberOfTriggers = std::atoi(tempString.c_str());
			for (int trigger = 0; trigger < numberOfTriggers; trigger++)
			{
				std::getline(inFile, tempTrigger.m_name);
				std::getline(inFile, tempString);
				if (tempString == "BuildingTrigger")
					tempTrigger.m_triggerType = BuildingTrigger;
				else if (tempString == "BlockTrigger")
					tempTrigger.m_triggerType = BlockTrigger;
				else if (tempString == "FloorTrigger")
					tempTrigger.m_triggerType = FloorTrigger;

				std::getline(inFile, tempString);
				std::istringstream ss(tempString);

				std::string s2;
				std::getline(ss, s2, ',');
				tempTrigger.m_mapPosition.x = std::atof(s2.c_str());
				std::getline(ss, s2, ',');
				tempTrigger.m_mapPosition.y = std::atof(s2.c_str());


				std::getline(inFile, tempString);
				std::istringstream ss2(tempString);

				std::getline(ss2, s2, ',');
				tempTrigger.m_moveToPosition.x = std::atof(s2.c_str());
				std::getline(ss2, s2, ',');
				tempTrigger.m_moveToPosition.y = std::atof(s2.c_str());

				tempFloor.m_floorTriggers.push_back(tempTrigger);
			}

			m_floors.push_back(tempFloor);
		}
	}
	inFile.close();
}

void MapSection::BuildMapSection(DX11RenderManager *graphics, std::string fileName)
{
	std::string tempString;
	SectionLayer tempLayer;
	Object tempObject;
	TriggerPoint tempTrigger;
	Building tempBuilding;
	D3D11_TEXTURE2D_DESC textDesc;
	m_graphicSystem = graphics;

	std::ifstream inFile(fileName);
	if (inFile)
	{
		std::getline(inFile, tempString);
		m_numberOfLayers = std::atoi(tempString.c_str());
		for (int layer = 0; layer < m_numberOfLayers; layer++)
		{
			std::getline(inFile, tempLayer.m_textureName);
			std::getline(inFile, tempString);
			tempLayer.m_scrollSpeed = std::atof(tempString.c_str());
			std::getline(inFile, tempString);
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
			m_layers.push_back(tempLayer);
		}
		
		std::getline(inFile, tempString);
		m_numberOfObjects = std::atoi(tempString.c_str());
		for (int object = 0; object < m_numberOfObjects; object++)
		{
			std::getline(inFile, tempObject.m_name);
			std::getline(inFile, tempObject.m_textureName);
			std::getline(inFile, tempString);
			tempObject.m_tileIndex = std::atoi(tempString.c_str());

			std::getline(inFile, tempString);
			std::istringstream ss(tempString);

			std::string s2;

			std::getline(ss, s2, ',');
			tempObject.m_position.x = std::atof(s2.c_str());
			std::getline(ss, s2, ',');
			tempObject.m_position.y = std::atof(s2.c_str());


			std::getline(inFile, tempString);
			if (tempString == "Money")
				tempObject.m_objectType = Money;
			else if (tempString == "LorePiece")
				tempObject.m_objectType = LorePiece;
			else if (tempString == "Upgrade")
				tempObject.m_objectType = Upgrade;

			m_objects.push_back(tempObject);
		}

		std::getline(inFile, tempString);
		m_numberOfTriggers = std::atoi(tempString.c_str());
		for (int trigger = 0; trigger < m_numberOfTriggers; trigger++)
		{
			std::getline(inFile, tempTrigger.m_name);
			std::getline(inFile, tempString);
			if (tempString == "BuildingTrigger")
				tempTrigger.m_triggerType = BuildingTrigger;
			else if (tempString == "BlockTrigger")
				tempTrigger.m_triggerType = BlockTrigger;
			else if (tempString == "FloorTrigger")
				tempTrigger.m_triggerType = FloorTrigger;

			std::getline(inFile, tempString);
			std::istringstream ss(tempString);

			std::string s2;
			std::getline(ss, s2, ',');
			tempTrigger.m_mapPosition.x = std::atof(s2.c_str());
			std::getline(ss, s2, ',');
			tempTrigger.m_mapPosition.y = std::atof(s2.c_str());

			std::getline(inFile, tempString);
			std::istringstream ss2(tempString);

			std::getline(ss2, s2, ',');
			tempTrigger.m_moveToPosition.x = std::atof(s2.c_str());
			std::getline(ss2, s2, ',');
			tempTrigger.m_moveToPosition.y = std::atof(s2.c_str());

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

RECT MapSection::UpdateMapSection(float delta, bool left, bool keyboardEntry)
{
	for (int layers = 0; layers < m_layers.size(); layers++)
	{
		if (m_layers[layers].m_autoScroll)
		{
			m_layers[layers].m_sourceRectangle.left += m_layers[layers].m_scrollSpeed * delta;
			m_layers[layers].m_sourceRectangle.right += m_layers[layers].m_scrollSpeed * delta;

			if (m_layers[layers].m_sourceRectangle.right >= m_layers[layers].m_width)
			{
				m_layers[layers].m_sourceRectangle.left = 0;
				m_layers[layers].m_sourceRectangle.right = m_graphicSystem->getWidth();
			}
		}
		else if (!m_layers[layers].m_autoScroll && keyboardEntry)
		{
			if (!left)
			{
				m_layers[layers].m_sourceRectangle.left += m_layers[layers].m_scrollSpeed * delta;
				m_layers[layers].m_sourceRectangle.right += m_layers[layers].m_scrollSpeed * delta;
				if (m_layers[layers].m_sourceRectangle.right >= m_layers[layers].m_width)
				{
					m_layers[layers].m_sourceRectangle.left = m_layers[layers].m_width - m_graphicSystem->getWidth();
					m_layers[layers].m_sourceRectangle.right = m_layers[layers].m_width;
				}
			}
			else
			{
				m_layers[layers].m_sourceRectangle.left -= m_layers[layers].m_scrollSpeed * delta;
				m_layers[layers].m_sourceRectangle.right -= m_layers[layers].m_scrollSpeed * delta;
				if (m_layers[layers].m_sourceRectangle.left <= 0)
				{
					m_layers[layers].m_sourceRectangle.left = 0;
					m_layers[layers].m_sourceRectangle.right = m_graphicSystem->getWidth();
				}
			}
		}
	}
	return m_layers[m_layers.size() - 1].m_sourceRectangle;
}

void MapSection::DrawMapSection()
{
	for (int layers = 0; layers < m_layers.size(); layers++)
	{
		m_graphicSystem->DrawObject(m_layers[layers].m_textureName, m_layers[layers].m_sourceRectangle);
	}

	for (int objects = 0; objects < m_objects.size(); objects++)
	{
		m_graphicSystem->DrawObject(m_objects[objects].m_textureName, m_objects[objects].m_position);
	}
}