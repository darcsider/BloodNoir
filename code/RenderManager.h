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
#include "DX11Graphics.h"

class RenderManager
{
	private:
		DX11Graphics *m_win32_Graphics;
		// PS4Graphics *m_ps4_Graphics;

		vector<string> m_textures;

	public:
		void AddTexture(string filename, string name);
		void RenderObject(string name, Vector2 position = Vector2(0.0,0.0), const XMVECTORF32& color = Colors::White);
		void RenderObject(string name, RECT sourceRect, Vector2 position = Vector2(0.0, 0.0), const XMVECTORF32& color = Colors::White);
};
#endif