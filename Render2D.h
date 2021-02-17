#pragma once
#include "pch.h"
#include "EventSystem/IEventListener.h"
#include "EventSystem/EventPublisher.h"
#include "EventSystem\UtilityEvents.h"

//Direct2D will be used to render all of the graphics, and use XAML for everything else UI related (Layout, interactions).
//XAML: https://docs.microsoft.com/en-us/windows/uwp/gaming/directx-and-xaml-interop
//Direct2D: https://docs.microsoft.com/en-us/windows/win32/direct2d/direct2d-and-direct3d-interoperation-overview

class Render2D : public EventPublisher, IEventListener {
private:
	//From the event handler
	Microsoft::WRL::ComPtr<ID2D1Factory> m_pFactory;
	Microsoft::WRL::ComPtr<ID2D1RenderTarget> m_pRenderTarget2D;

	//Saved here for now as it is only for the example
	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> m_pBrush;
	void UpdateDXHandlers(IEvent& event) noexcept;
public:
	Render2D() noexcept;
	virtual ~Render2D() = default;

	[[nodiscard]] const bool Initialize() noexcept;
	void RenderUI();
	void OnEvent(IEvent& event) noexcept;
};
