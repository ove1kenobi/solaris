//TODO: Move includes to pch (Emil F)
#pragma once
#include "DXDebug.h"

class DXCore
{
private:
	Microsoft::WRL::ComPtr<ID3D11Device> m_pDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_pDeviceContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain> m_pSwapChain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_pBackBuffer;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_pDepthStencilView;
	D3D11_VIEWPORT m_DefaultViewport;
	unsigned int m_MSAAQuality;
public:
	DXCore() noexcept;
	virtual ~DXCore() = default;
	const bool Initialize(const unsigned int& clientWindowWidth, const unsigned int& clientWindowHeight, const HWND& windowHandle);
	[[nodiscard]] const Microsoft::WRL::ComPtr<ID3D11Device>& GetDevice() const noexcept;
	[[nodiscard]] const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& GetDeviceContext() const noexcept;
	[[nodiscard]] const Microsoft::WRL::ComPtr<IDXGISwapChain>& GetSwapChain() const noexcept;
};