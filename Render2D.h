#pragma once
#include "pch.h"
#include "PlanetInteractionUI.h"
#include "EventSystem/IEventListener.h"
#include "EventSystem/EventPublisher.h"
//TODO: add keyboard events so it only renders when you press "E"

//Direct2D will be used to render all of the graphics, and use XAML for everything else UI related (Layout, interactions).
//XAML: https://docs.microsoft.com/en-us/windows/uwp/gaming/directx-and-xaml-interop
//Direct2D: https://docs.microsoft.com/en-us/windows/win32/direct2d/direct2d-and-direct3d-interoperation-overview

class Render2D : public EventPublisher, IEventListener {
private:
	PlanetInteractionUI* m_TestUI;
	bool m_Render;
public:
	Render2D() noexcept;
	virtual ~Render2D();

	[[nodiscard]] const bool Initialize() noexcept;
	void RenderUI();
	void OnEvent(IEvent& event) noexcept;
};
