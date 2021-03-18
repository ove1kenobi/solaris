#include "..\pch.h"
#include "ControlsUI.h"

ControlsUI::ControlsUI() noexcept {
	EventBuss::Get().AddListener(this, EventType::MouseButtonEvent);
	EventBuss::Get().AddListener(this, EventType::WindowCloseEvent);

	m_pScreen = D2D1::RectF();

	m_pBackTextBox = D2D1::RectF();
	m_pBackText = L"BACK";

	m_pGray = 0x393b3d;
	m_pLightGray = 0x636363;
	m_pWhite = 0xFFFDF9;
	m_pHighlight = 0xFFB724;

	m_pRenderBitmaps = true;
}

ControlsUI::~ControlsUI() {
	EventBuss::Get().RemoveListener(this, EventType::WindowCloseEvent);
	EventBuss::Get().RemoveListener(this, EventType::MouseButtonEvent);
}

bool ControlsUI::Initialize() {
	if (!CreateBrush()) {
		return false;
	}
	if (!CreateScreen()) {
		return false;
	}
	return true;
}

bool ControlsUI::CreateScreen() {
	ErrorCheck(m_pTextFactory->GetSystemFontCollection(&m_pTextFont, false), "GetSystemFont");

	ErrorCheck(m_pTextFactory->CreateTextFormat(
		L"Tenika",
		m_pTextFont.Get(),
		DWRITE_FONT_WEIGHT_REGULAR,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		50.0f,
		L"en-us",
		&m_pBackButtonFormat
	), "TextFormat");

	ErrorCheck(m_pBackButtonFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER), "TextAlignment");
    return true;
}

bool ControlsUI::CreateKeys() {
	return false;
}

bool ControlsUI::CreateMouse()
{
	return false;
}

bool ControlsUI::UpdateScreen() {
	m_pScreen = D2D1::RectF(
		0.0f,
		0.0f,
		m_pWindowWidth,
		m_pWindowHeight
	);
    return true;
}

bool ControlsUI::UpdateBack() {
	m_pBackTextBox = D2D1::RectF(
		(m_pWindowWidth / 2.0f) - 120.0f,
		(m_pWindowHeight / 2.0f) + 400.0f,
		(m_pWindowWidth / 2.0f) + 120.0f,
		(m_pWindowHeight / 2.0f) + 450.0f
	);
	return true;
}

bool ControlsUI::UpdateKeys()
{
	return false;
}

bool ControlsUI::UpdateMouse()
{
	return false;
}

bool ControlsUI::UpdateModules() {
	if (!UpdateScreen()) {
		return false;
	}
	if (!UpdateBack()) {
		return false;
	}
    return true;
}

void ControlsUI::RenderScreen() {
	UpdateBrush(m_pGray, 0.5f);
	m_pRenderTarget2D->FillRectangle(m_pScreen, m_pBrush.Get());
}

void ControlsUI::RenderBack() {
	//if hover
	if (m_pMouseX > m_pBackTextBox.left && m_pMouseX < m_pBackTextBox.right &&
		m_pMouseY > m_pBackTextBox.top && m_pMouseY < m_pBackTextBox.bottom) {
		this->UpdateBrush(m_pHighlight, 1.0f);
	}
	else {
		this->UpdateBrush(m_pWhite, 1.0f);
	}

	m_pRenderTarget2D.Get()->DrawTextW(
		m_pBackText.c_str(),
		(UINT32)m_pBackText.length(),
		m_pBackButtonFormat.Get(),
		m_pBackTextBox,
		m_pBrush.Get()
	);
}

void ControlsUI::RenderKeys()
{
}

void ControlsUI::RenderMouse()
{
}

void ControlsUI::Render() {
	BeginFrame();

	RenderScreen();
	RenderBack();

	EndFrame();
}

void ControlsUI::OnEvent(IEvent& event) noexcept {
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
		if (virKey == VK_LBUTTON && state == KeyState::KeyPress && m_pOnScreen) {
			//Back button
			if (m_pMouseX > m_pBackTextBox.left && m_pMouseX < m_pBackTextBox.right &&
				m_pMouseY > m_pBackTextBox.top && m_pMouseY < m_pBackTextBox.bottom) {
				m_pOnScreen = false;
				ToggleControls tc;
				EventBuss::Get().Delegate(tc);
			}
		}
		break;
	}
	case EventType::WindowCloseEvent:
	{
		this->CleanUp();
	}
	default:
		break;
	}
}

void ControlsUI::CleanUp()
{
}
