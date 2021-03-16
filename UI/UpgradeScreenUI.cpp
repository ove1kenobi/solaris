#include "..\pch.h"
#include "UpgradeScreenUI.h"

UpgradeScreenUI::UpgradeScreenUI() noexcept {
	EventBuss::Get().AddListener(this, EventType::MouseButtonEvent);

	m_pScreen = D2D1::RectF();
	m_pObjectiveTextBox = D2D1::RectF();
	m_pShipDisplay = D2D1::RectF();
	m_pResourceDisplay = D2D1::RectF();

	m_pDisplayPadding = 20.0f;

	m_pObjectiveText = L"Find a way to escape the solar system.";

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
	resources.push_back(L"Science.png");

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

bool UpgradeScreenUI::CreateShipDisplay() {
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

	//Here's where upgrade description and costs get's set
	//For now it's just examples to show it works 
	//Each cost has to added seperatly

	//TODO: create an enum(or find one) to figure out what upgrade goes to what ID
	m_pUpgrades.at(0)->SetUpgrade(L"Warp drive!", 0);
	m_pUpgrades.at(0)->AddCost(L"Fuel.png", L"10");
	m_pUpgrades.at(0)->AddCost(L"Science.png", L"30");

	m_pUpgrades.at(1)->SetUpgrade(L"Cold upgrade!", 1);
	m_pUpgrades.at(1)->AddCost(L"Science.png", L"10");
	m_pUpgrades.at(1)->AddCost(L"Health.png", L"20");

	m_pUpgrades.at(2)->SetUpgrade(L"Warm upgrade!", 2);
	m_pUpgrades.at(2)->AddCost(L"Health.png", L"20");
	m_pUpgrades.at(2)->AddCost(L"Health.png", L"20");
	m_pUpgrades.at(2)->AddCost(L"Health.png", L"20");
	m_pUpgrades.at(2)->AddCost(L"Health.png", L"20");
	m_pUpgrades.at(2)->AddCost(L"Health.png", L"20");
	m_pUpgrades.at(2)->AddCost(L"Health.png", L"20");
	m_pUpgrades.at(2)->AddCost(L"Health.png", L"20");

	m_pUpgrades.at(3)->SetUpgrade(L"Radioactive upgrade!", 3);
	m_pUpgrades.at(3)->AddCost(L"Health.png", L"20");

	m_pUpgrades.at(4)->SetUpgrade(L"Living quarters!", 4);
	m_pUpgrades.at(4)->AddCost(L"Health.png", L"20");

	m_pUpgrades.at(5)->SetUpgrade(L"Extra space!", 5);
	m_pUpgrades.at(5)->AddCost(L"Health.png", L"20");

	m_pUpgrades.at(6)->SetUpgrade(L"Asteroid protection!", 6);
	m_pUpgrades.at(6)->AddCost(L"Health.png", L"20");

	m_pUpgrades.at(7)->SetUpgrade(L"Asteroid protection!", 7);

	m_pUpgrades.at(8)->SetUpgrade(L"Asteroid protection!", 8);

	m_pUpgrades.at(9)->SetUpgrade(L"Asteroid protection!", 9);
	return true;
}

bool UpgradeScreenUI::UpdateShipDisplay() {
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

void UpgradeScreenUI::RenderShipDisplay() {
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
	default:
		break;
	}
}

void UpgradeScreenUI::CleanUp() {
    //Will need bitmap clean up later
}
