#pragma once
#include "pch.h"
#include "EventSystem/IEventListener.h"
#include "EventSystem/EventPublisher.h"
#include "EventSystem\UtilityEvents.h"

//Direct2D will be used to render all of the graphics, and use XAML for everything else UI related (Layout, interactions).
//XAML: https://docs.microsoft.com/en-us/windows/uwp/gaming/directx-and-xaml-interop
//Direct2D: https://docs.microsoft.com/en-us/windows/win32/direct2d/direct2d-and-direct3d-interoperation-overview

//Will only know how to draw things based on instructions, should not keep the instructions
class Render2D : public EventPublisher, IEventListener {
private:
	//From the event handler
	Microsoft::WRL::ComPtr<ID2D1Factory> m_pFactory;
	Microsoft::WRL::ComPtr<ID2D1RenderTarget> m_pRenderTarget2D;

	//Planet interaction UI
	Microsoft::WRL::ComPtr<ID2D1GeometrySink> m_pSink;
	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> m_pBrushDarkBlue;
	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> m_pBrushLightBlue;

	D2D1_RECT_F m_pMainRectangle;
	Microsoft::WRL::ComPtr<ID2D1PathGeometry> m_pBottomLeft;
	Microsoft::WRL::ComPtr<ID2D1PathGeometry> m_pBottomRight;
	Microsoft::WRL::ComPtr<ID2D1PathGeometry> m_pTop;
	//---------------------
	void UpdateDXHandlers(IEvent& event) noexcept;
public:
	Render2D() noexcept;
	virtual ~Render2D() = default;

	[[nodiscard]] const bool Initialize() noexcept;
	void RenderUI();
	void OnEvent(IEvent& event) noexcept;
};
