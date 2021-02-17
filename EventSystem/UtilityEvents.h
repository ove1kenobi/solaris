#pragma once
#include "IEvent.h"
class PlayerCamera;
class DelegateDXEvent : public IEvent
{
private:
	Microsoft::WRL::ComPtr<ID3D11Device>			m_ppDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>		m_ppDeviceContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain>			m_pSwapChain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>  m_ppBackBuffer;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>  m_ppDepthStencilView;
	Microsoft::WRL::ComPtr<ID2D1Factory>			m_pFactory;
	Microsoft::WRL::ComPtr<ID2D1RenderTarget>		m_pSurfaceRenderTarget;
public:
	DelegateDXEvent(Microsoft::WRL::ComPtr<ID3D11Device>& pdevice,
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pdeviceContext,
		Microsoft::WRL::ComPtr<IDXGISwapChain>&	pswapChain,
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView>& pbackBuffer,
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView>& pdepthStencilView,
		Microsoft::WRL::ComPtr<ID2D1Factory>& pfactory,
		Microsoft::WRL::ComPtr<ID2D1RenderTarget>& pSurfaceRenderTarget) noexcept
	{
		m_ppDevice = pdevice;
		m_ppDeviceContext = pdeviceContext;
		m_pSwapChain = pswapChain;
		m_ppBackBuffer = pbackBuffer;
		m_ppDepthStencilView = pdepthStencilView;
		m_pFactory = pfactory;
		m_pSurfaceRenderTarget = pSurfaceRenderTarget;

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
	[[nodiscard]] const Microsoft::WRL::ComPtr<ID2D1Factory>& GetFactory() const
	{
		return m_pFactory;
	}
	[[nodiscard]] const Microsoft::WRL::ComPtr<ID2D1RenderTarget>& GetSurfaceRenderTarget() const
	{
		return m_pSurfaceRenderTarget;
	}

};

class RequestCameraEvent : public IEvent
{
private:
public:
	RequestCameraEvent() noexcept = default;
	virtual ~RequestCameraEvent() noexcept = default;

	[[nodiscard]] const EventType GetEventType() const noexcept override
	{
		return EventType::RequestCameraEvent;
	}
	[[nodiscard]] const std::string GetDebugName() const noexcept override
	{
		return "RequestCameraEvent";
	}
};

class DelegateCameraEvent : public IEvent
{
private:
	PlayerCamera* m_Camera;
public:
	DelegateCameraEvent(PlayerCamera* camera) noexcept
	{
		m_Camera = camera;
	}
	virtual ~DelegateCameraEvent() noexcept = default;

	[[nodiscard]] const EventType GetEventType() const noexcept override
	{
		return EventType::DelegateCameraEvent;
	}
	[[nodiscard]] const std::string GetDebugName() const noexcept override
	{
		return "DelegateCameraEvent";
	}
	[[nodiscard]] PlayerCamera* GetCamera() const noexcept
	{
		return m_Camera;
	}
};