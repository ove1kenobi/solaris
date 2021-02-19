#pragma once
#include "DXDebug.h"
#include "EventSystem\IEventListener.h"
#include "EventSystem\EventPublisher.h"
#include "EventSystem\UtilityEvents.h"
#include "EventSystem\RenderEvents.h"
#include "ImGui\imgui_impl_dx11.h"

#pragma comment(lib, "user32")
#pragma comment(lib, "gdi32")
#pragma comment(lib, "d3d11")
#pragma comment(lib, "d3dcompiler")
#pragma comment(lib, "d2d1")

class DXCore : public IEventListener, EventPublisher
{
private:
	Microsoft::WRL::ComPtr<ID3D11Device>			m_pDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>		m_pDeviceContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain>			m_pSwapChain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>	m_pBackBuffer;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>	m_pDepthStencilView;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>	m_pRasterizerStateFill;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>	m_pRasterizerStateWireFrame;
	Microsoft::WRL::ComPtr<ID2D1Factory>			m_pFactory;
	Microsoft::WRL::ComPtr<IDXGISurface>			m_pSurface;
	Microsoft::WRL::ComPtr<ID2D1RenderTarget>		m_pSurfaceRenderTarget;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>   m_pRasterizerStateNoCull;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>   m_pRasterizerStateNoCullWF;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_pDepthStencilStateDefault;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_pDepthStencilStateSkybox;
	D3D11_VIEWPORT	m_DefaultViewport;
	D3D11_VIEWPORT m_ShadowMapViewPort;
	unsigned int	m_MSAAQuality;
	bool			m_WireFrameEnabled;
	bool			m_SkyboxEnabled;
private:
	void OnEvent(IEvent& event) noexcept override;
	void ToggleWireFrame() noexcept;
	void ToggleDepthStencilState() noexcept;
	void DelegateDXHandles() noexcept;
public:
	DXCore() noexcept;
	virtual ~DXCore() noexcept = default;
	const bool Initialize(const unsigned int& clientWindowWidth, const unsigned int& clientWindowHeight, const HWND& windowHandle);
	void CreateShadowMapViewport(IEvent& event) noexcept;
	void ResetDefaultViewport() const noexcept;
	[[nodiscard]] const Microsoft::WRL::ComPtr<ID3D11Device>& GetDevice() const noexcept;
	[[nodiscard]] const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& GetDeviceContext() const noexcept;
	[[nodiscard]] const Microsoft::WRL::ComPtr<IDXGISwapChain>& GetSwapChain() const noexcept;
	[[nodiscard]] const Microsoft::WRL::ComPtr<ID3D11RenderTargetView>& GetBackBuffer() const noexcept;
	[[nodiscard]] const Microsoft::WRL::ComPtr<ID3D11DepthStencilView>& GetDepthStencilView() const noexcept;
	[[nodiscard]] const Microsoft::WRL::ComPtr<ID2D1Factory>& GetFactory() const noexcept;
	[[nodiscard]] const Microsoft::WRL::ComPtr<ID2D1RenderTarget>& GetSurfaceRenderTarget() const noexcept;
};