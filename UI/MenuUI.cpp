#include "..\pch.h"
#include "MenuUI.h"

MenuUI::MenuUI() noexcept {
	m_pTitleTextBox = D2D1::RectF();
	m_pTitleText = L"SOLARIS";
	m_pStartText = L"START";
	m_pExitText = L"EXIT GAME";
}

MenuUI::~MenuUI() {

}

//Create functions
bool MenuUI::Initialize() {
	if (!CreateBrush()) {
		return false;
	}
	if (!CreateTitle()) {
		return false;
	}
	if (!CreateButtons()) {
		return false;
	}
	return true;
}

bool MenuUI::CreateTitle() {
	ErrorCheck(m_pTextFactory->GetSystemFontCollection(&m_pTextFont, false), "GetSystemFont");

	ErrorCheck(m_pTextFactory->CreateTextFormat(
		L"Netron",
		m_pTextFont.Get(),
		DWRITE_FONT_WEIGHT_REGULAR,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		180.0f,
		L"en-us",
		&m_pTitleFormat
	), "TextFormat");

	ErrorCheck(m_pTitleFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER), "TextAlignment");
	return true;
}

bool MenuUI::CreateButtons() {
	ErrorCheck(m_pTextFactory->CreateTextFormat(
		L"Tenika",
		m_pTextFont.Get(),
		DWRITE_FONT_WEIGHT_REGULAR,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		50.0f,
		L"en-us",
		&m_pButtonFormat
	), "TextFormat");

	ErrorCheck(m_pButtonFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER), "TextAlignment");
	return true;
}

//Update functions
bool MenuUI::UpdateTitle() {
	m_pTitleTextBox = D2D1::RectF(
		(m_pWindowWidth / 2.0f) - 650.0f,
		0.0f,
		(m_pWindowWidth / 2.0f) + 650.0f,
		300.0f
	);
	return true;
}

bool MenuUI::UpdateButtons() {
	m_pStartTextBox = D2D1::RectF(
		(m_pWindowWidth / 2.0f) - 120.0f,
		(m_pWindowHeight / 2.0f) - 50.0f,
		(m_pWindowWidth / 2.0f) + 120.0f,
		(m_pWindowHeight / 2.0f)
	);

	m_pExitTextBox = D2D1::RectF(
		(m_pWindowWidth / 2.0f) - 180.0f,
		(m_pWindowHeight / 2.0f) + 30.0f,
		(m_pWindowWidth / 2.0f) + 180.0f,
		(m_pWindowHeight / 2.0f) + 80.0f
	);
	return true;
}

bool MenuUI::UpdateModules() {
	if (!UpdateTitle()) {
		return false;
	}
	if (!UpdateButtons()) {
		return false;
	}
	return true;
}

void MenuUI::Render() {
	//Helping code only, will be removed later
	this->UpdateBrush(D2D1::ColorF::MediumPurple, 0.5f);
	//m_pRenderTarget2D->FillRectangle(m_pTitleTextBox, m_pBrush.Get());
	//m_pRenderTarget2D->FillRectangle(m_pStartTextBox, m_pBrush.Get());
	//m_pRenderTarget2D->FillRectangle(m_pExitTextBox, m_pBrush.Get());

	this->UpdateBrush(D2D1::ColorF::Snow, 1.0f);
	m_pRenderTarget2D.Get()->DrawTextW(
		m_pTitleText.c_str(),
		(UINT32)m_pTitleText.length(),
		m_pTitleFormat.Get(),
		m_pTitleTextBox,
		m_pBrush.Get()
	);

	m_pRenderTarget2D.Get()->DrawTextW(
		m_pStartText.c_str(),
		(UINT32)m_pStartText.length(),
		m_pButtonFormat.Get(),
		m_pStartTextBox,
		m_pBrush.Get()
	);

	m_pRenderTarget2D.Get()->DrawTextW(
		m_pExitText.c_str(),
		(UINT32)m_pExitText.length(),
		m_pButtonFormat.Get(),
		m_pExitTextBox,
		m_pBrush.Get()
	);

	//If mouse coords are within button, render hover
}

void MenuUI::OnEvent(IEvent& event) noexcept {
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
	//if mouse click event on button, create event
	//BUT ONLY if menuUI is active?
	default:
		break;
	}
}

void MenuUI::CleanUp() {

}
