#pragma once
#include "IEvent.h"
class PlayerCamera;
class Planet;
class Player;
struct PlayerInfo;
class Sun;
class DelegateDXEvent : public IEvent
{
private:
	Microsoft::WRL::ComPtr<ID3D11Device>				m_ppDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>			m_ppDeviceContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain>				m_pSwapChain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>		m_ppBackBuffer;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>		m_ppDepthStencilView;
	Microsoft::WRL::ComPtr<ID2D1Factory>				m_pFactory2D;
	Microsoft::WRL::ComPtr<ID2D1RenderTarget>			m_pSurfaceRenderTarget;
	Microsoft::WRL::ComPtr<IDWriteFactory>				m_pTextFactory;
	Microsoft::WRL::ComPtr<IWICImagingFactory>			m_pBitMapFactory;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	m_ppDepthShaderResourceView;
public:
	DelegateDXEvent(Microsoft::WRL::ComPtr<ID3D11Device>& pdevice,
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pdeviceContext,
		Microsoft::WRL::ComPtr<IDXGISwapChain>&	pswapChain,
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView>& pbackBuffer,
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView>& pdepthStencilView,
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& pdepthShaderResourceView,
		Microsoft::WRL::ComPtr<ID2D1Factory>& pfactory,
		Microsoft::WRL::ComPtr<ID2D1RenderTarget>& pSurfaceRenderTarget,
		Microsoft::WRL::ComPtr<IDWriteFactory>&	pTextFactory,
		Microsoft::WRL::ComPtr<IWICImagingFactory>& pBitMapFactory) noexcept
	{
		m_ppDevice = pdevice;
		m_ppDeviceContext = pdeviceContext;
		m_pSwapChain = pswapChain;
		m_ppBackBuffer = pbackBuffer;
		m_ppDepthStencilView = pdepthStencilView;
		m_ppDepthShaderResourceView = pdepthShaderResourceView; 
		m_pFactory2D = pfactory;
		m_pSurfaceRenderTarget = pSurfaceRenderTarget;
		m_pTextFactory = pTextFactory;
		m_pBitMapFactory = pBitMapFactory;

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
	[[nodiscard]] const Microsoft::WRL::ComPtr<ID2D1Factory>& GetFactory2D() const
	{
		return m_pFactory2D;
	}
	[[nodiscard]] const Microsoft::WRL::ComPtr<ID2D1RenderTarget>& GetSurfaceRenderTarget() const
	{
		return m_pSurfaceRenderTarget;
	}
	[[nodiscard]] const Microsoft::WRL::ComPtr<IDWriteFactory>& GetTextFactory() const
	{
		return m_pTextFactory;
	}
	[[nodiscard]] const Microsoft::WRL::ComPtr<IWICImagingFactory>& GetBitMapFactory() const
	{
		return m_pBitMapFactory;
	}
	[[nodiscard]] const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& GetShaderResourceView() const
	{
		return m_ppDepthShaderResourceView;
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

class SendDSVEvent : public IEvent
{
private:
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_pDepthStencilView;
public:
	SendDSVEvent(Microsoft::WRL::ComPtr<ID3D11DepthStencilView>& depthStencilView) noexcept
	{
		m_pDepthStencilView = depthStencilView;
	}
	virtual ~SendDSVEvent() noexcept = default;

	[[nodiscard]] const EventType GetEventType() const noexcept override
	{
		return EventType::SendDSVEvent;
	}
	[[nodiscard]] const std::string GetDebugName() const noexcept override
	{
		return "SendDSVEvent";
	}
	[[nodiscard]] Microsoft::WRL::ComPtr<ID3D11DepthStencilView> GetDepthStencilView() const noexcept
	{
		return m_pDepthStencilView;
	}
};

class RequestDSVEvent : public IEvent
{
private:
public:
	RequestDSVEvent() noexcept = default;
	virtual ~RequestDSVEvent() noexcept = default;

	[[nodiscard]] const EventType GetEventType() const noexcept override
	{
		return EventType::RequestDSVEvent;
	}
	[[nodiscard]] const std::string GetDebugName() const noexcept override
	{
		return "RequestDSVEvent";
	}
};

class RequestSunEvent : public IEvent
{
private:
public:
	RequestSunEvent() noexcept = default;
	virtual ~RequestSunEvent() noexcept = default;

	[[nodiscard]] const EventType GetEventType() const noexcept override
	{
		return EventType::RequestSunEvent;
	}
	[[nodiscard]] const std::string GetDebugName() const noexcept override
	{
		return "RequestSunEvent";
	}
};

class DelegateSunEvent : public IEvent
{
private:
	DirectX::XMFLOAT3* m_Center;
	float* m_Radius;
public:
	DelegateSunEvent(DirectX::XMFLOAT3* center, float* radius) noexcept
	{
		m_Center = center;
		m_Radius = radius;
	}
	virtual ~DelegateSunEvent() noexcept = default;

	[[nodiscard]] const EventType GetEventType() const noexcept override
	{
		return EventType::DelegateSunEvent;
	}
	[[nodiscard]] const std::string GetDebugName() const noexcept override
	{
		return "DelegateSunEvent";
	}
	[[nodiscard]] DirectX::XMFLOAT3* GetCenter() const noexcept
	{
		return m_Center;
	}
	[[nodiscard]] float* GetRadius() const noexcept
	{
		return m_Radius;
	}
};

class DelegatePlayerInfoEvent : public IEvent
{
private:
	PlayerInfo* m_pPlayerInfo;
public:
	DelegatePlayerInfoEvent(PlayerInfo* pPlayerInfo) noexcept
	{
		m_pPlayerInfo = pPlayerInfo;
	}
	virtual ~DelegatePlayerInfoEvent() noexcept = default;

	[[nodiscard]] const EventType GetEventType() const noexcept override
	{
		return EventType::DelegatePlayerInfoEvent;
	}
	[[nodiscard]] const std::string GetDebugName() const noexcept override
	{
		return "DelegatePlayerInfoEvent";
	}
	[[nodiscard]] PlayerInfo* GetPlayerInfo() const noexcept
	{
		return m_pPlayerInfo;
	}
};