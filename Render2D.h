#pragma once
#include "pch.h"
#include "EventSystem/IEventListener.h"
#include "EventSystem/EventPublisher.h"
#include "EventSystem\UtilityEvents.h"

//Direct2D will be used to render all of the graphics, and use XAML for everything else UI related.
//XAML: https://docs.microsoft.com/en-us/windows/uwp/gaming/directx-and-xaml-interop
//Direct2D: https://docs.microsoft.com/en-us/windows/win32/direct2d/direct2d-and-direct3d-interoperation-overview

class Render2D : public EventPublisher, IEventListener {
private:
	//Microsoft::WRL::ComPtr<ID2D1Factory> m_pID2D1Factory;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_pBackBuffer;
	Microsoft::WRL::ComPtr<IDXGISwapChain> m_pSwapChain;
	//Microsoft::WRL::ComPtr<HWND> m_pHWND;	Holds the window handler

	void UpdateDXHandlers(IEvent& event) noexcept;
public:
	Render2D() noexcept;
	virtual ~Render2D() = default;

	[[nodiscard]] const bool Initialize() noexcept;
	void RenderUI(HWND hwnd);
	void OnEvent(IEvent& event) noexcept;
};
