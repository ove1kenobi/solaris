#pragma once
#include "pch.h"
#include "EventSystem/IEventListener.h"
#include "EventSystem/EventPublisher.h"

//Direct2D will be used to render all of the graphics, and use XAML for everything else UI related.
//XAML: https://docs.microsoft.com/en-us/windows/uwp/gaming/directx-and-xaml-interop
//Direct2D: https://docs.microsoft.com/en-us/windows/win32/direct2d/direct2d-and-direct3d-interoperation-overview

class Render2D : public EventPublisher, IEventListener {
private:
public:
	Render2D() noexcept;
	virtual ~Render2D() = default;

	[[nodiscard]] const bool Initialize() noexcept;
	void RenderUI();
	void OnEvent(IEvent& event) noexcept;
};

//The Render2D class needs to be able to know what has to rendered when, which is way it is an event listener
//If it get's input that the user interacted with a button, then it also needs to be an eventPublisher to inform what happens next


/*TODO:
- Look into what Direct2D needs to work with direct3D
- Try to render something
- EventHandler?
*/

