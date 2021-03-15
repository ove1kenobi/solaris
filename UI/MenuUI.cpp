#include "..\pch.h"
#include "MenuUI.h"

MenuUI::MenuUI() noexcept {
	EventBuss::Get().AddListener(this, EventType::MouseButtonEvent);

	m_pTitleTextBox = D2D1::RectF();
	m_pTitleText = L"SOLARIS";
	m_pStartText = L"START";
	m_pExitText = L"EXIT GAME";

	m_pWhite = 0xFFFDF9;
	m_pHighlight = 0xFFB724;
}

MenuUI::~MenuUI() {
	EventBuss::Get().RemoveListener(this, EventType::MouseButtonEvent);
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
	float shadowOffset = 8.0f;
	m_pTitleTextBox = D2D1::RectF(
		(m_pWindowWidth / 2.0f) - 650.0f,
		0.0f,
		(m_pWindowWidth / 2.0f) + 650.0f,
		300.0f
	);

	m_pTitleTextShadowBox = D2D1::RectF(
		m_pTitleTextBox.left + shadowOffset,
		m_pTitleTextBox.top,
		m_pTitleTextBox.right + shadowOffset,
		m_pTitleTextBox.bottom
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

void MenuUI::RenderTitle() {
	this->UpdateBrush(m_pHighlight, 1.0f);
	m_pRenderTarget2D.Get()->DrawTextW(
		m_pTitleText.c_str(),
		(UINT32)m_pTitleText.length(),
		m_pTitleFormat.Get(),
		m_pTitleTextShadowBox,
		m_pBrush.Get()
	);

	this->UpdateBrush(m_pWhite, 1.0f);
	m_pRenderTarget2D.Get()->DrawTextW(
		m_pTitleText.c_str(),
		(UINT32)m_pTitleText.length(),
		m_pTitleFormat.Get(),
		m_pTitleTextBox,
		m_pBrush.Get()
	);
}

void MenuUI::RenderStart() {
	//if hover
	if (m_pMouseX > m_pStartTextBox.left && m_pMouseX < m_pStartTextBox.right &&
		m_pMouseY > m_pStartTextBox.top && m_pMouseY < m_pStartTextBox.bottom) {
		this->UpdateBrush(m_pHighlight, 1.0f);
	}
	else {
		this->UpdateBrush(m_pWhite, 1.0f);
	}
	m_pRenderTarget2D.Get()->DrawTextW(
		m_pStartText.c_str(),
		(UINT32)m_pStartText.length(),
		m_pButtonFormat.Get(),
		m_pStartTextBox,
		m_pBrush.Get()
	);
}

void MenuUI::RenderExit() {
	//if hover
	if (m_pMouseX > m_pExitTextBox.left && m_pMouseX < m_pExitTextBox.right &&
		m_pMouseY > m_pExitTextBox.top && m_pMouseY < m_pExitTextBox.bottom) {
		this->UpdateBrush(m_pHighlight, 1.0f);
	}
	else {
		this->UpdateBrush(m_pWhite, 1.0f);
	}
	m_pRenderTarget2D.Get()->DrawTextW(
		m_pExitText.c_str(),
		(UINT32)m_pExitText.length(),
		m_pButtonFormat.Get(),
		m_pExitTextBox,
		m_pBrush.Get()
	);
}

void MenuUI::Render() {
	this->BeginFrame();

	RenderTitle();
	RenderStart();
	RenderExit();

	this->EndFrame();
}

void MenuUI::OnEvent(IEvent& event) noexcept {
	switch (event.GetEventType()) {
	case EventType::DelegateDXEvent:
	{
		UpdateDXHandlers(event);
		break;
	}
	case EventType::DelegateMouseCoordsEvent:
	{
		m_pMouseX = static_cast<DelegateMouseCoordsEvent*>(&event)->GetXCoord();
		m_pMouseY = static_cast<DelegateMouseCoordsEvent*>(&event)->GetYCoord();
		break;
	}
	case EventType::DelegateResolutionEvent:
	{
		m_pWindowWidth = static_cast<float>(static_cast<DelegateResolutionEvent*>(&event)->GetClientWindowWidth());
		m_pWindowHeight = static_cast<float>(static_cast<DelegateResolutionEvent*>(&event)->GetClientWindowHeight());
		this->UpdateModules();
		break;
	}
	case EventType::MouseButtonEvent:
	{
		m_pMouseX = static_cast<MouseButtonEvent*>(&event)->GetXCoord();
		m_pMouseY = static_cast<MouseButtonEvent*>(&event)->GetYCoord();
		KeyState state = static_cast<MouseButtonEvent*>(&event)->GetKeyState();
		int virKey = static_cast<MouseButtonEvent*>(&event)->GetVirtualKeyCode();
		if (virKey == VK_LBUTTON && state == KeyState::KeyPress && !m_pOnScreen) {
			//something something, check where you clicked before doing stuff
			if (m_pMouseX > m_pStartTextBox.left && m_pMouseX < m_pStartTextBox.right &&
				m_pMouseY > m_pStartTextBox.top && m_pMouseY < m_pStartTextBox.bottom) {
				m_pStartText = L"Hello!";
				//Create start game event here
			}
			//oh and that stuff renders
			if (m_pMouseX > m_pExitTextBox.left && m_pMouseX < m_pExitTextBox.right &&
				m_pMouseY > m_pExitTextBox.top && m_pMouseY < m_pExitTextBox.bottom) {
				m_pExitText = L"I was clicked on!";
				//Are we able to create and render a screen beforehand asking if you're sure?
				//Create close window event here
			}
		}
		break;
	}
	default:
		break;
	}
}

void MenuUI::CleanUp() {
	//No clean up needed here
}
