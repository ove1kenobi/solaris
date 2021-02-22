#include "..\pch.h"
#include "Button.h"

bool Button::CreateButton() {
	return false;
}

void Button::RenderHover() {

}

void Button::RenderClick() {

}

Button::Button() noexcept {
	m_pHoverBox = D2D1::RectF(
		100.0f,
		50.0f,
		m_pWindowWidth - 100.0f,
		m_pWindowHeight - 200.0f
	);
}

bool Button::Initialize()
{
	return false;
}

bool Button::Initialize(D2D1_RECT_F hoverBox, float TextPadding) {
	m_pHoverBox = hoverBox;

	m_pTextBox = D2D1::RectF(
		m_pHoverBox.left + TextPadding,
		m_pHoverBox.top + TextPadding,
		m_pHoverBox.right - TextPadding,
		m_pHoverBox.bottom - TextPadding
	);

	return ErrorCheck(m_pTextFactory->CreateTextFormat(
		L"Arial",
		NULL,
		DWRITE_FONT_WEIGHT_REGULAR,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		16.0f,
		L"en-us",
		&m_pTextFormat
	), "TextFormat");
}

void Button::SetText(std::wstring text) {
	m_pText = text;
}

bool Button::UpdateModules()
{
	return false;
}

void Button::Render() {
	this->UpdateBrush(D2D1::ColorF::Green, 0.5f);
	m_pRenderTarget2D->FillRectangle(m_pHoverBox, m_pBrush.Get());

	/*
	//Event one
	m_pRenderTarget2D.Get()->DrawTextW(
		m_pEventOneText.c_str(),
		(UINT32)m_pEventOneText.length(),
		m_pBodyTextFormat.Get(),
		m_pEventOneTextBox,
		m_pBrush.Get()
	);

	m_pRenderTarget2D->DrawRectangle(m_pEventOneHoverTextBox, m_pBrush.Get(), 5.0f);

	if hover:
	m_pRenderTarget2D->FillGeometry(m_pEventHover.Get(), m_pBrush.Get());
	*/
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
