#include "..\pch.h"
#include "PauseUI.h"

PauseUI::PauseUI() noexcept {
	EventBuss::Get().AddListener(this, EventType::MouseButtonEvent);
	EventBuss::Get().AddListener(this, EventType::WindowCloseEvent);

	m_pTitleTextBox = D2D1::RectF();
	m_pTitleText = L"PAUSE";
	m_pReturnText = L"RETURN";
	m_pExitText = L"EXIT GAME";

	m_pWhite = 0xFFFDF9;
	m_pHighlight = 0xFFB724;
	m_pCyan = 0x00ffff;
	m_pLightBlue = 0x0BA4CC;
	m_pDarkblue = 0x2741b4;

	m_IsHoveringReturn = false;
	m_IsHoveringExit = false;
}

PauseUI::~PauseUI() {
	EventBuss::Get().RemoveListener(this, EventType::WindowCloseEvent);
	EventBuss::Get().RemoveListener(this, EventType::MouseButtonEvent);
}

//Create functions
bool PauseUI::Initialize() {
	if (!CreateBrush()) {
		return false;
	}
	if (!CreateTools()) {
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

bool PauseUI::CreateTitle() {
	ErrorCheck(m_pTextFactory->GetSystemFontCollection(&m_pTextFont, false), "GetSystemFont");

	ErrorCheck(m_pTextFactory->CreateTextFormat(
		L"Netron",
		m_pTextFont.Get(),
		DWRITE_FONT_WEIGHT_REGULAR,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		60.0f,
		L"en-us",
		&m_pTitleFormat
	), "TextFormat");

	ErrorCheck(m_pTitleFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER), "TextAlignment");
	return true;
}

bool PauseUI::CreateButtons() {
	ErrorCheck(m_pTextFactory->CreateTextFormat(
		L"Tenika",
		m_pTextFont.Get(),
		DWRITE_FONT_WEIGHT_REGULAR,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		40.0f,
		L"en-us",
		&m_pButtonFormat
	), "TextFormat");

	ErrorCheck(m_pButtonFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER), "TextAlignment");
	return true;
}

bool PauseUI::CreateTools() {
	//Main gradient
	D2D1_GRADIENT_STOP gradientStops[3];
	gradientStops[0].color = D2D1::ColorF(m_pLightBlue, 0.25f);
	gradientStops[0].position = 0.0f;
	gradientStops[1].color = D2D1::ColorF(m_pCyan, 0.25f);
	gradientStops[1].position = 0.5f;
	gradientStops[2].color = D2D1::ColorF(m_pLightBlue, 0.25f);
	gradientStops[2].position = 1.0f;

	ErrorCheck(m_pRenderTarget2D->CreateGradientStopCollection(
		gradientStops,
		3,
		D2D1_GAMMA_1_0,
		D2D1_EXTEND_MODE_CLAMP,
		&m_pMainGradientStops
	), "GradientStops");

	ErrorCheck(m_pRenderTarget2D->CreateLinearGradientBrush(
		D2D1::LinearGradientBrushProperties(
			D2D1::Point2F(m_pScreen.left, m_pWindowHeight),
			D2D1::Point2F(m_pScreen.right, m_pWindowHeight)),
		m_pMainGradientStops.Get(),
		&m_pMainGradientBrush), "GradientBrush");
	return true;
}

//Update functions
bool PauseUI::UpdateTitle() {
	m_pTitleTextBox = D2D1::RectF(
		(m_pWindowWidth / 2.0f) - 200.0f,
		(m_pWindowHeight / 2.0f) - 210.0f,
		(m_pWindowWidth / 2.0f) + 200.0f,
		(m_pWindowHeight / 2.0f) - 130.0f
	);
	return true;
}

bool PauseUI::UpdateButtons() {
	float buttonSize = 50.0f;
	float buttonPadding = 25.0f;
	//Height 50
	m_pReturnTextBox = D2D1::RectF(
		(m_pWindowWidth / 2.0f) - 120.0f,
		(m_pWindowHeight / 2.0f) - 100.0f,
		(m_pWindowWidth / 2.0f) + 120.0f,
		(m_pWindowHeight / 2.0f) - 50.0f
	);

	//Height 50
	m_pExitTextBox = D2D1::RectF(
		(m_pWindowWidth / 2.0f) - 180.0f,
		m_pReturnTextBox.bottom + buttonPadding,
		(m_pWindowWidth / 2.0f) + 180.0f,
		m_pReturnTextBox.bottom + buttonSize + buttonPadding
	);
	return true;
}

bool PauseUI::UpdateScreen() {
	m_pScreen = D2D1::RectF(
		(m_pWindowWidth / 2.0f) - 150.0f,
		(m_pWindowHeight / 2.0f) - 150.0f,
		(m_pWindowWidth / 2.0f) + 150.0f,
		(m_pWindowHeight / 2.0f) + 50.0f
	);
	return true;
}

bool PauseUI::UpdateTools() {
	m_pMainGradientBrush->SetStartPoint(D2D1::Point2F(m_pScreen.left, m_pWindowHeight));
	m_pMainGradientBrush->SetEndPoint(D2D1::Point2F(m_pScreen.right, m_pWindowHeight));
	return true;
}

bool PauseUI::UpdateModules() {
	if (!UpdateScreen()) {
		return false;
	}
	if (!UpdateTitle()) {
		return false;
	}
	if (!UpdateButtons()) {
		return false;
	}
	if (!UpdateTools()) {
		return false;
	}
	return true;
}

void PauseUI::RenderTitle() {
	this->UpdateBrush(m_pHighlight, 1.0f);
	m_pRenderTarget2D.Get()->DrawTextW(
		m_pTitleText.c_str(),
		(UINT32)m_pTitleText.length(),
		m_pTitleFormat.Get(),
		m_pTitleTextBox,
		m_pBrush.Get()
	);
}

void PauseUI::RenderReturn() {
	//if hover
	if (m_pMouseX > m_pReturnTextBox.left && m_pMouseX < m_pReturnTextBox.right &&
		m_pMouseY > m_pReturnTextBox.top && m_pMouseY < m_pReturnTextBox.bottom) {
		this->UpdateBrush(m_pHighlight, 1.0f);
		if (m_IsHoveringReturn == false)
		{
			//Do sound:
			PlaySoundEvent playSoundEvent(SoundID::Hover, false);
			EventBuss::Get().Delegate(playSoundEvent);
			m_IsHoveringReturn = true;
		}
	}
	else {
		this->UpdateBrush(m_pWhite, 1.0f);
		m_IsHoveringReturn = false;
	}
	m_pRenderTarget2D.Get()->DrawTextW(
		m_pReturnText.c_str(),
		(UINT32)m_pReturnText.length(),
		m_pButtonFormat.Get(),
		m_pReturnTextBox,
		m_pBrush.Get()
	);
}

void PauseUI::RenderExit() {
	//if hover
	if (m_pMouseX > m_pExitTextBox.left && m_pMouseX < m_pExitTextBox.right &&
		m_pMouseY > m_pExitTextBox.top && m_pMouseY < m_pExitTextBox.bottom) {
		this->UpdateBrush(m_pHighlight, 1.0f);
		if (m_IsHoveringExit == false)
		{
			//Do sound:
			PlaySoundEvent playSoundEvent(SoundID::Hover, false);
			EventBuss::Get().Delegate(playSoundEvent);
			m_IsHoveringExit = true;
		}
	}
	else {
		this->UpdateBrush(m_pWhite, 1.0f);
		m_IsHoveringExit = false;
	}
	m_pRenderTarget2D.Get()->DrawTextW(
		m_pExitText.c_str(),
		(UINT32)m_pExitText.length(),
		m_pButtonFormat.Get(),
		m_pExitTextBox,
		m_pBrush.Get()
	);
}

void PauseUI::RenderScreen() {
	this->UpdateBrush(m_pDarkblue, 0.5f);
	m_pRenderTarget2D->FillRectangle(m_pScreen, m_pBrush.Get());

	//Grid for the square
	unsigned int gridSize = 25;
	this->UpdateBrush(m_pCyan, 0.25f);
	for (float x = m_pScreen.left; x < m_pScreen.right; x += gridSize)
	{
		m_pRenderTarget2D->DrawLine(
			D2D1::Point2F(static_cast<FLOAT>(x), m_pScreen.top),
			D2D1::Point2F(static_cast<FLOAT>(x), m_pScreen.bottom),
			m_pBrush.Get(),
			0.5f
		);
	}

	for (float y = m_pScreen.top; y < m_pScreen.bottom; y += gridSize)
	{
		m_pRenderTarget2D->DrawLine(
			D2D1::Point2F(m_pScreen.left, static_cast<FLOAT>(y)),
			D2D1::Point2F(m_pScreen.right, static_cast<FLOAT>(y)),
			m_pBrush.Get(),
			0.5f
		);
	}

	m_pRenderTarget2D->FillRectangle(&m_pScreen, m_pMainGradientBrush.Get());

	this->UpdateBrush(m_pWhite, 0.5f);
	m_pRenderTarget2D->DrawRectangle(m_pScreen, m_pBrush.Get());
}

void PauseUI::Render() {
	this->BeginFrame();

	RenderScreen();
	RenderTitle();
	RenderReturn();
	RenderExit();

	this->EndFrame();
}

void PauseUI::OnEvent(IEvent& event) noexcept {
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
			//Start game
			if (m_pMouseX > m_pReturnTextBox.left && m_pMouseX < m_pReturnTextBox.right &&
				m_pMouseY > m_pReturnTextBox.top && m_pMouseY < m_pReturnTextBox.bottom) {
				m_pOnScreen = false;
				PlaySoundEvent playSoundEvent(SoundID::Click, false);
				EventBuss::Get().Delegate(playSoundEvent);
				TogglePauseGame pg;
				EventBuss::Get().Delegate(pg);
			}
			//Exit game
			if (m_pMouseX > m_pExitTextBox.left && m_pMouseX < m_pExitTextBox.right &&
				m_pMouseY > m_pExitTextBox.top && m_pMouseY < m_pExitTextBox.bottom) {
				m_pOnScreen = false;
				PlaySoundEvent playSoundEvent(SoundID::Click, false);
				EventBuss::Get().Delegate(playSoundEvent);
				GameOverEvent ov(false);
				EventBuss::Get().Delegate(ov);
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

void PauseUI::CleanUp() {
	//Does not need any clean up
}
