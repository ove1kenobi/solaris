#include "..\pch.h"
#include "Button.h"
Button::Button() noexcept {
	m_pHoverBox = D2D1::RectF();
	m_pTextBox = D2D1::RectF();
	m_pTextPadding = 20.0f;
}

bool Button::Initialize() {
	return this->CreateBrush();
}

void Button::SetText(std::wstring text) {
	m_pText = text;
}

bool Button::UpdateModules() {
	return true;
}

void Button::Render() {

}

void Button::OnEvent(IEvent& event) noexcept {
	switch (event.GetEventType()) {
	case EventType::DelegateDXEvent:
	{
		UpdateDXHandlers(event);
		break;
	}
	case EventType::DelegateResolutionEvent:
	{
		m_pWindowWidth = static_cast<float>(static_cast<DelegateResolutionEvent*>(&event)->GetClientWindowWidth());
		m_pWindowHeight = static_cast<float>(static_cast<DelegateResolutionEvent*>(&event)->GetClientWindowHeight());
		this->UpdateModules();
		break;
	}
	default:
		break;
	}
}
