#include "..\pch.h"
#include "UpgradeScreenUI.h"

UpgradeScreenUI::UpgradeScreenUI() noexcept {
	m_pObjectiveText = L"Find a way to escape the solar system.";

	//Create upgrade objects
	for (unsigned int i = 0; i < 6; i++) {
		m_pUpgrades.push_back(new UpgradeUI());
	}
}

UpgradeScreenUI::~UpgradeScreenUI() {
	//Release memory
	for (unsigned int i = 0; i < m_pUpgrades.size(); i++) {
		delete m_pUpgrades.at(i);
	}
}

bool UpgradeScreenUI::Initialize() {
	if (!CreateBrush()) {
		return false;
	}
    if (!CreateScreen()) {
        return false;
    }
	if (!CreateObjective()) {
		return false;
	}
	if (!CreateUpgrades()) {
		return false;
	}
	if (!CreateShipDisplay()) {
		return false;
	}
	if (!CreateResourceList()) {
		return false;
	}
    return true;
}

bool UpgradeScreenUI::CreateScreen() {
	ErrorCheck(m_pTextFactory->GetSystemFontCollection(&m_pFont, false), "GetSystemFont");

	ErrorCheck(m_pTextFactory->CreateTextFormat(
		L"Netron",
		m_pFont.Get(),
		DWRITE_FONT_WEIGHT_REGULAR,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		18.0f,
		L"en-us",
		&m_pFormat
	), "TextFormat");
    return true;
}

bool UpgradeScreenUI::CreateObjective() {

	return true;
}

bool UpgradeScreenUI::CreateUpgrades() {
	for (unsigned int i = 0; i < m_pUpgrades.size(); i++) {
		if (!m_pUpgrades.at(i)->Initialize()) {
			return false;
		};
	}
	return true;
}

bool UpgradeScreenUI::CreateShipDisplay() {
	return true;
}

bool UpgradeScreenUI::CreateResourceList() {
	return true;
}

//Update functions
bool UpgradeScreenUI::UpdateScreen() {
    m_pScreen = D2D1::RectF(
        0.0f,
        0.0f,
        m_pWindowWidth,
        m_pWindowHeight
    );
    return true;
}

bool UpgradeScreenUI::UpdateObjective() {
	m_pObjectiveTextBox = D2D1::RectF(
		(m_pWindowWidth / 2.0f),
		0.0f,
		m_pWindowWidth,
		200.0f
	);
	return true;
}

bool UpgradeScreenUI::UpdateUpgrades() {
	//ScreenHeight - padding*2 - blockPadding*(m_pUpgrades.size() - 1) = blockSize
	//padding, BLOCK1, blockPadding1, BLOCK2, blockPadding2, BLOCK3, blockPadding3, BLOCK4, blockPadding4, BLOCK5, blockPadding5, BLOCK6, padding
	float ScreenPadding = 10.0f;
	float blockPadding = 5.0f;
	float blockSize = (m_pWindowHeight - ScreenPadding * 2 - blockPadding * (m_pUpgrades.size() - 1));
	

	//Upgrade 0
	m_pUpgrades.at(0)->SetHoverBox(D2D1::RectF(
		ScreenPadding,
		ScreenPadding,
		(m_pWindowWidth / 2.0f) - ScreenPadding,
		ScreenPadding + blockSize
	));

	//Upgrade 1
	m_pUpgrades.at(1)->SetHoverBox(D2D1::RectF(
		0.0f,
		0.0f,
		(m_pWindowWidth / 2.0f) - 50.0f,
		150.0f
	));

	//Upgrade 2
	/*
	m_pUpgrades.at(2)->SetHoverBox(D2D1::RectF(
		0.0f,
		0.0f,
		(m_pWindowWidth / 2.0f) - 50.0f,
		150.0f
	));

	//Upgrade 3
	m_pUpgrades.at(3)->SetHoverBox(D2D1::RectF(
		0.0f,
		0.0f,
		(m_pWindowWidth / 2.0f) - 50.0f,
		150.0f
	));

	//Upgrade 4
	m_pUpgrades.at(4)->SetHoverBox(D2D1::RectF(
		0.0f,
		0.0f,
		(m_pWindowWidth / 2.0f) - 50.0f,
		150.0f
	));

	//Upgrade 5
	m_pUpgrades.at(5)->SetHoverBox(D2D1::RectF(
		0.0f,
		0.0f,
		(m_pWindowWidth / 2.0f) - 50.0f,
		150.0f
	));
	*/
	return true;
}

bool UpgradeScreenUI::UpdateShipDisplay() {
	return true;
}

bool UpgradeScreenUI::UpdateResourceList() {
	return true;
}

bool UpgradeScreenUI::UpdateModules() {
    if (!UpdateScreen()) {
        return false;
    }
	if (!UpdateObjective()) {
		return false;
	}
	if (!UpdateUpgrades()) {
		return false;
	}
	if (!UpdateShipDisplay()) {
		return false;
	}
	if (!UpdateResourceList()) {
		return false;
	}
    return true;
}

void UpgradeScreenUI::RenderScreen() {
	this->UpdateBrush(D2D1::ColorF::MediumPurple, 0.5f);
	m_pRenderTarget2D->FillRectangle(m_pScreen, m_pBrush.Get());
}

void UpgradeScreenUI::RenderObjective() {
	this->UpdateBrush(D2D1::ColorF::MediumPurple, 0.5f);
	m_pRenderTarget2D->FillRectangle(m_pObjectiveTextBox, m_pBrush.Get());

	this->UpdateBrush(D2D1::ColorF::Snow, 1.0f);
	m_pRenderTarget2D.Get()->DrawTextW(
		m_pObjectiveText.c_str(),
		(UINT32)m_pObjectiveText.length(),
		m_pFormat.Get(),
		m_pObjectiveTextBox,
		m_pBrush.Get()
	);
}

void UpgradeScreenUI::RenderUpgrades() {
	for (unsigned int i = 0; i < m_pUpgrades.size(); i++) {
		m_pUpgrades.at(i)->Render();
	}
}

void UpgradeScreenUI::RenderShipDisplay() {

}

void UpgradeScreenUI::RenderResourceList() {

}

void UpgradeScreenUI::Render() {
	BeginFrame();

	RenderScreen();
	RenderObjective();
	RenderUpgrades();
	RenderShipDisplay();
	RenderResourceList();

	EndFrame();
}

void UpgradeScreenUI::OnEvent(IEvent& event) noexcept {
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
	default:
		break;
	}
}

void UpgradeScreenUI::CleanUp() {
    //Will need bitmap clean up later
}
