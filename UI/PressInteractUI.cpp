#include "..\pch.h"
#include "PressInteractUI.h"

PressInteractUI::PressInteractUI() noexcept {
	m_pTextBox = D2D1::RectF();
	m_pText = L"[E] Orbit and launch probe";
}

PressInteractUI::~PressInteractUI() {

}

bool PressInteractUI::Initialize() {
	if (!CreateBrush()) {
		return false;
	}
	if (!CreateText()) {
		return false;
	}
	return true;
}

bool PressInteractUI::CreateText() {
	ErrorCheck(m_pTextFactory->GetSystemFontCollection(&m_pFont, false), "GetSystemFont");

	ErrorCheck(m_pTextFactory->CreateTextFormat(
		L"Tenika",
		m_pFont.Get(),
		DWRITE_FONT_WEIGHT_REGULAR,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		100.0f,
		L"en-us",
		&m_pFormat
	), "TextFormat");

	ErrorCheck(m_pFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER), "TextAlignment");
	return true;
}

bool PressInteractUI::UpdateModules() {
	m_pTextBox = D2D1::RectF(
		(m_pWindowWidth / 2.0f) - 650.0f,
		(m_pWindowHeight / 2.0f) - 100.0f,
		(m_pWindowWidth / 2.0f) + 650.0f,
		(m_pWindowHeight / 2.0f) + 100.0f
	);
	return true;
}

void PressInteractUI::Render() {
	this->UpdateBrush(D2D1::ColorF::MediumPurple, 0.5f);
	m_pRenderTarget2D->FillRectangle(m_pTextBox, m_pBrush.Get());

	this->UpdateBrush(D2D1::ColorF::Snow, 1.0f);
	m_pRenderTarget2D.Get()->DrawTextW(
		m_pText.c_str(),
		(UINT32)m_pText.length(),
		m_pFormat.Get(),
		m_pTextBox,
		m_pBrush.Get()
	);
}

void PressInteractUI::OnEvent(IEvent& event) noexcept {
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
