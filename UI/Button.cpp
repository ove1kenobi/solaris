#include "..\pch.h"
#include "Button.h"

/*A lot of functions are left empty here for now, as I've yet to need the button class.*/

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

void Button::SetHoverBox(D2D1_RECT_F hoverBox, float textPadding) {
	m_pHoverBox = hoverBox;
	m_pTextPadding = textPadding;
	UpdateModules();
}

bool Button::UpdateModules() {
	return true;
}

void Button::Render() {

}

void Button::Render(int mouseX, int mouseY) {

}

void Button::OnClick(int mouseX, int mouseY) {

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

void Button::CleanUp() {

}