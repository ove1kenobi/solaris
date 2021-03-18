#include "..\pch.h"
#include "EndgameUI.h"

EndgameUI::EndgameUI() noexcept {
	m_pTextBox = D2D1::RectF();
	m_pText = L"";
}

EndgameUI::~EndgameUI() {

}

bool EndgameUI::Initialize() {
	if (!CreateBrush()) {
		return false;
	}
	if (!CreateText()) {
		return false;
	}
	return true;
}

bool EndgameUI::CreateText() {
	ErrorCheck(m_pTextFactory->GetSystemFontCollection(&m_pFont, false), "GetSystemFont");

	ErrorCheck(m_pTextFactory->CreateTextFormat(
		L"Tenika",
		m_pFont.Get(),
		DWRITE_FONT_WEIGHT_REGULAR,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		90.0f,
		L"en-us",
		&m_pFormat
	), "TextFormat");

	ErrorCheck(m_pFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER), "TextAlignment");
	return true;
}

bool EndgameUI::UpdateModules() {
	m_pTextBox = D2D1::RectF(
		(m_pWindowWidth / 2.0f) - 650.0f,
		(m_pWindowHeight)-600.0f,
		(m_pWindowWidth / 2.0f) + 650.0f,
		(m_pWindowHeight)-500.0f
	);
	return true;
}

void EndgameUI::Render() {
	this->BeginFrame();

	this->UpdateBrush(D2D1::ColorF::Snow, 1.0f);
	m_pRenderTarget2D.Get()->DrawTextW(
		m_pText.c_str(),
		(UINT32)m_pText.length(),
		m_pFormat.Get(),
		m_pTextBox,
		m_pBrush.Get()
	);

	this->EndFrame();
}

void EndgameUI::OnEvent(IEvent& event) noexcept {
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

void EndgameUI::SetText(std::wstring text)
{
	m_pText = text;
}

void EndgameUI::CleanUp() {
	//No clean up needed here
}