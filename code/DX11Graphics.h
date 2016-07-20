#pragma once
/*=====================================================================================
$File: DX11Graphics.h
$Date: June 27, 2016
$Creator: Jamie Cooper
$Notice: (C) Copyright 2015 by Punch Drunk Squirrel Games LLC. All Rights Reserved.
=====================================================================================*/
#ifndef DX11GRAPHICS_H
#define DX11GRAPHICS_H
#include "Includes.h"
class DX11Graphics
{
	protected:
		D3D_FEATURE_LEVEL				m_featureLevel;
		ComPtr<ID3D11Device>			m_d3dDevice;
		ComPtr<ID3D11Device1>			m_d3dDevice1;
		ComPtr<ID3D11DeviceContext>		m_d3dContext;
		ComPtr<ID3D11DeviceContext1>	m_d3dContext1;

		ComPtr<IDXGISwapChain>			m_swapChain;
		ComPtr<IDXGISwapChain1>			m_swapChain1;
		ComPtr<ID3D11RenderTargetView>	m_renderTargetView;
		ComPtr<ID3D11DepthStencilView>	m_depthStencilView;
		ComPtr<ID3D11Texture2D>			m_depthStencil;

		ComPtr<ID3D11InputLayout>		m_inputLayout;
		UINT							m4xMsaaQuality;

		int								m_gameWidth;
		int								m_gameHeight;
		HWND							m_window;
		bool							graphicsInitialized;

		unique_ptr<CommonStates>		m_graphicStates;
		unique_ptr<BasicEffect>			m_effectSystem;
		unique_ptr<EffectFactory>		m_effectFactory;
		unique_ptr<SpriteBatch>			m_spriteBatch;
		unique_ptr<SpriteFont>			m_spriteFont;

		unique_ptr<PrimitiveBatch<VertexPositionColor>> m_primitiveBatch;

		map<string, ComPtr<ID3D11ShaderResourceView>> m_textures;

	public:
		DX11Graphics();
		~DX11Graphics();

		void InitDirectX11();
		bool InitializeGraphics(HWND Window, int width, int height);
		bool InitDirectXTKObjects();

		bool AddTexture(string filename, string name);
		void CreateResources();
		void OnDeviceLost();

		void BeginScene();
		void ClearScene();
		void PresentScene();
		void DrawObject(string textureName, Vector2 position, const XMVECTORF32& color = Colors::White);
		void DrawObject(string textureName, RECT sourceRect, Vector2 position = Vector2(0.0, 0.0), const XMVECTORF32& color = Colors::White);
		void DrawQuad(Vector2 position, int width, int height, XMFLOAT4 color);
		void EndScene();

		float GetLineSpacing()
		{
			return m_spriteFont->GetLineSpacing();
		}

		void DrawTextToScreen(string text, Vector2 position, const XMVECTORF32& color = Colors::White);

		D3D11_TEXTURE2D_DESC getTextureDesc(string textureName);

		void SetWidth(int width)
		{
			m_gameWidth = width;
		}

		int GetWidth()
		{
			return m_gameWidth;
		}

		void SetHeight(int height)
		{
			m_gameHeight = height;
		}

		int GetHeight()
		{
			return m_gameHeight;
		}

		ID3D11Device* GetDirectXDevice()
		{
			return m_d3dDevice.Get();
		}

		ID3D11DeviceContext* GetDirectXContext()
		{
			return m_d3dContext.Get();
		}

		void SetWindowHandle(HWND Window)
		{
			m_window = Window;
		}

		// return the aspect ration of the window
		float AspectRatio()
		{
			return static_cast<float>(m_gameWidth) / m_gameHeight;
		}

		wstring ConvertSTRtoWSTR(string text)
		{
			typedef std::codecvt_utf8<wchar_t> convert_type;
			wstring convertedText;
			wstring_convert<convert_type, wchar_t> converter;

			convertedText = converter.from_bytes(text);
			return convertedText;
		}
};
#endif