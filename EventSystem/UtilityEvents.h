#pragma once
#include "IEvent.h"
class PlayerCamera;
class DelegateDXEvent : public IEvent
{
private:
	Microsoft::WRL::ComPtr<ID3D11Device> m_ppDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_ppDeviceContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_ppBackBuffer;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_ppDepthStencilView;
public:
	DelegateDXEvent(Microsoft::WRL::ComPtr<ID3D11Device>& pdevice,
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pdeviceContext,
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView>& pbackBuffer,
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView>& pdepthStencilView) noexcept
	{
		m_ppDevice = pdevice;
		m_ppDeviceContext = pdeviceContext;
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
	[[nodiscard]] const Microsoft::WRL::ComPtr<ID3D11RenderTargetView>& GetBackBuffer() const
	{
		return m_ppBackBuffer;
	}
	[[nodiscard]] const Microsoft::WRL::ComPtr<ID3D11DepthStencilView>& GetDepthStencilView() const
	{
		return m_ppDepthStencilView;
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