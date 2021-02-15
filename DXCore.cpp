#include "pch.h"
#include "DXCore.h"
DXCore::DXCore() noexcept
	: m_pDevice{ nullptr }, 
	  m_pDeviceContext{ nullptr }, 
	  m_pSwapChain{ nullptr },
	  m_pBackBuffer{ nullptr },
	  m_pDepthStencilView{ nullptr },
	  m_pRasterizerStateFill{ nullptr},
	  m_pRasterizerStateWireFrame{ nullptr },
	  m_DefaultViewport { 0 },
	  m_MSAAQuality{ 4u },
	  m_WireFrameEnabled{ false },
	  m_SkyboxEnabled{ false }
{

}

const bool DXCore::Initialize(const unsigned int& clientWindowWidth, 
							  const unsigned int& clientWindowHeight, 
							  const HWND& windowHandle)
{
	EventBuss::Get().AddListener(this, EventType::ToggleWireFrameEvent, EventType::ToggleDepthStencilStateEvent, EventType::RequestDSVEvent);
	EventBuss::Get().AddListener(this, EventType::BindBackBufferEvent);

	UINT flags = D3D11_CREATE_DEVICE_SINGLETHREADED;
	#if defined(DEBUG) || defined(_DEBUG)
		flags |= D3D11_CREATE_DEVICE_DEBUG;
	#endif

	D3D_FEATURE_LEVEL pFeatureLevel[1] = { D3D_FEATURE_LEVEL_11_0 };
	D3D_FEATURE_LEVEL verifiedFeatureLevel = {};
	HR(D3D11CreateDevice(nullptr, 
						 D3D_DRIVER_TYPE_HARDWARE, 
						 nullptr, 
						 flags, 
						 pFeatureLevel,
						 1u,
						 D3D11_SDK_VERSION,
						 &m_pDevice,
						 &verifiedFeatureLevel,
						 &m_pDeviceContext),
						 "D3D11CeateDevice");
	assert(verifiedFeatureLevel == D3D_FEATURE_LEVEL_11_0);

	HR(m_pDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM,
											    4u,
											    &m_MSAAQuality),
											    "CheckMultiSampleQualityLevels");
	assert(m_MSAAQuality > 0u);

	DXGI_SWAP_CHAIN_DESC swapChainDescriptor = {};
	swapChainDescriptor.BufferDesc.Width = clientWindowWidth;
	swapChainDescriptor.BufferDesc.Height = clientWindowHeight;
	swapChainDescriptor.BufferDesc.RefreshRate.Numerator = 60u;								//TODO: Get actual monitor refreshrate (Emil F)
	swapChainDescriptor.BufferDesc.RefreshRate.Denominator = 1u;							//*-*
	swapChainDescriptor.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDescriptor.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDescriptor.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDescriptor.SampleDesc.Count = 4u;
	swapChainDescriptor.SampleDesc.Quality = m_MSAAQuality - 1u;
	swapChainDescriptor.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDescriptor.BufferCount = 1u;
	swapChainDescriptor.OutputWindow = windowHandle;
	swapChainDescriptor.Windowed = true;
	swapChainDescriptor.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDescriptor.Flags = 0u;
	
	Microsoft::WRL::ComPtr<IDXGIDevice> pDXGIDevice = nullptr;
	HR(m_pDevice->QueryInterface(__uuidof(IDXGIDevice), 
								 &pDXGIDevice), 
								 "QueryInterface");

	Microsoft::WRL::ComPtr<IDXGIAdapter> pDXGIAdapter = nullptr;
	HR(pDXGIDevice->GetParent(__uuidof(IDXGIAdapter), 
							  &pDXGIAdapter), 
							  "GetParent");

	Microsoft::WRL::ComPtr<IDXGIFactory> pDXGIFactory = nullptr;
	HR(pDXGIAdapter->GetParent(__uuidof(IDXGIFactory), 
							   &pDXGIFactory), 
							   "GetParent");

	HR(pDXGIFactory->CreateSwapChain(m_pDevice.Get(), 
									 &swapChainDescriptor, 
									 &m_pSwapChain), 
									 "CreateSwapChain");

	Microsoft::WRL::ComPtr<ID3D11Texture2D> pSwapChainBuffer = nullptr;
	HR(m_pSwapChain->GetBuffer(0u, 
							   __uuidof(ID3D11Texture2D), 
							   &pSwapChainBuffer), 
							   "GetBuffer");

	HR(m_pDevice->CreateRenderTargetView(pSwapChainBuffer.Get(), 
										 nullptr, 
										 &m_pBackBuffer), 
										 "CreateRenderTargetView");

	D3D11_DEPTH_STENCIL_DESC depthStencilDescriptor = {};
	depthStencilDescriptor.DepthEnable = true;
	depthStencilDescriptor.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDescriptor.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencilDescriptor.StencilEnable = false;
	depthStencilDescriptor.StencilReadMask = 0u;
	depthStencilDescriptor.StencilWriteMask = 0u;
	HR(m_pDevice->CreateDepthStencilState(&depthStencilDescriptor, &m_pDepthStencilStateDefault), "CreateDepthStencilState");
	m_pDeviceContext->OMSetDepthStencilState(m_pDepthStencilStateDefault.Get(), 1u);
	depthStencilDescriptor.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	depthStencilDescriptor.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	HR(m_pDevice->CreateDepthStencilState(&depthStencilDescriptor, &m_pDepthStencilStateSkybox), "CreateDepthStencilState");

	D3D11_TEXTURE2D_DESC depthStencilTextureDescriptor = {};
	depthStencilTextureDescriptor.Width = clientWindowWidth;
	depthStencilTextureDescriptor.Height = clientWindowHeight;
	depthStencilTextureDescriptor.MipLevels = 1u;
	depthStencilTextureDescriptor.ArraySize = 1u;
	depthStencilTextureDescriptor.Format = DXGI_FORMAT_D32_FLOAT;			//No stencil part used if technique won't be used (Emil F)
	depthStencilTextureDescriptor.SampleDesc.Count = 4u;
	depthStencilTextureDescriptor.SampleDesc.Quality = m_MSAAQuality - 1u;
	depthStencilTextureDescriptor.Usage = D3D11_USAGE_DEFAULT;
	depthStencilTextureDescriptor.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilTextureDescriptor.CPUAccessFlags = 0u;
	depthStencilTextureDescriptor.MiscFlags = 0u;

	Microsoft::WRL::ComPtr<ID3D11Texture2D> pDepthStencilTexture = nullptr; //Might need to hold on to as member variable (Emil F)
	HR(m_pDevice->CreateTexture2D(&depthStencilTextureDescriptor,
								  nullptr, 
								  &pDepthStencilTexture), 
								  "CreateTexture2D");

	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDescriptor = {};
	depthStencilViewDescriptor.Format = DXGI_FORMAT_D32_FLOAT;
	depthStencilViewDescriptor.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
	depthStencilViewDescriptor.Texture2D.MipSlice = 0u;

	HR(m_pDevice->CreateDepthStencilView(pDepthStencilTexture.Get(), 
										 &depthStencilViewDescriptor,
										 &m_pDepthStencilView), 
										 "CreateDepthStencilView");
	m_pDeviceContext->OMSetRenderTargets(1u, m_pBackBuffer.GetAddressOf(), m_pDepthStencilView.Get());

	m_DefaultViewport.TopLeftX = 0.0f;
	m_DefaultViewport.TopLeftY = 0.0f;
	m_DefaultViewport.Width = static_cast<FLOAT>(clientWindowWidth);
	m_DefaultViewport.Height = static_cast<FLOAT>(clientWindowHeight);
	m_DefaultViewport.MinDepth = 0.0f;
	m_DefaultViewport.MaxDepth = 1.0f;
	m_pDeviceContext->RSSetViewports(1u, &m_DefaultViewport);

	D3D11_RASTERIZER_DESC rasterizerDesc = {};
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = D3D11_CULL_BACK;
	rasterizerDesc.DepthBias = 0u;
	rasterizerDesc.DepthBiasClamp = 0.0f;
	rasterizerDesc.SlopeScaledDepthBias = 0.0f;
	rasterizerDesc.DepthClipEnable = true;
	rasterizerDesc.ScissorEnable = false;
	rasterizerDesc.MultisampleEnable = true;
	rasterizerDesc.AntialiasedLineEnable = false;
	HR(m_pDevice->CreateRasterizerState(&rasterizerDesc, &m_pRasterizerStateFill), "CreateRasterizerState");

	rasterizerDesc.CullMode = D3D11_CULL_NONE;
	HR(m_pDevice->CreateRasterizerState(&rasterizerDesc, &m_pRasterizerStateNoCull), "CreateRasterizerState");

	rasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;
	HR(m_pDevice->CreateRasterizerState(&rasterizerDesc, &m_pRasterizerStateNoCullWF), "CreateRasterizerState");

	rasterizerDesc.CullMode = D3D11_CULL_BACK;
	HR(m_pDevice->CreateRasterizerState(&rasterizerDesc, &m_pRasterizerStateWireFrame), "CreateRasterizerState");
	m_pDeviceContext->RSSetState(m_pRasterizerStateFill.Get());

	ImGui_ImplDX11_Init(m_pDevice.Get(), m_pDeviceContext.Get());

	DelegateDXHandles();
	return true;
}

void DXCore::OnEvent(IEvent& event) noexcept
{
	switch (event.GetEventType())
	{
	case EventType::ToggleWireFrameEvent:
		ToggleWireFrame();
		break;
	case EventType::ToggleDepthStencilStateEvent:
		ToggleDepthStencilState();
		break;

	case EventType::RequestDSVEvent:
	{
		SendDSVEvent dsvEvent(m_pDepthStencilView);
		EventBuss::Get().Delegate(dsvEvent);
		break;
	}
	case EventType::BindBackBufferEvent:
		m_pDeviceContext->OMSetRenderTargets(1, m_pBackBuffer.GetAddressOf(), m_pDepthStencilView.Get());
		break;
	}
}

void DXCore::ToggleWireFrame() noexcept
{
	if (m_WireFrameEnabled)
	{
		m_pDeviceContext->RSSetState(m_pRasterizerStateFill.Get());
		m_WireFrameEnabled = false;
	}
	else
	{
		m_pDeviceContext->RSSetState(m_pRasterizerStateWireFrame.Get());
		m_WireFrameEnabled = true;
	}
}

void DXCore::DelegateDXHandles() noexcept
{
	DelegateDXEvent event(m_pDevice, m_pDeviceContext, m_pBackBuffer, m_pDepthStencilView);
	EventBuss::Get().Delegate(event);
}

const Microsoft::WRL::ComPtr<ID3D11Device>& DXCore::GetDevice() const noexcept
{
	return m_pDevice;
}

const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& DXCore::GetDeviceContext() const noexcept
{
	return m_pDeviceContext;
}

void DXCore::ToggleDepthStencilState() noexcept
{
	m_SkyboxEnabled = !m_SkyboxEnabled;
	if (m_SkyboxEnabled == true)
	{
		m_pDeviceContext->OMSetDepthStencilState(m_pDepthStencilStateSkybox.Get(), 1u);
		if (!m_WireFrameEnabled)
			m_pDeviceContext->RSSetState(m_pRasterizerStateNoCull.Get());
		else
			m_pDeviceContext->RSSetState(m_pRasterizerStateNoCullWF.Get());
	}
	else
	{
		m_pDeviceContext->OMSetDepthStencilState(m_pDepthStencilStateDefault.Get(), 1u);
		if (!m_WireFrameEnabled)
			m_pDeviceContext->RSSetState(m_pRasterizerStateFill.Get());
		else
			m_pDeviceContext->RSSetState(m_pRasterizerStateWireFrame.Get());
	}
}

const Microsoft::WRL::ComPtr<IDXGISwapChain>& DXCore::GetSwapChain() const noexcept
{
	return m_pSwapChain;
}

const Microsoft::WRL::ComPtr<ID3D11RenderTargetView>& DXCore::GetBackBuffer() const noexcept
{
	return m_pBackBuffer;
}

const Microsoft::WRL::ComPtr<ID3D11DepthStencilView>& DXCore::GetDepthStencilView() const noexcept
{
	return m_pDepthStencilView;
}
