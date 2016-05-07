/*=====================================================================================
$File: DX11RenderManager.cpp
$Date: April 25, 2016
$Creator: Jamie Cooper
$Notice: (C) Copyright 2015 by Punch Drunk Squirrel Games LLC. All Rights Reserved.
=====================================================================================*/
#include "DX11RenderManager.h"

DX11RenderManager::DX11RenderManager()
{
	m_window = nullptr;
	graphicsInitialized = false;
	m4xMsaaQuality = 0;
	m_gameWidth = 0;
	m_gameHeight = 0;
}

DX11RenderManager::~DX11RenderManager()
{
	m_depthStencil.Reset();
	m_depthStencilView.Reset();
	m_renderTargetView.Reset();
	m_swapChain1.Reset();
	m_swapChain.Reset();
	m_d3dContext1.Reset();
	m_d3dContext.Reset();
	m_d3dDevice1.Reset();
	m_d3dDevice.Reset();
	m_textures.clear();
}

bool DX11RenderManager::InitDirectXTKObjects()
{
	m_graphicStates.reset(new DirectX::CommonStates(m_d3dDevice.Get()));
	m_effectFactory.reset(new DirectX::EffectFactory(m_d3dDevice.Get()));
	m_effectSystem.reset(new DirectX::BasicEffect(m_d3dDevice.Get()));
	m_effectSystem->SetProjection(XMMatrixOrthographicOffCenterLH(0, m_gameWidth, m_gameHeight, 0, 0, 1));
	m_effectSystem->SetVertexColorEnabled(true);

	{
		void const* shaderByteCode;
		size_t byteCodeLength;

		m_effectSystem->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);
		if (FAILED(m_d3dDevice->CreateInputLayout(VertexPositionColor::InputElements,
			VertexPositionColor::InputElementCount, shaderByteCode, byteCodeLength, m_inputLayout.GetAddressOf())))
			return false;
	}

	m_spriteBatch.reset(new DirectX::SpriteBatch(m_d3dContext.Get()));
	m_spriteFont.reset(new SpriteFont(m_d3dDevice.Get(), L"..\\Art\\CourierNew.spriteFont"));

	m_primitiveBatch.reset(new PrimitiveBatch<VertexPositionColor>(m_d3dContext.Get()));

	return true;
}

bool DX11RenderManager::InitializeGraphics(HWND Window, int width, int height)
{
	m_window = Window;
	m_gameWidth = width;
	m_gameHeight = height;

	InitDirectX11();

	CreateResources();

	InitDirectXTKObjects();

	graphicsInitialized = true;

	return true;
}

void DX11RenderManager::InitDirectX11()
{
	// Create the device and device context.
	HRESULT Result;
	UINT creationFlags = 0;
#ifdef _DEBUG
	creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif // _DEBUG

	static const D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1,
	};

	Result = D3D11CreateDevice(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		creationFlags,
		featureLevels,
		_countof(featureLevels),
		D3D11_SDK_VERSION,
		m_d3dDevice.ReleaseAndGetAddressOf(),
		&m_featureLevel,
		m_d3dContext.ReleaseAndGetAddressOf());

	if (Result == E_INVALIDARG)
	{
		// DirectX 11.0 platforms will not recognize D3D_FEATURE_LEVEL_11_1 so we need to retry without it
		Result = D3D11CreateDevice(nullptr,
			D3D_DRIVER_TYPE_HARDWARE,
			nullptr,
			creationFlags,
			&featureLevels[1],
			_countof(featureLevels) - 1,
			D3D11_SDK_VERSION,
			m_d3dDevice.ReleaseAndGetAddressOf(),
			&m_featureLevel,
			m_d3dContext.ReleaseAndGetAddressOf()
			);
	}

	DX::ThrowIfFailed(Result);

#ifndef NDEBUG
	ComPtr<ID3D11Debug> d3dDebug;
	if (SUCCEEDED(m_d3dDevice.As(&d3dDebug)))
	{
		ComPtr<ID3D11InfoQueue> d3dInfoQueue;
		if (SUCCEEDED(d3dDebug.As(&d3dInfoQueue)))
		{
#ifdef _DEBUG
			d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_CORRUPTION, true);
			d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_ERROR, true);
#endif
			D3D11_MESSAGE_ID hide[] =
			{
				D3D11_MESSAGE_ID_SETPRIVATEDATA_CHANGINGPARAMS,
				// TODO: Add more message IDs here as needed.
			};
			D3D11_INFO_QUEUE_FILTER filter;
			memset(&filter, 0, sizeof(filter));
			filter.DenyList.NumIDs = _countof(hide);
			filter.DenyList.pIDList = hide;
			d3dInfoQueue->AddStorageFilterEntries(&filter);
		}
	}
#endif

	// DirectX 11.1 if present
	if (SUCCEEDED(m_d3dDevice.As(&m_d3dDevice1)))
		(void)m_d3dContext.As(&m_d3dContext1);
}

bool DX11RenderManager::AddTexture(std::wstring filename, std::string name)
{
	ComPtr<ID3D11ShaderResourceView> texture;

	if ((!filename.empty()) && (!name.empty()))
	{
		auto textureIndex = m_textures.find(name);

		if (textureIndex == m_textures.end())
		{
			if (SUCCEEDED(CreateWICTextureFromFile(m_d3dDevice.Get(), filename.c_str(), nullptr, texture.ReleaseAndGetAddressOf())))
			{
				m_textures.insert(std::pair<std::string, ComPtr<ID3D11ShaderResourceView>>(name, texture));
			}
			else
			{
				return false;
			}
			return true;
		}
		else
		{
			return true;
		}
	}
	else
	{
		return false;
	}
	return true;
}

void DX11RenderManager::BeginScene()
{
	if (m_spriteBatch != NULL)
	{
		m_spriteBatch->Begin();
	}
}

void DX11RenderManager::ClearScene()
{
	// Clear the views
	m_d3dContext->ClearRenderTargetView(m_renderTargetView.Get(), Colors::Green);
	m_d3dContext->ClearDepthStencilView(m_depthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);

	m_d3dContext->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), m_depthStencilView.Get());

	// set the viewport
	CD3D11_VIEWPORT screenViewport(0.0f, 0.0f, static_cast<float>(m_gameWidth), static_cast<float>(m_gameHeight));
	m_d3dContext->RSSetViewports(1, &screenViewport);
}

void DX11RenderManager::DrawObject(std::string textureName, Vector2 position)
{
	if (graphicsInitialized)
	{
		if (!textureName.empty())
		{
			auto textureIndex = m_textures.find(textureName);
			if (textureIndex != m_textures.end())
			{
				m_spriteBatch->Draw(textureIndex->second.Get(), position);
			}
		}
	}
}

void DX11RenderManager::DrawObject(std::string textureName, RECT sourceRect, Vector2 position)
{
	if (graphicsInitialized)
	{
		if (!textureName.empty())
		{
			auto textureIndex = m_textures.find(textureName);
			if (textureIndex != m_textures.end())
			{
				m_spriteBatch->Draw(textureIndex->second.Get(), position, &sourceRect);
			}
		}
	}
}

void DX11RenderManager::DrawQuad(Vector2 position, int width, int height, XMFLOAT4 color)
{
	float startX = position.x;
	float startY = position.y;

	float endX = position.x + width;
	float endY = position.y + height;

	VertexPositionColor v1(XMFLOAT3(startX, startY, 0), color);
	VertexPositionColor v2(XMFLOAT3(endX, startY, 0), color);
	VertexPositionColor v3(XMFLOAT3(endX, endY, 0), color);
	VertexPositionColor v4(XMFLOAT3(startX, endY, 0), color);

	m_d3dContext.Get()->OMSetBlendState(m_graphicStates.get()->AlphaBlend(), nullptr, 0xFFFFFFFF);
	m_d3dContext.Get()->OMSetDepthStencilState(m_graphicStates.get()->DepthNone(), 0);
	m_d3dContext.Get()->RSSetState(m_graphicStates.get()->CullNone());

	m_effectSystem->Apply(m_d3dContext.Get());
	m_d3dContext.Get()->IASetInputLayout(m_inputLayout.Get());

	m_primitiveBatch->Begin();
	m_primitiveBatch->DrawQuad(v1, v2, v3, v4);
	m_primitiveBatch->End();
}

D3D11_TEXTURE2D_DESC DX11RenderManager::getTextureDesc(std::string textureName)
{
	D3D11_TEXTURE2D_DESC textDesc = { 0 };
	ComPtr<ID3D11Texture2D>	texture;
	ComPtr<ID3D11Resource> resource;
	D3D11_RESOURCE_DIMENSION dim;

	if (graphicsInitialized)
	{
		if (!textureName.empty())
		{
			auto textureIndex = m_textures.find(textureName);
			if (textureIndex != m_textures.end())
			{
				textureIndex->second.Get()->GetResource(resource.GetAddressOf());
				resource->GetType(&dim);
				if (dim != D3D11_RESOURCE_DIMENSION_TEXTURE2D)
					throw std::exception("No texture2D no fun");
				resource.As(&texture);
				texture->GetDesc(&textDesc);
			}
		}
	}
	return textDesc;
}

void DX11RenderManager::EndScene()
{
	m_spriteBatch->End();
}

void DX11RenderManager::PresentScene()
{
	// The first argument instructs DXGI to block until VSync, putting the application
	// to sleep until the next VSync. This ensures we don't waste any cycles rendering
	// frames that will never be displayed to the screen.
	HRESULT hr = m_swapChain->Present(1, 0);

	// If the device was reset we must completely reinitialize the renderer
	if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
	{
		OnDeviceLost();
	}
	else
	{
		DX::ThrowIfFailed(hr);
	}
}

void DX11RenderManager::DrawTextToScreen(std::string text, Vector2 position)
{
	std::wstring convertedText;
	std::wstring_convert<convert_type, wchar_t> converter;

	convertedText = converter.from_bytes(text);
	m_spriteFont->DrawString(m_spriteBatch.get(), convertedText.c_str(), position, Colors::BlueViolet);
}

void DX11RenderManager::DrawTextToScreen(std::string text, Vector2 position, const XMVECTORF32& color)
{
	std::wstring convertedText;
	std::wstring_convert<convert_type, wchar_t> converter;

	convertedText = converter.from_bytes(text);
	m_spriteFont->DrawString(m_spriteBatch.get(), convertedText.c_str(), position, color);
}

void DX11RenderManager::CreateResources()
{
	// Clear the previous window size specific context
	ID3D11RenderTargetView* nullViews[] = { nullptr };
	m_d3dContext->OMSetRenderTargets(_countof(nullViews), nullViews, nullptr);
	m_renderTargetView.Reset();
	m_depthStencilView.Reset();
	m_d3dContext->Flush();

	UINT backBufferWidth = static_cast<UINT>(m_gameWidth);
	UINT backBufferHeight = static_cast<UINT>(m_gameHeight);
	DXGI_FORMAT backBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
	DXGI_FORMAT depthBufferFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	UINT backbufferCount = 2;

	// If the swap chain already exists, resize it, otherwise create one
	if (m_swapChain)
	{
		HRESULT hr = m_swapChain->ResizeBuffers(backbufferCount, backBufferWidth, backBufferHeight, backBufferFormat, 0);

		if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
		{
			OnDeviceLost();
			return;
		}
		else
		{
			DX::ThrowIfFailed(hr);
		}
	}
	else
	{
		HRESULT hr = m_d3dDevice->CheckMultisampleQualityLevels(backBufferFormat, 4, &m4xMsaaQuality);
		DX::ThrowIfFailed(hr);

		// First, retrieve the underlying DXGI Device from the D3D Device
		ComPtr<IDXGIDevice1> dxgiDevice;
		DX::ThrowIfFailed(m_d3dDevice.As(&dxgiDevice));

		// Identify the physical adapter (GPU or card) this device is running on.
		ComPtr<IDXGIAdapter> dxgiAdapter;
		DX::ThrowIfFailed(dxgiDevice->GetAdapter(dxgiAdapter.GetAddressOf()));

		// And obtain the factory object that created it.
		ComPtr<IDXGIFactory1> dxgiFactory;
		DX::ThrowIfFailed(dxgiAdapter->GetParent(IID_PPV_ARGS(dxgiFactory.GetAddressOf())));

		ComPtr<IDXGIFactory2> dxgiFactory2;
		if (SUCCEEDED(dxgiFactory.As(&dxgiFactory2)))
		{
			// DirectX 11.1 or later

			// Create a descriptor for the swap chain
			DXGI_SWAP_CHAIN_DESC1 swapChainDesc = { 0 };
			swapChainDesc.Width = backBufferWidth;
			swapChainDesc.Height = backBufferHeight;
			swapChainDesc.Format = backBufferFormat;
			swapChainDesc.SampleDesc.Count = 4;
			swapChainDesc.SampleDesc.Quality = m4xMsaaQuality - 1;
			swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			swapChainDesc.BufferCount = backbufferCount;

			DXGI_SWAP_CHAIN_FULLSCREEN_DESC fsSwapChainDesc = { 0 };
			fsSwapChainDesc.Windowed = TRUE;

			// Create a SwapChain from a Win32 window
			DX::ThrowIfFailed(dxgiFactory2->CreateSwapChainForHwnd(
				m_d3dDevice.Get(),
				m_window,
				&swapChainDesc,
				&fsSwapChainDesc,
				nullptr,
				m_swapChain1.ReleaseAndGetAddressOf()));

			DX::ThrowIfFailed(m_swapChain1.As(&m_swapChain));
		}
		else
		{
			DXGI_SWAP_CHAIN_DESC swapChainDesc = { 0 };
			swapChainDesc.BufferCount = backbufferCount;
			swapChainDesc.BufferDesc.Width = backBufferWidth;
			swapChainDesc.BufferDesc.Height = backBufferHeight;
			swapChainDesc.BufferDesc.Format = backBufferFormat;
			swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			swapChainDesc.OutputWindow = m_window;
			swapChainDesc.SampleDesc.Count = 4;
			swapChainDesc.SampleDesc.Quality = m4xMsaaQuality - 1;
			swapChainDesc.Windowed = TRUE;

			DX::ThrowIfFailed(dxgiFactory->CreateSwapChain(m_d3dDevice.Get(), &swapChainDesc, m_swapChain.ReleaseAndGetAddressOf()));
		}
		// TODO add support for going between full screen and windowed
	}

	// Obtain the back buffer for this window which will be the final 3D render target.
	ComPtr<ID3D11Texture2D> backBuffer;
	DX::ThrowIfFailed(m_swapChain->GetBuffer(0, IID_PPV_ARGS(backBuffer.GetAddressOf())));

	// Create a view interface on the render target to use on bind.
	DX::ThrowIfFailed(m_d3dDevice->CreateRenderTargetView(backBuffer.Get(), nullptr, m_renderTargetView.ReleaseAndGetAddressOf()));

	// Allocate a 2-D surface as the depth/stencil buffer and
	// create a DepthStencil view on this surface to use on bind.
	CD3D11_TEXTURE2D_DESC depthStencilDesc(depthBufferFormat, backBufferWidth, backBufferHeight, 1, 1, D3D11_BIND_DEPTH_STENCIL);
	depthStencilDesc.SampleDesc.Count = 4;
	depthStencilDesc.SampleDesc.Quality = m4xMsaaQuality - 1;

	ComPtr<ID3D11Texture2D> depthStencil;
	DX::ThrowIfFailed(m_d3dDevice->CreateTexture2D(&depthStencilDesc, nullptr, depthStencil.GetAddressOf()));

	CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc(D3D11_DSV_DIMENSION_TEXTURE2DMS);
	DX::ThrowIfFailed(m_d3dDevice->CreateDepthStencilView(depthStencil.Get(), &depthStencilViewDesc, m_depthStencilView.ReleaseAndGetAddressOf()));

	// TODO Initialize windows' size dependent objects here.
}

void DX11RenderManager::OnDeviceLost()
{
	m_depthStencil.Reset();
	m_depthStencilView.Reset();
	m_renderTargetView.Reset();
	m_swapChain1.Reset();
	m_swapChain.Reset();
	m_d3dContext1.Reset();
	m_d3dContext.Reset();
	m_d3dDevice1.Reset();
	m_d3dDevice.Reset();
	m_graphicStates.reset();
	m_effectSystem.reset();
	m_primitiveBatch.reset();
	m_inputLayout.Reset();
	InitDirectX11();
	CreateResources();
}