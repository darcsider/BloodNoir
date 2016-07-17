#pragma once
/*=====================================================================================
$File: Includes.h
$Date: February 2, 2016
$Creator: Jamie Cooper
$Notice: (C) Copyright 2015 by Punch Drunk Squirrel Games LLC. All Rights Reserved.
=====================================================================================*/
#ifndef INCLUDES_H
#define INCLUDES_H
#pragma once
#include <WinSDKVer.h>
#define _WIN32_WINNT 0x0600
#include <SDKDDKVer.h>

// Use the C++ standard templated min/max
#define NOMINMAX

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <string>
#include <stack>
#include <vector>
#include <list>
#include <map>
#include <wrl/client.h>
#include <locale>
#include <codecvt>
#include <d3d11_1.h>
#include <algorithm>
#include <exception>
#include <memory>
#include <fstream>
#include <iostream>
#include <sstream>
#include <NvGsa.h>

// DirectX TK Stuff
#include "CommonStates.h"
#include "DDSTextureLoader.h"
#include "DirectXHelpers.h"
#include "Effects.h"
#include "GamePad.h"
#include "GeometricPrimitive.h"
#include "Keyboard.h"
#include "Model.h"
#include "Mouse.h"
#include "PrimitiveBatch.h"
#include "ScreenGrab.h"
#include "SimpleMath.h"
#include "SpriteBatch.h"
#include "SpriteFont.h"
#include "VertexTypes.h"
#include "WICTextureLoader.h"
#include "Audio.h"

using namespace DirectX;
using namespace std;
using Microsoft::WRL::ComPtr;
using DirectX::SimpleMath::Vector2;

#define SAFE_DELETE( p ) { if( p ) { delete ( p );     ( p ) = NULL; } }

namespace DX
{
	inline void ThrowIfFailed(HRESULT hr)
	{
		if (FAILED(hr))
		{
			// Set a breakpoint on this line to catch DirectX API errors
			throw exception();
		}
	}
}
#endif