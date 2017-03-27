#pragma once
/*=====================================================================================
$File: Win32_Platform.h
$Date: January 6, 2016
$Creator: Jamie Cooper
$Notice: (C) Copyright 2015 by Punch Drunk Squirrel Games LLC. All Rights Reserved.
=====================================================================================*/
#ifndef WIN32_PLATFORM_H
#define WIN32_PLATFORM_H
#if _WIN32
#include "Includes.h"
#include "Game.h"

// all stuff in these files are windows platform specific
// class name used for the window
string className = "BloodNoir";
// windowTitle that will display if the window is not fullscreen
string windowTitle = "Blood Noir 1.0";

// pointer to the game object
std::unique_ptr<Game> m_game;

// window rect and window handle
RECT rc;
HWND m_window;

// initialize the class used to build the window
bool InitWindowClass(HINSTANCE program);

// window process method that handles all messages passed in by windows
LRESULT CALLBACK WndProc(HWND Window, UINT Message, WPARAM wParam, LPARAM lParam);

// winMain windows starting point
int WINAPI wWinMain(_In_ HINSTANCE Program, _In_opt_ HINSTANCE PreviousProgram, _In_ LPWSTR CommandLine, _In_ int ShowCommand);
#endif
#endif // !WIN32_PLATFORM_H