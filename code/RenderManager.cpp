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

}

void RenderManager::AddTexture(string filename, string name)
{
#if _WIN32
	m_win32_Graphics->AddTexture(filename, name);
//#else _PS4
#endif
}

void RenderManager::RenderObject(string name, RECT destRect, const XMVECTORF32& color)
{
#if _WIN32
	m_win32_Graphics->DrawObject(name, destRect, color);
#endif
}

void RenderManager::RenderObject(string name, Vector2 position, const XMVECTORF32& color)
{
#if _WIN32
	m_win32_Graphics->DrawObject(name, position, color);
#endif
}

void RenderManager::RenderObject(string name, RECT sourceRect, Vector2 position, const XMVECTORF32& color)
{
#if _WIN32
	m_win32_Graphics->DrawObject(name, sourceRect, position, color);
#endif
}

void RenderManager::RenderText(string text, Vector2 position, const XMVECTORF32& color)
{
#if _WIN32
	m_win32_Graphics->DrawTextToScreen(text, position, color);
#endif
}

void RenderManager::OnWindowSizeChange()
{
#if _WIN32
	m_win32_Graphics->CreateResources();
#endif // _WIN32
}

void RenderManager::InitializeGraphics(HWND Window, int width, int height)
{
	gameWidth = width;
	gameHeight = height;
#if _WIN32
	m_win32_Graphics->InitializeGraphics(Window, width, height);
#endif
}

void RenderManager::BeginScene()
{
#if _WIN32
	m_win32_Graphics->BeginScene();
#endif
}

void RenderManager::EndScene()
{
#if _WIN32
	m_win32_Graphics->EndScene();
#endif // _WIN32
}

void RenderManager::ClearScene()
{
#if _WIN32
	m_win32_Graphics->ClearScene();
#endif
}

void RenderManager::PresentScene()
{
#if _WIN32
	m_win32_Graphics->PresentScene();
#endif
}

void RenderManager::RenderQuad(Vector2 position, int width, int height, XMFLOAT4 color)
{
#if _WIN32
	m_win32_Graphics->DrawQuad(position, width, height, color);
#endif // _WIN32
}

D3D11_TEXTURE2D_DESC RenderManager::getTextureDesc(string name)
{
#if _WIN32
	return m_win32_Graphics->getTextureDesc(name);
#endif
	return D3D11_TEXTURE2D_DESC();
}

float RenderManager::GetLineSpacing()
{
#if _WIN32
	return m_win32_Graphics->GetLineSpacing();
#endif
	return 0.0;
}

bool RenderManager::IsNVIDIAChipset()
{
#if _WIN32
	return m_win32_Graphics->IsNVIDIAChipset();
#endif
}

bool RenderManager::IsAMDChipset()
{
#if _WIN32
	return m_win32_Graphics->IsAMDChipset();
#endif
}