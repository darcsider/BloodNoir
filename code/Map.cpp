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

void Map::InitializeMap(DX11RenderManager *graphics, InputHandler *input, int screenWidth, int screenHeight)
{
	m_graphicSystem = graphics;
	m_input = input;
	m_currentPosition.top = 0;
	m_currentPosition.left = 0;
	m_currentPosition.right = screenWidth;
	m_currentPosition.bottom = screenHeight;

	// this stuff here is temporary while I continue to work on finishing various systems

	Command *testCommand = new Command();
	testCommand->setKeyboardKeyBinding(Keyboard::Keys::Left);

	function<void(bool)> funcPointLeft = bind(&Map::MoveMapLeft, this, placeholders::_1);
	testCommand->setCallbackFunction(funcPointLeft);

	m_input->AddCommand(testCommand);

	Command *testCommand2 = new Command();
	testCommand2->setKeyboardKeyBinding(Keyboard::Keys::Right);
	
	function<void(bool)> funcPointRight = bind(&Map::MoveMapRight, this, placeholders::_1);
	testCommand2->setCallbackFunction(funcPointRight);

	m_input->AddCommand(testCommand2);
}

void Map::BuildMap(string mapTextFile)
{
	string tempString;

	ifstream inFile(mapTextFile);
	if (inFile)
	{
		getline(inFile, m_mapName);
		getline(inFile, tempString);
		m_numberOfMapSections = atoi(tempString.c_str());
		for (int i = 0; i < m_numberOfMapSections; i++)
		{
			getline(inFile, tempString);
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
	{
		m_currentSection.UpdateVelocity(1);
	}
	else
		m_currentSection.UpdateVelocity(0);
}

void Map::UpdateMap(float timeDelta)
{
	m_currentPosition = m_currentSection.UpdateMapSection(timeDelta);
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
				tempObject.m_position.x = atof(s2.c_str());
				getline(ss, s2, ',');
				tempObject.m_position.y = atof(s2.c_str());

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
				tempTrigger.m_mapPosition.x = atof(s2.c_str());
				getline(ss, s2, ',');
				tempTrigger.m_mapPosition.y = atof(s2.c_str());


				getline(inFile, tempString);
				istringstream ss2(tempString);

				getline(ss2, s2, ',');
				tempTrigger.m_moveToPosition.x = atof(s2.c_str());
				getline(ss2, s2, ',');
				tempTrigger.m_moveToPosition.y = atof(s2.c_str());

				tempFloor.m_floorTriggers.push_back(tempTrigger);
			}

			m_floors.push_back(tempFloor);
		}
	}
	inFile.close();
}

void MapSection::BuildMapSection(DX11RenderManager *graphics, string fileName)
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
			tempLayer.m_scrollSpeed = atof(tempString.c_str());
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
			tempObject.m_position.x = atof(s2.c_str());
			getline(ss, s2, ',');
			tempObject.m_position.y = atof(s2.c_str());


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
			tempTrigger.m_mapPosition.x = atof(s2.c_str());
			getline(ss, s2, ',');
			tempTrigger.m_mapPosition.y = atof(s2.c_str());

			getline(inFile, tempString);
			istringstream ss2(tempString);

			getline(ss2, s2, ',');
			tempTrigger.m_moveToPosition.x = atof(s2.c_str());
			getline(ss2, s2, ',');
			tempTrigger.m_moveToPosition.y = atof(s2.c_str());

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
	for (int i = 0; i < m_layers.size(); i++)
	{
		if (!m_layers[i].m_autoScroll)
			m_layers[i].m_velocity = m_layers[i].m_scrollSpeed * value;
	}
}

RECT MapSection::UpdateMapSection(float delta)
{
	for (int layers = 0; layers < m_layers.size(); layers++)
	{
		m_layers[layers].m_sourceRectangle.left += m_layers[layers].m_velocity * delta;
		m_layers[layers].m_sourceRectangle.right += m_layers[layers].m_velocity * delta;

		if (!m_layers[layers].m_autoScroll)
		{
			if (m_layers[layers].m_sourceRectangle.left < 0)
			{
				m_layers[layers].m_sourceRectangle.left = 0;
				m_layers[layers].m_sourceRectangle.right = m_layers[layers].m_sourceRectangle.left + m_graphicSystem->getWidth();
			}

			if (m_layers[layers].m_sourceRectangle.right > m_layers[layers].m_width)
			{
				m_layers[layers].m_sourceRectangle.right = m_layers[layers].m_width;
				m_layers[layers].m_sourceRectangle.left = m_layers[layers].m_sourceRectangle.right - m_graphicSystem->getWidth();
			}
		}
		else
		{
			if (m_layers[layers].m_sourceRectangle.right > m_layers[layers].m_width)
			{
				m_layers[layers].m_sourceRectangle.left = 0;
				m_layers[layers].m_sourceRectangle.right = m_graphicSystem->getWidth();
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