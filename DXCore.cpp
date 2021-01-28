#include "DXCore.h"
DXCore::DXCore() noexcept
	: m_pDevice{ nullptr }, 
	  m_pDeviceContext{ nullptr }, 
	  m_pSwapChain{ nullptr },
	  m_pBackBuffer{ nullptr },
	  m_pDepthStencilView{ nullptr },
	  m_DefaultViewport { 0 },
	  m_MSAAQuality{ 4u }
{

}

const bool DXCore::Initialize(const unsigned int& clientWindowWidth, 
							  const unsigned int& clientWindowHeight, 
							  const HWND& windowHandle)
{
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
	swapChainDescriptor.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;						//TODO: Research whether modern monitors support more than 24-bit color.
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

	D3D11_TEXTURE2D_DESC texture2DDescriptor = {};
	texture2DDescriptor.Width = clientWindowWidth;
	texture2DDescriptor.Height = clientWindowHeight;
	texture2DDescriptor.MipLevels = 1u;
	texture2DDescriptor.ArraySize = 1u;
	texture2DDescriptor.Format = DXGI_FORMAT_D32_FLOAT;				//No stencil part used if technique won't be used (Emil F)
	texture2DDescriptor.SampleDesc.Count = 4u;
	texture2DDescriptor.SampleDesc.Quality = m_MSAAQuality - 1u;
	texture2DDescriptor.Usage = D3D11_USAGE_DEFAULT;
	texture2DDescriptor.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	texture2DDescriptor.CPUAccessFlags = 0u;
	texture2DDescriptor.MiscFlags = 0u;

	Microsoft::WRL::ComPtr<ID3D11Texture2D> pDepthStencilBuffer = nullptr; //Might need to hold on to as member variable (Emil F)
	HR(m_pDevice->CreateTexture2D(&texture2DDescriptor, 
								  nullptr, 
								  &pDepthStencilBuffer), 
								  "CreateTexture2D");

	HR(m_pDevice->CreateDepthStencilView(pDepthStencilBuffer.Get(), 
										 nullptr, 
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
	return true;
}

const Microsoft::WRL::ComPtr<ID3D11Device>& DXCore::GetDevice() const noexcept
{
	return m_pDevice;
}

const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& DXCore::GetDeviceContext() const noexcept
{
	return m_pDeviceContext;
}

const Microsoft::WRL::ComPtr<IDXGISwapChain>& DXCore::GetSwapChain() const noexcept
{
	return m_pSwapChain;
}