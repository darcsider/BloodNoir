/*=====================================================================================
$File: RenderManager.cpp
$Date: July 4, 2016
$Creator: Jamie Cooper
$Notice: (C) Copyright 2015 by Punch Drunk Squirrel Games LLC. All Rights Reserved.
=====================================================================================*/
#include "RenderManager.h"

RenderManager::RenderManager()
{
#if _WIN32
	m_win32_Graphics = make_unique<Win32_Graphics>();
#endif
}

RenderManager::~RenderManager()
{
#if _WIN32
	m_win32_Graphics.release();
#endif
}

void RenderManager::AddTexture(string filename, string name)
{
	// add a texture to the platform specific container
#if _WIN32
	m_win32_Graphics->AddTexture(filename, name);
//#else _PS4
#endif
}

void RenderManager::RenderObject(string name, Vector2 position)
{
	// draw an object to the screen with the platform specific graphics system
#if _WIN32
	m_win32_Graphics->DrawObject(name, position);
#endif
}

void RenderManager::RenderObject(string name, RECT sourceRect, Vector2 position)
{
	// draw an object to the screen with the platform specific graphics system
#if _WIN32
	m_win32_Graphics->DrawObject(name, sourceRect, position);
#endif
}

void RenderManager::RenderObject(string name, RECT sourceRect, RECT destRect)
{
	// draw an object to the screen with the platform specific graphics system
#if _WIN32
	m_win32_Graphics->DrawObject(name, sourceRect, destRect);
#endif
}

void RenderManager::RenderText(string text, Vector2 position, const XMVECTORF32& color)
{
	// draw an object to the screen with the platform specific graphics system
#if _WIN32
	m_win32_Graphics->DrawTextToScreen(text, position, color);
#endif
}

void RenderManager::OnWindowSizeChange()
{
	// only fired if windows, linux or macOS as window size cant change on consoles
#if _WIN32
	m_win32_Graphics->CreateResources();
#endif // _WIN32
}

void RenderManager::InitializeGraphics(HWND Window, int width, int height)
{
	// set the game width and height that is passed in
	m_gameWidth = width;
	m_gameHeight = height;
	// initialize the windows platform specific graphics object
#if _WIN32
	m_win32_Graphics->InitializeGraphics(Window, width, height);
#endif
}

void RenderManager::BeginScene()
{
	// begin rendering sprites to the scene
#if _WIN32
	m_win32_Graphics->BeginScene();
#endif
}

void RenderManager::EndScene()
{
	// end rendering sprites to the scene
#if _WIN32
	m_win32_Graphics->EndScene();
#endif // _WIN32
}

void RenderManager::ClearScene()
{
	// clear the screen
#if _WIN32
	m_win32_Graphics->ClearScene();
#endif
}

void RenderManager::PresentScene()
{
	// present all sprites and other stuff to the screen
#if _WIN32
	m_win32_Graphics->PresentScene();
#endif
}

void RenderManager::RenderQuad(Vector2 position, int width, int height, XMFLOAT4 color)
{
	// draw a quad made up of 2 triangles to the screen
#if _WIN32
	m_win32_Graphics->DrawQuad(position, width, height, color);
#endif // _WIN32
}

D3D11_TEXTURE2D_DESC RenderManager::getTextureDesc(string name)
{
	// return the Texture Desc of the texture requested
#if _WIN32
	return m_win32_Graphics->getTextureDesc(name);
#endif
	return D3D11_TEXTURE2D_DESC();
}

float RenderManager::GetLineSpacing()
{
	// return the line spacing used in rendering font to screen
	// only used in console window and level editor
#if _WIN32
	return m_win32_Graphics->GetLineSpacing();
#endif
	return 0.0;
}

bool RenderManager::IsNVIDIAChipset()
{
	// check if its a NVIDIA graphics card
	// windows, linux and macOS platform specific
#if _WIN32
	return m_win32_Graphics->IsNVIDIAChipset();
#endif
}

bool RenderManager::IsAMDChipset()
{
	// check if its a AMD graphics card
	// windows, linux and macOS platform specific
#if _WIN32
	return m_win32_Graphics->IsAMDChipset();
#endif
}