#include "..\pch.h"
#include "CreditsUI.h"

CreditsUI::CreditsUI() noexcept
	: m_IsHoveringBack{ false }
{
	EventBuss::Get().AddListener(this, EventType::MouseButtonEvent);

	m_pScreen = D2D1::RectF();

	m_pBackTextBox = D2D1::RectF();
	m_pBackText = L"BACK";

	m_pTitlePictureTextBox = D2D1::RectF();
	m_pPictureTextBox = D2D1::RectF();
	m_pTitlePictureText = L"IMAGES";
	m_pPictureText = L"Sun texture by NASA, All icons by game-icons.net";

	m_pTitleSoundTextBox = D2D1::RectF();
	m_pSoundTextBox = D2D1::RectF();
	m_pTitleSoundText = L"SOUND";
	m_pSoundText = L"Additional sound effects from zapsplat.com“";

	m_pTitleMusicTextBox = D2D1::RectF();
	m_pMusicTextBox = D2D1::RectF();
	m_pTitleMusicText = L"MUSIC";
	m_pMusicText = L"Stasis (music for space) by Sondre Drakensson";

	m_pTitleFontsTextBox = D2D1::RectF();
	m_pFontsTextBox = D2D1::RectF();
	m_pTitleFontsText = L"FONTS";
	m_pFontsText = L"Tenika by Wahyu S. Adi Wibowo, Aware by Lyric Type, Netron by linecreative, Neoterique by Hareesh Seela";

	m_pTitleModelsTextBox = D2D1::RectF();
	m_pModelsTextBox = D2D1::RectF();
	m_pTitleModelsText = L"MODELS";
	m_pModelsText = L"Spaceship designed, modeled and textured by Brando_Freeman";

	m_pTitleDeveloperTextBox = D2D1::RectF();
	m_pDeveloperTextBox = D2D1::RectF();
	m_pTitleDeveloperText = L"DEVELOPERS";
	m_pDeveloperText = L"Emil Fransson, Emil Högstedt, Stephanie Sundqvist Tepfers,  Adrian Tegelind, Ove Ødegård";

	m_pGray = 0x393b3d;
	m_pLightGray = 0x636363;
	m_pWhite = 0xFFFDF9;
	m_pHighlight = 0xFFB724;
}

CreditsUI::~CreditsUI() {
	EventBuss::Get().RemoveListener(this, EventType::MouseButtonEvent);
}

bool CreditsUI::Initialize() {
	if (!CreateBrush()) {
		return false;
	}
    if (!CreateScreen()) {
        return false;
    }
    return true;
}

bool CreditsUI::CreateScreen() {
	ErrorCheck(m_pTextFactory->GetSystemFontCollection(&m_pTextFont, false), "GetSystemFont");

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

	ErrorCheck(m_pTextFactory->CreateTextFormat(
		L"Aware",
		m_pTextFont.Get(),
		DWRITE_FONT_WEIGHT_REGULAR,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		36.0f,
		L"en-us",
		&m_pTitleFormat
	), "TextFormat");

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

bool CreditsUI::UpdateTitles() {
	float titleSize = 50.0f;
	float offSetY = 60.0f;
	m_pTitlePictureTextBox = D2D1::RectF(
		(m_pWindowWidth / 2.0f) - 450.0f,
		(m_pWindowHeight / 2.0f) - 300.0f,
		(m_pWindowWidth / 2.0f) + 450.0f,
		(m_pWindowHeight / 2.0f) - 250.0f
	);

	m_pTitleSoundTextBox = D2D1::RectF(
		m_pTitlePictureTextBox.left,
		m_pTitlePictureTextBox.bottom + offSetY,
		m_pTitlePictureTextBox.right,
		m_pTitlePictureTextBox.bottom + offSetY + titleSize
	);

	m_pTitleMusicTextBox = D2D1::RectF(
		m_pTitleSoundTextBox.left,
		m_pTitleSoundTextBox.bottom + offSetY,
		m_pTitleSoundTextBox.right,
		m_pTitleSoundTextBox.bottom + offSetY + titleSize
	);

	m_pTitleFontsTextBox = D2D1::RectF(
		m_pTitleMusicTextBox.left,
		m_pTitleMusicTextBox.bottom + offSetY,
		m_pTitleMusicTextBox.right,
		m_pTitleMusicTextBox.bottom + offSetY + titleSize
	);

	m_pTitleModelsTextBox = D2D1::RectF(
		m_pTitleFontsTextBox.left,
		m_pTitleFontsTextBox.bottom + offSetY,
		m_pTitleFontsTextBox.right,
		m_pTitleFontsTextBox.bottom + offSetY + titleSize
	);

	m_pTitleDeveloperTextBox = D2D1::RectF(
		m_pTitleModelsTextBox.left,
		m_pTitleModelsTextBox.bottom + offSetY,
		m_pTitleModelsTextBox.right,
		m_pTitleModelsTextBox.bottom + offSetY + titleSize
	);
	return true;
}

bool CreditsUI::UpdateText() {
	float padding = 5.0f;
	float textSize = 30.0f;
	m_pPictureTextBox = D2D1::RectF(
		m_pTitlePictureTextBox.left,
		m_pTitlePictureTextBox.bottom + padding,
		m_pTitlePictureTextBox.right,
		m_pTitlePictureTextBox.bottom + padding + textSize
	);

	m_pSoundTextBox = D2D1::RectF(
		m_pTitleSoundTextBox.left,
		m_pTitleSoundTextBox.bottom + padding,
		m_pTitleSoundTextBox.right,
		m_pTitleSoundTextBox.bottom + padding + textSize
	);

	m_pMusicTextBox = D2D1::RectF(
		m_pTitleMusicTextBox.left,
		m_pTitleMusicTextBox.bottom + padding,
		m_pTitleMusicTextBox.right,
		m_pTitleMusicTextBox.bottom + padding + textSize
	);

	m_pFontsTextBox = D2D1::RectF(
		m_pTitleFontsTextBox.left,
		m_pTitleFontsTextBox.bottom + padding,
		m_pTitleFontsTextBox.right,
		m_pTitleFontsTextBox.bottom + padding + textSize
	);

	m_pDeveloperTextBox = D2D1::RectF(
		m_pTitleDeveloperTextBox.left,
		m_pTitleDeveloperTextBox.bottom + padding,
		m_pTitleDeveloperTextBox.right,
		m_pTitleDeveloperTextBox.bottom + padding + textSize
	);

	m_pModelsTextBox = D2D1::RectF(
		m_pTitleModelsTextBox.left,
		m_pTitleModelsTextBox.bottom + padding,
		m_pTitleModelsTextBox.right,
		m_pTitleModelsTextBox.bottom + padding + textSize
	);
	return true;
}

bool CreditsUI::UpdateScreen() {
	m_pScreen = D2D1::RectF(
		0.0f,
		0.0f,
		m_pWindowWidth,
		m_pWindowHeight
	);

	m_pBackTextBox = D2D1::RectF(
		(m_pWindowWidth / 2.0f) - 120.0f,
		(m_pWindowHeight / 2.0f) + 400.0f,
		(m_pWindowWidth / 2.0f) + 120.0f,
		(m_pWindowHeight / 2.0f) + 450.0f
	);
    return true;
}

bool CreditsUI::UpdateModules() {
    if (!UpdateScreen()) {
        return false;
    }
	if (!UpdateTitles()) {
		return false;
	}
	if (!UpdateText()) {
		return false;
	}
    return true;
}

void CreditsUI::RenderScreen() {
	//UpdateBrush(m_pGray, 0.5f);
	//m_pRenderTarget2D->FillRectangle(m_pScreen, m_pBrush.Get());
}

void CreditsUI::RenderTitle() {
	UpdateBrush(m_pHighlight, 1.0f);
	m_pRenderTarget2D.Get()->DrawTextW(
		m_pTitlePictureText.c_str(),
		(UINT32)m_pTitlePictureText.length(),
		m_pTitleFormat.Get(),
		m_pTitlePictureTextBox,
		m_pBrush.Get()
	);

	m_pRenderTarget2D.Get()->DrawTextW(
		m_pTitleSoundText.c_str(),
		(UINT32)m_pTitleSoundText.length(),
		m_pTitleFormat.Get(),
		m_pTitleSoundTextBox,
		m_pBrush.Get()
	);

	m_pRenderTarget2D.Get()->DrawTextW(
		m_pTitleMusicText.c_str(),
		(UINT32)m_pTitleMusicText.length(),
		m_pTitleFormat.Get(),
		m_pTitleMusicTextBox,
		m_pBrush.Get()
	);

	m_pRenderTarget2D.Get()->DrawTextW(
		m_pTitleFontsText.c_str(),
		(UINT32)m_pTitleFontsText.length(),
		m_pTitleFormat.Get(),
		m_pTitleFontsTextBox,
		m_pBrush.Get()
	);

	m_pRenderTarget2D.Get()->DrawTextW(
		m_pTitleModelsText.c_str(),
		(UINT32)m_pTitleModelsText.length(),
		m_pTitleFormat.Get(),
		m_pTitleModelsTextBox,
		m_pBrush.Get()
	);

	m_pRenderTarget2D.Get()->DrawTextW(
		m_pTitleDeveloperText.c_str(),
		(UINT32)m_pTitleDeveloperText.length(),
		m_pTitleFormat.Get(),
		m_pTitleDeveloperTextBox,
		m_pBrush.Get()
	);
}

void CreditsUI::RenderText() {
	UpdateBrush(m_pWhite, 1.0f);
	m_pRenderTarget2D.Get()->DrawTextW(
		m_pPictureText.c_str(),
		(UINT32)m_pPictureText.length(),
		m_pTextFormat.Get(),
		m_pPictureTextBox,
		m_pBrush.Get()
	);

	m_pRenderTarget2D.Get()->DrawTextW(
		m_pSoundText.c_str(),
		(UINT32)m_pSoundText.length(),
		m_pTextFormat.Get(),
		m_pSoundTextBox,
		m_pBrush.Get()
	);

	m_pRenderTarget2D.Get()->DrawTextW(
		m_pMusicText.c_str(),
		(UINT32)m_pMusicText.length(),
		m_pTextFormat.Get(),
		m_pMusicTextBox,
		m_pBrush.Get()
	);

	m_pRenderTarget2D.Get()->DrawTextW(
		m_pFontsText.c_str(),
		(UINT32)m_pFontsText.length(),
		m_pTextFormat.Get(),
		m_pFontsTextBox,
		m_pBrush.Get()
	);

	m_pRenderTarget2D.Get()->DrawTextW(
		m_pModelsText.c_str(),
		(UINT32)m_pModelsText.length(),
		m_pTextFormat.Get(),
		m_pModelsTextBox,
		m_pBrush.Get()
	);

	m_pRenderTarget2D.Get()->DrawTextW(
		m_pDeveloperText.c_str(),
		(UINT32)m_pDeveloperText.length(),
		m_pTextFormat.Get(),
		m_pDeveloperTextBox,
		m_pBrush.Get()
	);
}

void CreditsUI::RenderBack() {
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
		m_pButtonFormat.Get(),
		m_pBackTextBox,
		m_pBrush.Get()
	);
}

void CreditsUI::Render() {
	BeginFrame();

	RenderScreen();
	RenderTitle();
	RenderText();
	RenderBack();

	EndFrame();
}

void CreditsUI::OnEvent(IEvent& event) noexcept {
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
				ToggleCredits tc;
				EventBuss::Get().Delegate(tc);
			}
		}
		break;
	}
	default:
		break;
	}
}

void CreditsUI::CleanUp() {
    //Will not need any clean up
}
