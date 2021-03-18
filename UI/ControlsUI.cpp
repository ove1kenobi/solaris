#include "..\pch.h"
#include "ControlsUI.h"

ControlsUI::ControlsUI() noexcept
	: m_IsHoveringBack{ false } 
{
	EventBuss::Get().AddListener(this, EventType::MouseButtonEvent);
	EventBuss::Get().AddListener(this, EventType::WindowCloseEvent);

	m_pScreen = D2D1::RectF();

	m_pBackTextBox = D2D1::RectF();
	m_pBackText = L"BACK";

	m_pButtonBoxE = D2D1::RectF();
	m_pButtonBoxDescriptionE = D2D1::RectF();
	m_pButtonTextE = L"E";
	m_pButtonTextDescriptionE = L"Send probe down to planet.";

	m_pButtonBoxQ = D2D1::RectF();
	m_pButtonBoxDescriptionQ = D2D1::RectF();
	m_pButtonTextQ = L"Q";
	m_pButtonTextDescriptionQ = L"Toggle ship stabilizer.";

	m_pButtonBoxW = D2D1::RectF();
	m_pButtonBoxDescriptionW = D2D1::RectF();
	m_pButtonTextW = L"W";
	m_pButtonTextDescriptionW = L"Accelerate forward.";

	m_pButtonBoxS = D2D1::RectF();
	m_pButtonBoxDescriptionS = D2D1::RectF();
	m_pButtonTextS = L"S";
	m_pButtonTextDescriptionS = L"Accelerate backwards";

	m_pButtonBoxU = D2D1::RectF();
	m_pButtonBoxDescriptionU = D2D1::RectF();
	m_pButtonTextU = L"U";
	m_pButtonTextDescriptionU = L"Toggle upgrade menu.";

	m_pMousePosition = D2D1::RectF();
	m_pButtonBoxDescriptionLeft = D2D1::RectF();
	m_pMouseBoxDescription = D2D1::RectF();
	m_pButtonTextDescriptionLeft = L"Left click to select events.";
	m_pMouseTextDescription = L"Use mouse to move.";

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
	if (!CreateMouse()) {
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

	ErrorCheck(m_pTextFactory->CreateTextFormat(
		L"Tenika",
		m_pTextFont.Get(),
		DWRITE_FONT_WEIGHT_REGULAR,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		30.0f,
		L"en-us",
		&m_pKeyFormat
	), "TextFormat");

	m_pKeyFormat.Get()->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);

	ErrorCheck(m_pTextFactory->CreateTextFormat(
		L"Tenika",
		m_pTextFont.Get(),
		DWRITE_FONT_WEIGHT_REGULAR,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		20.0f,
		L"en-us",
		&m_pTextFormat
	), "TextFormat");
    return true;
}

bool ControlsUI::CreateMouse() {
	LoadBitmapFromFile(GetIconFilePath(L"Mouse.png").c_str(), &m_pMouseBitmap);
	return true;
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

bool ControlsUI::UpdateKeys() {
	//Screen and title
	m_pKeyDisplay = D2D1::RectF(
		(m_pWindowWidth / 2.0f) - 450.0f,
		(m_pWindowHeight / 2.0f) - 300.0f,
		(m_pWindowWidth / 2.0f) + 450.0f,
		(m_pWindowHeight / 2.0f) + 350.0f
	);

	float offsetX = 30.0f;
	float offsetY = 40.0f;
	float buttonSize = 75.0f;
	float buttonDistance = 20.0f;
	//Button E
	m_pButtonE = D2D1::RoundedRect(
		D2D1::RectF(
			m_pKeyDisplay.left + offsetX,
			m_pKeyDisplay.top + offsetY,
			m_pKeyDisplay.left + buttonSize + offsetX,
			m_pKeyDisplay.top + buttonSize + offsetY
		),
		10.f,
		10.f
	);

	m_pButtonBoxE = D2D1::RectF(
		m_pKeyDisplay.left + offsetX,
		m_pKeyDisplay.top + offsetY + 20.0f,
		m_pKeyDisplay.left + buttonSize + offsetX,
		m_pKeyDisplay.top + buttonSize + offsetY
	);

	m_pButtonBoxDescriptionE = D2D1::RectF(
		m_pButtonBoxE.right + 10.0f,
		m_pButtonBoxE.top + 5.0f,
		m_pButtonBoxE.right + 400.0f,
		m_pButtonBoxE.bottom
	);

	//Button Q
	m_pButtonQ = D2D1::RoundedRect(
		D2D1::RectF(
			m_pKeyDisplay.left + offsetX,
			m_pKeyDisplay.top + offsetY + buttonSize + buttonDistance,
			m_pKeyDisplay.left + buttonSize + offsetX,
			m_pKeyDisplay.top + buttonSize * 2 + offsetY + buttonDistance
		),
		10.f,
		10.f
	);

	m_pButtonBoxQ = D2D1::RectF(
		m_pKeyDisplay.left + offsetX,
		m_pKeyDisplay.top + offsetY + buttonSize + buttonDistance + 20.0f,
		m_pKeyDisplay.left + buttonSize + offsetX,
		m_pKeyDisplay.top + buttonSize * 2 + offsetY + buttonDistance
	);

	m_pButtonBoxDescriptionQ = D2D1::RectF(
		m_pButtonBoxQ.right + 10.0f,
		m_pButtonBoxQ.top + 5.0f,
		m_pButtonBoxQ.right + 400.0f,
		m_pButtonBoxQ.bottom
	);

	//Button U
	m_pButtonU = D2D1::RoundedRect(
		D2D1::RectF(
			m_pKeyDisplay.left + offsetX,
			m_pKeyDisplay.top + offsetY + buttonSize * 2.0f + buttonDistance * 2.0f,
			m_pKeyDisplay.left + buttonSize + offsetX,
			m_pKeyDisplay.top + buttonSize * 3.0f + offsetY + buttonDistance * 2.0f
		),
		10.f,
		10.f
	);

	m_pButtonBoxU = D2D1::RectF(
		m_pKeyDisplay.left + offsetX,
		m_pKeyDisplay.top + offsetY + buttonSize * 2.0f + buttonDistance * 2.0f + 20.0f,
		m_pKeyDisplay.left + buttonSize + offsetX,
		m_pKeyDisplay.top + buttonSize * 3.0f + offsetY + buttonDistance * 2.0f
	);

	m_pButtonBoxDescriptionU = D2D1::RectF(
		m_pButtonBoxU.right + 10.0f,
		m_pButtonBoxU.top + 5.0f,
		m_pButtonBoxU.right + 400.0f,
		m_pButtonBoxU.bottom
	);


	//Button W
	m_pButtonW = D2D1::RoundedRect(
		D2D1::RectF(
			m_pKeyDisplay.left + offsetX,
			m_pKeyDisplay.top + offsetY * 2.0f + buttonSize * 3.0f + buttonDistance * 3.0f,
			m_pKeyDisplay.left + buttonSize + offsetX,
			m_pKeyDisplay.top + buttonSize * 4.0f + offsetY * 2.0f + buttonDistance * 3.0f
		),
		10.f,
		10.f
	);

	m_pButtonBoxW = D2D1::RectF(
		m_pKeyDisplay.left + offsetX,
		m_pKeyDisplay.top + offsetY * 2.0f + buttonSize * 3.0f + buttonDistance * 3.0f + 20.0f,
		m_pKeyDisplay.left + buttonSize + offsetX,
		m_pKeyDisplay.top + buttonSize * 4.0f + offsetY * 2.0f + buttonDistance * 3.0f
	);

	m_pButtonBoxDescriptionW = D2D1::RectF(
		m_pButtonBoxW.right + 10.0f,
		m_pButtonBoxW.top + 5.0f,
		m_pButtonBoxW.right + 400.0f,
		m_pButtonBoxW.bottom
	);


	//Button S
	m_pButtonS = D2D1::RoundedRect(
		D2D1::RectF(
			m_pKeyDisplay.left + offsetX,
			m_pKeyDisplay.top + offsetY * 2.0f + buttonSize * 4.0f + buttonDistance * 4.0f,
			m_pKeyDisplay.left + buttonSize + offsetX,
			m_pKeyDisplay.top + buttonSize * 5.0f + offsetY * 2.0f + buttonDistance * 4.0f
		),
		10.f,
		10.f
	);

	m_pButtonBoxS = D2D1::RectF(
		m_pKeyDisplay.left + offsetX,
		m_pKeyDisplay.top + offsetY * 2.0f + buttonSize * 4.0f + buttonDistance * 4.0f + 20.0f,
		m_pKeyDisplay.left + buttonSize + offsetX,
		m_pKeyDisplay.top + buttonSize * 5.0f + offsetY * 2.0f + buttonDistance * 4.0f
	);

	m_pButtonBoxDescriptionS = D2D1::RectF(
		m_pButtonBoxS.right + 10.0f,
		m_pButtonBoxS.top + 5.0f,
		m_pButtonBoxS.right + 400.0f,
		m_pButtonBoxS.bottom
	);

	return true;
}

bool ControlsUI::UpdateMouse() {
	float offsetX = 30.0f;
	float offsetY = 40.0f;
	float buttonSize = 75.0f;
	float buttonDistance = 20.0f;
	//Mouse
	m_pMousePosition = D2D1::RectF(
		m_pKeyDisplay.right - offsetX,
		m_pKeyDisplay.top + offsetX,
		m_pKeyDisplay.right - 450.0f - offsetX,
		m_pKeyDisplay.top + 450.0f + offsetX
	);

	m_pButtonBoxDescriptionLeft = D2D1::RectF(
		m_pKeyDisplay.right - offsetX,
		m_pKeyDisplay.top + offsetX + 90.0f,
		m_pKeyDisplay.right - 380.0f - offsetX,
		m_pMousePosition.top + 110.0f
	);

	m_pMouseBoxDescription = D2D1::RectF(
		m_pKeyDisplay.right - offsetX,
		m_pMousePosition.bottom - 25.0f,
		m_pKeyDisplay.right - 355.0f - offsetX,
		m_pKeyDisplay.bottom - offsetX
	);
	return true;
}

bool ControlsUI::UpdateModules() {
	if (!UpdateScreen()) {
		return false;
	}
	if (!UpdateBack()) {
		return false;
	}
	if (!UpdateKeys()) {
		return false;
	}
	if (!UpdateMouse()) {
		return false;
	}
    return true;
}

void ControlsUI::RenderScreen() {

}

void ControlsUI::RenderBack() {
	//if hover
	if (m_pMouseX > m_pBackTextBox.left && m_pMouseX < m_pBackTextBox.right &&
		m_pMouseY > m_pBackTextBox.top && m_pMouseY < m_pBackTextBox.bottom) {
		this->UpdateBrush(m_pHighlight, 1.0f);
		if (m_IsHoveringBack == false)
		{
			PlaySoundEvent playSoundEvent(SoundID::Hover, false);
			EventBuss::Get().Delegate(playSoundEvent);
			m_IsHoveringBack = true;
		}
	}
	else {
		this->UpdateBrush(m_pWhite, 1.0f);
		m_IsHoveringBack = false;
	}

	m_pRenderTarget2D.Get()->DrawTextW(
		m_pBackText.c_str(),
		(UINT32)m_pBackText.length(),
		m_pBackButtonFormat.Get(),
		m_pBackTextBox,
		m_pBrush.Get()
	);
}

void ControlsUI::RenderKeys() {
	this->UpdateBrush(m_pHighlight, 0.75f);
	m_pRenderTarget2D->FillRoundedRectangle(m_pButtonE, m_pBrush.Get());
	m_pRenderTarget2D->FillRoundedRectangle(m_pButtonQ, m_pBrush.Get());
	m_pRenderTarget2D->FillRoundedRectangle(m_pButtonW, m_pBrush.Get());
	m_pRenderTarget2D->FillRoundedRectangle(m_pButtonS, m_pBrush.Get());
	m_pRenderTarget2D->FillRoundedRectangle(m_pButtonU, m_pBrush.Get());

	this->UpdateBrush(m_pWhite, 1.0f);
	float edgeSize = 5.0f;
	m_pRenderTarget2D->DrawRoundedRectangle(m_pButtonE, m_pBrush.Get(), edgeSize);
	m_pRenderTarget2D->DrawRoundedRectangle(m_pButtonQ, m_pBrush.Get(), edgeSize);
	m_pRenderTarget2D->DrawRoundedRectangle(m_pButtonW, m_pBrush.Get(), edgeSize);
	m_pRenderTarget2D->DrawRoundedRectangle(m_pButtonS, m_pBrush.Get(), edgeSize);
	m_pRenderTarget2D->DrawRoundedRectangle(m_pButtonU, m_pBrush.Get(), edgeSize);

	//Button E
	this->UpdateBrush(m_pWhite, 1.0f);
	m_pRenderTarget2D.Get()->DrawTextW(
		m_pButtonTextE.c_str(),
		(UINT32)m_pButtonTextE.length(),
		m_pKeyFormat.Get(),
		m_pButtonBoxE,
		m_pBrush.Get()
	);

	m_pRenderTarget2D.Get()->DrawTextW(
		m_pButtonTextDescriptionE.c_str(),
		(UINT32)m_pButtonTextDescriptionE.length(),
		m_pTextFormat.Get(),
		m_pButtonBoxDescriptionE,
		m_pBrush.Get()
	);

	//Button Q
	this->UpdateBrush(m_pWhite, 1.0f);
	m_pRenderTarget2D.Get()->DrawTextW(
		m_pButtonTextQ.c_str(),
		(UINT32)m_pButtonTextQ.length(),
		m_pKeyFormat.Get(),
		m_pButtonBoxQ,
		m_pBrush.Get()
	);

	m_pRenderTarget2D.Get()->DrawTextW(
		m_pButtonTextDescriptionQ.c_str(),
		(UINT32)m_pButtonTextDescriptionQ.length(),
		m_pTextFormat.Get(),
		m_pButtonBoxDescriptionQ,
		m_pBrush.Get()
	);

	//Button W
	this->UpdateBrush(m_pWhite, 1.0f);
	m_pRenderTarget2D.Get()->DrawTextW(
		m_pButtonTextW.c_str(),
		(UINT32)m_pButtonTextW.length(),
		m_pKeyFormat.Get(),
		m_pButtonBoxW,
		m_pBrush.Get()
	);

	m_pRenderTarget2D.Get()->DrawTextW(
		m_pButtonTextDescriptionW.c_str(),
		(UINT32)m_pButtonTextDescriptionW.length(),
		m_pTextFormat.Get(),
		m_pButtonBoxDescriptionW,
		m_pBrush.Get()
	);

	//Button S
	this->UpdateBrush(m_pWhite, 1.0f);
	m_pRenderTarget2D.Get()->DrawTextW(
		m_pButtonTextS.c_str(),
		(UINT32)m_pButtonTextS.length(),
		m_pKeyFormat.Get(),
		m_pButtonBoxS,
		m_pBrush.Get()
	);

	m_pRenderTarget2D.Get()->DrawTextW(
		m_pButtonTextDescriptionS.c_str(),
		(UINT32)m_pButtonTextDescriptionS.length(),
		m_pTextFormat.Get(),
		m_pButtonBoxDescriptionS,
		m_pBrush.Get()
	);

	//Button U
	this->UpdateBrush(m_pWhite, 1.0f);
	m_pRenderTarget2D.Get()->DrawTextW(
		m_pButtonTextU.c_str(),
		(UINT32)m_pButtonTextU.length(),
		m_pKeyFormat.Get(),
		m_pButtonBoxU,
		m_pBrush.Get()
	);

	m_pRenderTarget2D.Get()->DrawTextW(
		m_pButtonTextDescriptionU.c_str(),
		(UINT32)m_pButtonTextDescriptionU.length(),
		m_pTextFormat.Get(),
		m_pButtonBoxDescriptionU,
		m_pBrush.Get()
	);
}

void ControlsUI::RenderMouse() {
	//Mouse
	if (m_pRenderBitmaps) {
		m_pRenderTarget2D->DrawBitmap(m_pMouseBitmap, m_pMousePosition);
	}

	this->UpdateBrush(m_pWhite, 1.0f);
	m_pRenderTarget2D.Get()->DrawTextW(
		m_pButtonTextDescriptionLeft.c_str(),
		(UINT32)m_pButtonTextDescriptionLeft.length(),
		m_pTextFormat.Get(),
		m_pButtonBoxDescriptionLeft,
		m_pBrush.Get()
	);

	m_pRenderTarget2D.Get()->DrawTextW(
		m_pMouseTextDescription.c_str(),
		(UINT32)m_pMouseTextDescription.length(),
		m_pTextFormat.Get(),
		m_pMouseBoxDescription,
		m_pBrush.Get()
	);
}

void ControlsUI::Render() {
	BeginFrame();

	RenderScreen();
	RenderKeys();
	RenderMouse();
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
				PlaySoundEvent playSoundEvent(SoundID::Click, false);
				EventBuss::Get().Delegate(playSoundEvent);
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

void ControlsUI::CleanUp() {
	m_pRenderBitmaps = false;
	if (m_pMouseBitmap) {
		m_pMouseBitmap->Release();
	}
}
