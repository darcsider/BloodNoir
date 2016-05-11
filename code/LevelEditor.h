#pragma once
/*=====================================================================================
$File: LevelEditor.h
$Date: March 20, 2016
$Creator: Jamie Cooper
$Notice: (C) Copyright 2015 by Punch Drunk Squirrel Games LLC. All Rights Reserved.
=====================================================================================*/
#ifndef LEVELEDITOR_H              // Prevent multiple definitions if this 
#define LEVELEDITOR_H             // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include <string>
#include <deque>
#include "Includes.h"

class LevelEditor
{
	private:
		// add variables later

	public:
		// Constructor
		LevelEditor();

		// Destructor
		virtual ~LevelEditor();

		void InitializeEditor();



		// Initialize the Editor
		// Pre: *g points to Graphics
		//      *tracker points to Input
		bool initialize();
};
#endif