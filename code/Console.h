#pragma once
/*=====================================================================================
$File: Console.h
$Date: March 20, 2016
$Creator: Jamie Cooper
$Notice: (C) Copyright 2015 by Punch Drunk Squirrel Games LLC. All Rights Reserved.
=====================================================================================*/
#ifndef _CONSOLE_H              // Prevent multiple definitions if this 
#define _CONSOLE_H             // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include <string>
#include <deque>
#include "Includes.h"
#include "DX11RenderManager.h"
#pragma warning (disable : 4018)

class Console
{
	private:
		DX11RenderManager				*m_graphicSystem;		// Graphics System variable
		string							m_commandString;		// string holding the command entered in by the user
		string							m_prompt;				// the prompt that displays on the screen
		string							m_textInput;			// the text that is being entered in by the keyboard
		XMFLOAT4						m_backgroundColor;		// the color for the background of the console
		Vector2							m_textPosition;			// position of current line of text on the screen
		Vector2							m_consolePosition;		// position of the console on the screen
		Vector2							m_promptPosition;		// where the prompt will display on the screen
		Vector2							m_startTextPosition;	// position of text that the user is typing in
		RECT							m_textLayout;			// layout of the text within the console window
		unsigned int					m_width;				// width of the console
		unsigned int					m_height;				// height of the console
		unsigned int					m_columns;
		unsigned int					m_rows;
		unsigned int					m_currentColumn;
		unsigned int					m_currentLine;
		float							m_lineSpacing;
		char							m_characterInput;		// recent character entered in (not sure if I'm going to use this)
		bool							m_newline;				// did the end user hit enter? yes=process command, no=keep inputing
		bool							m_visible;				// should the console be displayed or not?
		const int						MAXLINES = 8;			// max number of lines allowed to be in history
		vector<string>					m_history;				// history of commands entered in
		map<string, function<void()>>	m_commandMap;			// map holding the commands and function pointers to
																// the function that is being used to process that command

	public:
		// Constructor
		Console();

		// Destructor
		virtual ~Console();

		// Initialize the Console
		// Pre: *g points to Graphics
		//      *in points to Input
		bool initialize(DX11RenderManager *graphics, Vector2 startPosition, int width, int height, XMFLOAT4 backColor);

		void Draw();

		void HideShow()
		{
			m_visible = !m_visible;
			m_textInput.clear();
		}

		bool IsVisible()
		{
			return m_visible;
		}

		void TextInput(WPARAM wParam);

		void AddCommand(string commandName, function<void()> funcPoint);

		void ProcessCommand(std::string CommandName);

		// Call when graphics device is lost.
		void onLostDevice();

		// Call when graphics device is reset.
		void onResetDevice();
};

#endif