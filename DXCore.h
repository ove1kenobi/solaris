//TODO: Move includes to pch (Emil F)
#pragma once
#include "DXDebug.h"
#include "EventSystem\IEventListener.h"
#include "EventSystem\EventBuss.h"
#include "ImGui\imgui_impl_dx11.h"

class DXCore : public IEventListener
{
private:
	Microsoft::WRL::ComPtr<ID3D11Device> m_pDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_pDeviceContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain> m_pSwapChain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_pBackBuffer;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_pDepthStencilView;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_pRasterizerStateFill;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_pRasterizerStateWireFrame;
	D3D11_VIEWPORT m_DefaultViewport;
	unsigned int m_MSAAQuality;
	bool m_WireFrameEnabled;
public:
	DXCore() noexcept;
	virtual ~DXCore() noexcept;
	const bool Initialize(const unsigned int& clientWindowWidth, const unsigned int& clientWindowHeight, const HWND& windowHandle);
	void OnEvent(IEvent& event) noexcept override;
	void ToggleWireFrame() noexcept;
	[[nodiscard]] const Microsoft::WRL::ComPtr<ID3D11Device>& GetDevice() const noexcept;
	[[nodiscard]] const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& GetDeviceContext() const noexcept;
	[[nodiscard]] const Microsoft::WRL::ComPtr<IDXGISwapChain>& GetSwapChain() const noexcept;
	[[nodiscard]] const Microsoft::WRL::ComPtr<ID3D11RenderTargetView>& GetBackBuffer() const noexcept;
	[[nodiscard]] const Microsoft::WRL::ComPtr<ID3D11DepthStencilView>& GetDepthStencilView() const noexcept;
};