#include "..\pch.h"
#include "UpgradeScreenUI.h"

UpgradeScreenUI::UpgradeScreenUI() noexcept {
	EventBuss::Get().AddListener(this, EventType::MouseButtonEvent);
	EventBuss::Get().AddListener(this, EventType::WindowCloseEvent);

	m_pScreen = D2D1::RectF();
	m_pObjectiveTextBox = D2D1::RectF();
	m_pShipDisplay = D2D1::RectF();
	m_pResourceDisplay = D2D1::RectF();

	m_pDisplayPadding = 20.0f;

	m_pObjectiveText = L"Find a way to escape the solar system.";
	m_pRequirement = L"0";

	m_pRenderBitmaps = true;

	//Create upgrade objects
	for (unsigned int i = 0; i < 10; i++) {
		m_pUpgrades.push_back(new UpgradeUI());
	}

	//List pictures we will need
	resources.push_back(L"Khionerite.png");
	resources.push_back(L"Nanotech.png");
	resources.push_back(L"Plasma.png");
	resources.push_back(L"Radium.png");
	resources.push_back(L"Scrap.png");
	resources.push_back(L"Titanium.png");

	m_pYellow = 0xFFB724;
	m_pWhite = 0xFFFDF9;
	m_pCyan = 0x00ffff;
	m_pGray = 0x393b3d;
	m_pLightGray = 0x636363;
	m_pLightBlue = 0x0BA4CC;
	m_pDarkblue = 0x2741b4;
}

UpgradeScreenUI::~UpgradeScreenUI() {
	EventBuss::Get().RemoveListener(this, EventType::MouseButtonEvent);
	EventBuss::Get().RemoveListener(this, EventType::WindowCloseEvent);

	//Release memory
	for (unsigned int i = 0; i < m_pUpgrades.size(); i++) {
		delete m_pUpgrades.at(i);
	}

	for (auto const& bitmap : m_pResourceBitmap) {
		bitmap->Release();
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
	if (!CreateControllerDisplay()) {
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
		L"Tenika",
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

bool UpgradeScreenUI::CreateControllerDisplay() {
	//For now we do not need it, but later once I get to design
	//LoadBitmapFromFile(GetIconFilePath(L"Health.png").c_str(), &m_pShipBitmap);
	return true;
}

bool UpgradeScreenUI::CreateResourceList() {
	ID2D1Bitmap* holder = NULL;

	//Create resource bitmaps
	for (unsigned int i = 0; i < resources.size(); i++) {
		LoadBitmapFromFile(GetIconFilePath(resources.at(i)).c_str(), &holder);
		m_pResourceBitmap.push_back(holder);
	}

	//Create format
	ErrorCheck(m_pTextFactory->CreateTextFormat(
		L"Tenika",
		m_pFont.Get(),
		DWRITE_FONT_WEIGHT_REGULAR,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		20.0f,
		L"en-us",
		&m_pScienceFormat
	), "TextFormat");

	//Create science bitmap
	LoadBitmapFromFile(GetIconFilePath(L"Science.png").c_str(), &m_pScienceBitmap);
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
		(m_pWindowWidth / 2.0f) + m_pDisplayPadding,
		0.0f + m_pDisplayPadding,
		m_pWindowWidth - m_pDisplayPadding,
		200.0f - m_pDisplayPadding
	);
	return true;
}

bool UpgradeScreenUI::UpdateUpgrades() {
	float blockPadding = 10.0f;
	float blockSize = (m_pWindowHeight - m_pDisplayPadding * 2 - blockPadding * (m_pUpgrades.size() - 1))/ m_pUpgrades.size();
	
	for (unsigned int i = 0; i < m_pUpgrades.size(); i++) {
		m_pUpgrades.at(i)->SetHoverBox(D2D1::RectF(
			m_pDisplayPadding,
			m_pDisplayPadding + (blockSize + blockPadding)*i,
			(m_pWindowWidth / 2.0f) - m_pDisplayPadding,
			m_pDisplayPadding + blockSize + (blockSize + blockPadding)*i
		));
	}

	
	//Afterburner
	m_pUpgrades.at(0)->SetUpgrade(L"AfterBurner",L"Raises your ships maximum speed by 40%.", 0);
	m_pUpgrades.at(0)->AddCost(L"Scrap.png", L"10");
	m_pUpgrades.at(0)->AddCost(L"Titanium.png", L"10");
	m_pUpgrades.at(0)->AddCost(L"Nanotech.png", L"10");
	m_pUpgrades.at(0)->SetScience(1);

	//Antenna
	m_pUpgrades.at(1)->SetUpgrade(L"Antenna", L"Allows you to see a planet’s name from any distance.", 1);
	m_pUpgrades.at(1)->AddCost(L"Titanium.png", L"10");
	m_pUpgrades.at(1)->AddCost(L"Nanotech.png", L"10");
	m_pUpgrades.at(1)->AddCost(L"Scrap.png", L"10");
	m_pUpgrades.at(1)->SetScience(1);

	//Cargo
	m_pUpgrades.at(2)->SetUpgrade(L"Cargo", L"Doubles your max-weight, allowing you to carry more materials", 2);
	m_pUpgrades.at(2)->AddCost(L"Titanium.png", L"10");
	m_pUpgrades.at(2)->AddCost(L"Nanotech.png", L"10");
	m_pUpgrades.at(2)->AddCost(L"Scrap.png", L"10");
	m_pUpgrades.at(2)->SetScience(1);

	//Cold shield
	m_pUpgrades.at(3)->SetUpgrade(L"Cold shield", L"Allows the ship to travel to the colder parts of the solar system without freezing.", 3);
	m_pUpgrades.at(3)->AddCost(L"Titanium.png", L"20");
	m_pUpgrades.at(3)->AddCost(L"Nanotech.png", L"20");
	m_pUpgrades.at(3)->AddCost(L"Scrap.png", L"20");
	m_pUpgrades.at(3)->SetScience(5);

	//Warm shield
	m_pUpgrades.at(4)->SetUpgrade(L"Warm shield", L"Allows the ship to travel very close to the sun, without succumbing to the heat.", 7);
	m_pUpgrades.at(4)->AddCost(L"Titanium.png", L"20");
	m_pUpgrades.at(4)->AddCost(L"Nanotech.png", L"20");
	m_pUpgrades.at(4)->AddCost(L"Scrap.png", L"20");
	m_pUpgrades.at(4)->SetScience(5);

	//Radioactive shield
	m_pUpgrades.at(5)->SetUpgrade(L"Radioactive shield", L"Allows the ship to approach radioactive planets without being exposed to the radioactivity.", 9);
	m_pUpgrades.at(5)->AddCost(L"Titanium.png", L"20");
	m_pUpgrades.at(5)->AddCost(L"Nanotech.png", L"20");
	m_pUpgrades.at(5)->AddCost(L"Scrap.png", L"20");
	m_pUpgrades.at(5)->SetScience(5);

	//Fuel cells
	m_pUpgrades.at(6)->SetUpgrade(L"Fuel cells", L"Increases max fuel by 100%, also gives fuel equal to the max fuel before this upgrade.", 4);
	m_pUpgrades.at(6)->AddCost(L"Titanium.png", L"50");
	m_pUpgrades.at(6)->AddCost(L"Nanotech.png", L"5");
	m_pUpgrades.at(6)->AddCost(L"Scrap.png", L"5");
	m_pUpgrades.at(6)->SetScience(8);

	//Living quarters
	m_pUpgrades.at(7)->SetUpgrade(L"Living quarters", L"Increases max Oxygen by 100%, also gives Oxygen equal to the max oxygen before this upgrade.", 5);
	m_pUpgrades.at(7)->AddCost(L"Titanium.png", L"5");
	m_pUpgrades.at(7)->AddCost(L"Nanotech.png", L"50");
	m_pUpgrades.at(7)->AddCost(L"Scrap.png", L"5");
	m_pUpgrades.at(7)->SetScience(8);

	//Shield
	m_pUpgrades.at(8)->SetUpgrade(L"Shield", L"Increases max Health by 100%, also restores Health equal to the max health before this upgrade. Also takes half damage from asteroids.", 6);
	m_pUpgrades.at(8)->AddCost(L"Titanium.png", L"5");
	m_pUpgrades.at(8)->AddCost(L"Nanotech.png", L"5");
	m_pUpgrades.at(8)->AddCost(L"Scrap.png", L"50");
	m_pUpgrades.at(8)->SetScience(8);

	//Warpdrive
	m_pUpgrades.at(9)->SetUpgrade(L"Warpdrive", L"Allows for interstellar travel, giving you an opportunity to escape from the solar system.", 8);
	m_pUpgrades.at(9)->AddCost(L"Plasma.png", L"1");
	m_pUpgrades.at(9)->AddCost(L"Khionerite.png", L"1");
	m_pUpgrades.at(9)->AddCost(L"Radium.png", L"1");
	m_pUpgrades.at(9)->SetScience(12);
	return true;
}

bool UpgradeScreenUI::UpdateControllerDisplay() {
	m_pShipDisplay = D2D1::RectF(
		(m_pWindowWidth / 2.0f) + m_pDisplayPadding,
		m_pObjectiveTextBox.bottom + m_pDisplayPadding,
		m_pWindowWidth - m_pDisplayPadding,
		m_pObjectiveTextBox.bottom + 600.0f - m_pDisplayPadding
	);
	return true;
}

bool UpgradeScreenUI::UpdateResourceList() {
	float iconSize = 50.0f;
	float amountSize = 50.0f;
	float padding = 10.0f;

	m_pResourceDisplay = D2D1::RectF(
		(m_pWindowWidth / 2.0f) + m_pDisplayPadding,
		m_pShipDisplay.bottom + m_pDisplayPadding,
		m_pWindowWidth - m_pDisplayPadding,
		m_pWindowHeight - m_pDisplayPadding
	);

	for (unsigned int i = 0; i < resources.size(); i++) {
		float x = static_cast<float>(m_pResourcePosition.size() % 2);
		float y = floor(static_cast<float>(m_pResourcePosition.size()) / 2.0f);

		//Create square for picture
		m_pResourcePosition.push_back(D2D1::RectF(
			m_pResourceDisplay.left + (iconSize + iconSize + padding) * x,
			m_pResourceDisplay.top + (padding + iconSize) * y,
			m_pResourceDisplay.left + iconSize + (iconSize + iconSize + padding) * x,
			m_pResourceDisplay.top + iconSize + (padding + iconSize) * y
		));

		//Create square for text
		m_pAmountTextbox.push_back(D2D1::RectF(
			m_pResourcePosition.at(m_pResourcePosition.size() - 1).right + 5.0f,
			m_pResourcePosition.at(m_pResourcePosition.size() - 1).top,
			m_pResourcePosition.at(m_pResourcePosition.size() - 1).right + iconSize + 5.0f,
			m_pResourcePosition.at(m_pResourcePosition.size() - 1).bottom
		));

		//Add text
		m_pAmount.push_back(L"0");
	}

	//Create science icon
	float offset = 50.0f;
	float scienceSize = 125.0f;
	m_pSciencePosition = D2D1::RectF(
		m_pResourceDisplay.right - scienceSize - offset,
		((m_pResourceDisplay.bottom - m_pResourceDisplay.top) / 2.0f) + m_pResourceDisplay.top - (scienceSize/2.0f),
		m_pResourceDisplay.right - offset,
		((m_pResourceDisplay.bottom - m_pResourceDisplay.top) / 2.0f) + m_pResourceDisplay.top + (scienceSize / 2.0f)
	);

	//Create text box
	m_pRequirementTextbox = D2D1::RectF(
		m_pSciencePosition.left,
		m_pSciencePosition.top,
		m_pSciencePosition.right,
		m_pSciencePosition.bottom
	);
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
	if (!UpdateControllerDisplay()) {
		return false;
	}
	if (!UpdateResourceList()) {
		return false;
	}
    return true;
}

void UpgradeScreenUI::RenderScreen() {
	this->UpdateBrush(m_pGray, 1.0f);
	m_pRenderTarget2D->FillRectangle(m_pScreen, m_pBrush.Get());
}

void UpgradeScreenUI::RenderObjective() {
	this->UpdateBrush(m_pDarkblue, 0.5f);
	m_pRenderTarget2D->FillRectangle(m_pObjectiveTextBox, m_pBrush.Get());

	this->UpdateBrush(m_pWhite, 1.0f);
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

void UpgradeScreenUI::RenderControllerDisplay() {
	this->UpdateBrush(m_pDarkblue, 0.5f);
	m_pRenderTarget2D->FillRectangle(m_pShipDisplay, m_pBrush.Get());
}

void UpgradeScreenUI::RenderResourceList() {
	this->UpdateBrush(m_pDarkblue, 0.5f);
	m_pRenderTarget2D->FillRectangle(m_pResourceDisplay, m_pBrush.Get());

	unsigned int i = 0;
	for (auto const& bitmap : m_pResourceBitmap) {
		m_pRenderTarget2D->DrawBitmap(bitmap, m_pResourcePosition.at(i));
		this->UpdateBrush(m_pWhite, 1.0f);
		m_pRenderTarget2D.Get()->DrawTextW(
			m_pAmount.at(i).c_str(),
			(UINT32)m_pAmount.at(i).length(),
			m_pFormat.Get(),
			m_pAmountTextbox.at(i),
			m_pBrush.Get()
		);
		i++;
	}

	if (m_pRenderBitmaps) {
		m_pRenderTarget2D->DrawBitmap(m_pScienceBitmap, m_pSciencePosition);
	}

	this->UpdateBrush(D2D1::ColorF::Snow, 1.0f);
	m_pRenderTarget2D.Get()->DrawTextW(
		m_pRequirement.c_str(),
		(UINT32)m_pRequirement.length(),
		m_pScienceFormat.Get(),
		m_pRequirementTextbox,
		m_pBrush.Get()
	);
}

void UpgradeScreenUI::Render() {
	BeginFrame();

	RenderScreen();
	RenderObjective();
	RenderUpgrades();
	RenderControllerDisplay();
	RenderResourceList();

	EndFrame();
}

void UpgradeScreenUI::UpdateAmount() {

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
	//For clicking on UI elements
	case EventType::MouseButtonEvent:
	{
		m_pMouseX = static_cast<MouseButtonEvent*>(&event)->GetXCoord();
		m_pMouseY = static_cast<MouseButtonEvent*>(&event)->GetYCoord();
		KeyState state = static_cast<MouseButtonEvent*>(&event)->GetKeyState();
		int virKey = static_cast<MouseButtonEvent*>(&event)->GetVirtualKeyCode();
		if (virKey == VK_LBUTTON && state == KeyState::KeyPress && m_pOnScreen) {
			for (unsigned int i = 0; i < m_pUpgrades.size(); i++) {
				m_pUpgrades.at(i)->OnClick(m_pMouseX, m_pMouseY);
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

void UpgradeScreenUI::CleanUp() {
	m_pRenderBitmaps = false;
	if (m_pScienceBitmap) {
		m_pScienceBitmap->Release();
	}
	for (unsigned int i = 0; i < m_pUpgrades.size(); i++) {
		m_pUpgrades.at(i)->CleanUp();
	}
}
