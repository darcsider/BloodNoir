#pragma once
/*=====================================================================================
$File: RenderManager.h
$Date: July 4, 2016
$Creator: Jamie Cooper
$Notice: (C) Copyright 2015 by Punch Drunk Squirrel Games LLC. All Rights Reserved.
=====================================================================================*/
#ifndef RENDERMANAGER_H
#define RENDERMANAGER_H
#include "Includes.h"
#include "Win32_Graphics.h"

class RenderManager
{
	private:
		unique_ptr<Win32_Graphics> m_win32_Graphics;
		int gameWidth;
		int gameHeight;
		// PS4Graphics *m_ps4_Graphics;
		// XBONEGraphics *m_XBONEGraphics;

	public:
		RenderManager();
		~RenderManager();

		static RenderManager& GetInstance()
		{
			static RenderManager* gpSingleton = NULL;
			if (gpSingleton == NULL)
			{
				gpSingleton = new RenderManager();
			}
			return *gpSingleton;
		}

		void InitializeGraphics(HWND Window, int width, int height);

		// Win32 Only???
		void BeginScene();
		void EndScene();
		void ClearScene();
		void PresentScene();

		void AddTexture(string filename, string name);
		
		void RenderObject(string name, RECT destRect, const XMVECTORF32& color = Colors::White);
		void RenderObject(string name, Vector2 position = Vector2(0.0, 0.0), const XMVECTORF32& color = Colors::White);
		void RenderObject(string name, RECT sourceRect, Vector2 position, const XMVECTORF32& color = Colors::White);
		void RenderText(string text, Vector2 position, const XMVECTORF32& color = Colors::Black);

		void OnWindowSizeChange();

		// this will probably go away unless I figure out how to make an in game level editor work correctly
		void RenderQuad(Vector2 position, int width, int height, XMFLOAT4 color);

		D3D11_TEXTURE2D_DESC getTextureDesc(string name);
		float GetLineSpacing();

		bool IsNVIDIAChipset();
		bool IsAMDChipset();

		int GetGameWidth()
		{
			return gameWidth;
		}

		int GetGameHeight()
		{
			return gameHeight;
		}
};
#endif