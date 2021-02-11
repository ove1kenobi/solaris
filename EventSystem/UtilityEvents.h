#pragma once
#include "IEvent.h"
class DelegateDXEvent : public IEvent
{
private:
	Microsoft::WRL::ComPtr<ID3D11Device> m_ppDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_ppDeviceContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain>			m_pSwapChain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_ppBackBuffer;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_ppDepthStencilView;
public:
	DelegateDXEvent(Microsoft::WRL::ComPtr<ID3D11Device>& pdevice,
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pdeviceContext,
		Microsoft::WRL::ComPtr<IDXGISwapChain>&	pswapChain,
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView>& pbackBuffer,
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView>& pdepthStencilView) noexcept
	{
		m_ppDevice = pdevice;
		m_ppDeviceContext = pdeviceContext;
		m_pSwapChain = pswapChain;
		m_ppBackBuffer = pbackBuffer;
		m_ppDepthStencilView = pdepthStencilView;
	}
	virtual ~DelegateDXEvent() noexcept = default;

	[[nodiscard]] const EventType GetEventType() const noexcept override
	{
		return EventType::DelegateDXEvent;
	}
	[[nodiscard]] const std::string GetDebugName() const noexcept override
	{
		return "DelegateDXEvent";
	}
	[[nodiscard]] const Microsoft::WRL::ComPtr<ID3D11Device>& GetDevice() const noexcept
	{
		return m_ppDevice;
	}
	[[nodiscard]] const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& GetDeviceContext() const
	{
		return m_ppDeviceContext;
	}
	[[nodiscard]] const Microsoft::WRL::ComPtr<IDXGISwapChain>& GetSwapChain() const
	{
		return m_pSwapChain;
	}
	[[nodiscard]] const Microsoft::WRL::ComPtr<ID3D11RenderTargetView>& GetBackBuffer() const
	{
		return m_ppBackBuffer;
	}
	[[nodiscard]] const Microsoft::WRL::ComPtr<ID3D11DepthStencilView>& GetDepthStencilView() const
	{
		return m_ppDepthStencilView;
	}

};