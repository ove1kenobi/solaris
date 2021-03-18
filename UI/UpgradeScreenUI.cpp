#include "..\pch.h"
#include "UpgradeScreenUI.h"

UpgradeScreenUI::UpgradeScreenUI() noexcept {
	EventBuss::Get().AddListener(this, EventType::DelegatePlayerInfoEvent);
	EventBuss::Get().AddListener(this, EventType::MouseButtonEvent);
	EventBuss::Get().AddListener(this, EventType::WindowCloseEvent);

	m_pScreen = D2D1::RectF();
	m_pObjectiveDisplayBox = D2D1::RectF();
	m_pControllerDisplay = D2D1::RectF();
	m_pResourceDisplay = D2D1::RectF();

	m_pDisplayPadding = 20.0f;

	m_pObjectiveTitle = L"OBJECTIVE";
	m_pControllerTitle = L"CONTROLS";
	m_pInventoryTitle = L"INVENTORY";
	m_pObjectiveText = L"Find a way to escape the solar system...";
	m_pRequirement = L"0";

	m_pButtonTextE = L"E";
	m_pButtonTextDescriptionE = L"Send probe down to planet.";
	m_pButtonTextQ = L"Q";
	m_pButtonTextDescriptionQ = L"Toggle ship stabilizer.";
	m_pButtonTextW = L"W";
	m_pButtonTextDescriptionW = L"Accelerate forward.";
	m_pButtonTextS = L"S";
	m_pButtonTextDescriptionS = L"Accelerate backwards";
	m_pButtonTextU = L"U";
	m_pButtonTextDescriptionU = L"Toggle upgrade menu.";

	m_pButtonTextDescriptionLeft = L"Left click to select events.";
	m_pMouseTextDescription = L"Use mouse to move.";

	m_pRenderBitmaps = true;

	//Create upgrade objects
	for (unsigned int i = 0; i < 10; i++) {
		m_pUpgrades.push_back(new UpgradeUI());
	}

	//List pictures we will need
	resources.push_back(L"Khionerite");
	resources.push_back(L"Nanotech");
	resources.push_back(L"Plasma");
	resources.push_back(L"Radium");
	resources.push_back(L"Scrap");
	resources.push_back(L"Titanium");

	m_pYellow = 0xFFB724;
	m_pWhite = 0xFFFDF9;
	m_pCyan = 0x00ffff;
	m_pGray = 0x393b3d;
	m_pLightGray = 0x636363;
	m_pLightBlue = 0x0BA4CC;
	m_pDarkblue = 0x2741b4;
}

UpgradeScreenUI::~UpgradeScreenUI() {
	EventBuss::Get().RemoveListener(this, EventType::DelegatePlayerInfoEvent);
	EventBuss::Get().RemoveListener(this, EventType::WindowCloseEvent);
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
	if (!CreateControllerDisplay()) {
		return false;
	}
	if (!CreateResourceList()) {
		return false;
	}
	if (!CreateTools()) {
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
		20.0f,
		L"en-us",
		&m_pFormat
	), "TextFormat");

	ErrorCheck(m_pTextFactory->CreateTextFormat(
		L"Aware",
		m_pFont.Get(),
		DWRITE_FONT_WEIGHT_REGULAR,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		36.0f,
		L"en-us",
		&m_pTitleFormat
	), "TextFormat");
    return true;
}

bool UpgradeScreenUI::CreateObjective() {
	ErrorCheck(m_pTextFactory->CreateTextFormat(
		L"Tenika",
		m_pFont.Get(),
		DWRITE_FONT_WEIGHT_REGULAR,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		30.0f,
		L"en-us",
		&m_pObjectiveFormat
	), "TextFormat");
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
	LoadBitmapFromFile(GetIconFilePath(L"Mouse.png").c_str(), &m_pMouseBitmap);
	return true;
}

bool UpgradeScreenUI::CreateResourceList() {
	ID2D1Bitmap* holder = NULL;

	//Create resource bitmaps
	for (unsigned int i = 0; i < resources.size(); i++) {
		LoadBitmapFromFile(GetIconFilePath((resources.at(i)) + L".png").c_str(), &holder);
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

bool UpgradeScreenUI::CreateTools() {
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
			D2D1::Point2F(m_pControllerDisplay.left, m_pWindowHeight),
			D2D1::Point2F(m_pControllerDisplay.right, m_pWindowHeight)),
		m_pMainGradientStops.Get(),
		&m_pMainGradientBrush), "GradientBrush");

	//Corner gradient
	gradientStops[0].color = D2D1::ColorF(m_pLightGray, 0.25f);
	gradientStops[0].position = 0.0f;
	gradientStops[1].color = D2D1::ColorF(m_pGray, 0.75f);
	gradientStops[1].position = 0.5f;
	gradientStops[2].color = D2D1::ColorF(m_pLightGray, 0.25f);
	gradientStops[2].position = 1.0f;

	ErrorCheck(m_pRenderTarget2D->CreateGradientStopCollection(
		gradientStops,
		3,
		D2D1_GAMMA_1_0,
		D2D1_EXTEND_MODE_CLAMP,
		&m_pCornerGradientStops
	), "GradientStops");

	ErrorCheck(m_pRenderTarget2D->CreateLinearGradientBrush(
		D2D1::LinearGradientBrushProperties(
			D2D1::Point2F(0.0f, m_pWindowHeight),
			D2D1::Point2F(0.0f, m_pWindowHeight)),
		m_pCornerGradientStops.Get(),
		&m_pCornerGradientBrush), "GradientBrush");
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
	m_pObjectiveDisplayBox = D2D1::RectF(
		(m_pWindowWidth / 2.0f) + m_pDisplayPadding,
		0.0f + m_pDisplayPadding,
		m_pWindowWidth - m_pDisplayPadding,
		200.0f - m_pDisplayPadding
	);

	float padding = 20.0f;
	m_pObjectiveTextBox = D2D1::RectF(
		m_pObjectiveDisplayBox.left + padding,
		m_pObjectiveDisplayBox.top + padding + 10.0f,
		m_pObjectiveDisplayBox.right - padding,
		m_pObjectiveDisplayBox.bottom - padding
	);

	m_pObjectiveTitleBox = D2D1::RectF(
		m_pObjectiveDisplayBox.left + 20.0f,
		m_pObjectiveDisplayBox.top - 20.0f,
		m_pObjectiveDisplayBox.right,
		m_pObjectiveDisplayBox.top + 20.0f
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
	m_pUpgrades.at(0)->SetUpgrade(L"AFTERBURNER",L"Raises your ships maximum speed by 40%.", 0);
	m_pUpgrades.at(0)->AddCost(L"Scrap.png", L"10");
	m_pUpgrades.at(0)->AddCost(L"Titanium.png", L"10");
	m_pUpgrades.at(0)->AddCost(L"Nanotech.png", L"10");
	m_pUpgrades.at(0)->SetScience(1);

	//Antenna
	m_pUpgrades.at(1)->SetUpgrade(L"ANTENNA", L"Allows you to see a planet’s name from any distance.", 1);
	m_pUpgrades.at(1)->AddCost(L"Titanium.png", L"10");
	m_pUpgrades.at(1)->AddCost(L"Nanotech.png", L"10");
	m_pUpgrades.at(1)->AddCost(L"Scrap.png", L"10");
	m_pUpgrades.at(1)->SetScience(1);

	//Cargo
	m_pUpgrades.at(2)->SetUpgrade(L"CARGO", L"Doubles your max-weight, allowing you to carry more materials.", 2);
	m_pUpgrades.at(2)->AddCost(L"Titanium.png", L"10");
	m_pUpgrades.at(2)->AddCost(L"Nanotech.png", L"10");
	m_pUpgrades.at(2)->AddCost(L"Scrap.png", L"10");
	m_pUpgrades.at(2)->SetScience(1);

	//Cold shield
	m_pUpgrades.at(3)->SetUpgrade(L"COLD SHIELD", L"Allows the ship to travel to the colder parts of the solar system without freezing.", 3);
	m_pUpgrades.at(3)->AddCost(L"Titanium.png", L"20");
	m_pUpgrades.at(3)->AddCost(L"Nanotech.png", L"20");
	m_pUpgrades.at(3)->AddCost(L"Scrap.png", L"20");
	m_pUpgrades.at(3)->SetScience(5);

	//Warm shield
	m_pUpgrades.at(4)->SetUpgrade(L"WARM SHIELD", L"Allows the ship to travel very close to the sun, without succumbing to the heat.", 7);
	m_pUpgrades.at(4)->AddCost(L"Titanium.png", L"20");
	m_pUpgrades.at(4)->AddCost(L"Nanotech.png", L"20");
	m_pUpgrades.at(4)->AddCost(L"Scrap.png", L"20");
	m_pUpgrades.at(4)->SetScience(5);

	//Radioactive shield
	m_pUpgrades.at(5)->SetUpgrade(L"RADIOACTIVE SHIELD", L"Allows the ship to approach radioactive planets without being exposed to the radioactivity.", 9);
	m_pUpgrades.at(5)->AddCost(L"Titanium.png", L"20");
	m_pUpgrades.at(5)->AddCost(L"Nanotech.png", L"20");
	m_pUpgrades.at(5)->AddCost(L"Scrap.png", L"20");
	m_pUpgrades.at(5)->SetScience(5);

	//Fuel cells
	m_pUpgrades.at(6)->SetUpgrade(L"FUEL CELLS", L"Increases max fuel by 100%, also gives fuel equal to the max fuel before this upgrade.", 4);
	m_pUpgrades.at(6)->AddCost(L"Titanium.png", L"50");
	m_pUpgrades.at(6)->AddCost(L"Nanotech.png", L"5");
	m_pUpgrades.at(6)->AddCost(L"Scrap.png", L"5");
	m_pUpgrades.at(6)->SetScience(8);

	//Living quarters
	m_pUpgrades.at(7)->SetUpgrade(L"LIVING QUARTERS", L"Increases max Oxygen by 100%, also gives Oxygen equal to the max oxygen before this upgrade.", 5);
	m_pUpgrades.at(7)->AddCost(L"Titanium.png", L"5");
	m_pUpgrades.at(7)->AddCost(L"Nanotech.png", L"50");
	m_pUpgrades.at(7)->AddCost(L"Scrap.png", L"5");
	m_pUpgrades.at(7)->SetScience(8);

	//Shield
	m_pUpgrades.at(8)->SetUpgrade(L"SHIELD", L"Increases max Health by 100%, also restores Health equal to the max health before this upgrade. Also takes half damage from asteroids.", 6);
	m_pUpgrades.at(8)->AddCost(L"Titanium.png", L"5");
	m_pUpgrades.at(8)->AddCost(L"Nanotech.png", L"5");
	m_pUpgrades.at(8)->AddCost(L"Scrap.png", L"50");
	m_pUpgrades.at(8)->SetScience(8);

	//Warpdrive
	m_pUpgrades.at(9)->SetUpgrade(L"WARPDRIVE", L"Allows for interstellar travel, giving you an opportunity to escape from the solar system.", 8);
	m_pUpgrades.at(9)->AddCost(L"Plasma.png", L"1");
	m_pUpgrades.at(9)->AddCost(L"Khionerite.png", L"1");
	m_pUpgrades.at(9)->AddCost(L"Radium.png", L"1");
	m_pUpgrades.at(9)->SetScience(12);
	return true;
}

bool UpgradeScreenUI::UpdateControllerDisplay() {
	//Screen and title
	m_pControllerDisplay = D2D1::RectF(
		(m_pWindowWidth / 2.0f) + m_pDisplayPadding,
		m_pObjectiveDisplayBox.bottom + m_pDisplayPadding,
		m_pWindowWidth - m_pDisplayPadding,
		m_pObjectiveDisplayBox.bottom + 600.0f - m_pDisplayPadding
	);

	m_pControllerTitleBox = D2D1::RectF(
		m_pControllerDisplay.left + 20.0f,
		m_pControllerDisplay.top - 20.0f,
		m_pControllerDisplay.right,
		m_pControllerDisplay.top + 20.0f
	);

	float offsetX = 30.0f;
	float offsetY = 40.0f;
	float buttonSize = 75.0f;
	float buttonDistance = 20.0f;
	//Button E
	m_pButtonE = D2D1::RoundedRect(
		D2D1::RectF(
			m_pControllerDisplay.left + offsetX,
			m_pControllerDisplay.top + offsetY,
			m_pControllerDisplay.left + buttonSize + offsetX,
			m_pControllerDisplay.top + buttonSize + offsetY
		),
		10.f,
		10.f
	);

	m_pButtonBoxE = D2D1::RectF(
		m_pControllerDisplay.left + offsetX,
		m_pControllerDisplay.top + offsetY + 20.0f,
		m_pControllerDisplay.left + buttonSize + offsetX,
		m_pControllerDisplay.top + buttonSize + offsetY
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
			m_pControllerDisplay.left + offsetX,
			m_pControllerDisplay.top + offsetY + buttonSize + buttonDistance,
			m_pControllerDisplay.left + buttonSize + offsetX,
			m_pControllerDisplay.top + buttonSize*2 + offsetY + buttonDistance
		),
		10.f,
		10.f
	);

	m_pButtonBoxQ = D2D1::RectF(
			m_pControllerDisplay.left + offsetX,
			m_pControllerDisplay.top + offsetY + buttonSize + buttonDistance + 20.0f,
			m_pControllerDisplay.left + buttonSize + offsetX,
			m_pControllerDisplay.top + buttonSize * 2 + offsetY + buttonDistance
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
			m_pControllerDisplay.left + offsetX,
			m_pControllerDisplay.top + offsetY + buttonSize * 2.0f + buttonDistance * 2.0f,
			m_pControllerDisplay.left + buttonSize + offsetX,
			m_pControllerDisplay.top + buttonSize * 3.0f + offsetY + buttonDistance * 2.0f
		),
		10.f,
		10.f
	);

	m_pButtonBoxU = D2D1::RectF(
		m_pControllerDisplay.left + offsetX,
		m_pControllerDisplay.top + offsetY + buttonSize * 2.0f + buttonDistance * 2.0f + 20.0f,
		m_pControllerDisplay.left + buttonSize + offsetX,
		m_pControllerDisplay.top + buttonSize * 3.0f + offsetY + buttonDistance * 2.0f
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
			m_pControllerDisplay.left + offsetX,
			m_pControllerDisplay.top + offsetY* 2.0f + buttonSize * 3.0f + buttonDistance * 3.0f,
			m_pControllerDisplay.left + buttonSize + offsetX,
			m_pControllerDisplay.top + buttonSize * 4.0f + offsetY* 2.0f + buttonDistance * 3.0f
		),
		10.f,
		10.f
	);

	m_pButtonBoxW = D2D1::RectF(
		m_pControllerDisplay.left + offsetX,
		m_pControllerDisplay.top + offsetY*2.0f + buttonSize * 3.0f + buttonDistance * 3.0f + 20.0f,
		m_pControllerDisplay.left + buttonSize + offsetX,
		m_pControllerDisplay.top + buttonSize * 4.0f + offsetY*2.0f + buttonDistance * 3.0f
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
			m_pControllerDisplay.left + offsetX,
			m_pControllerDisplay.top + offsetY* 2.0f + buttonSize * 4.0f + buttonDistance * 4.0f,
			m_pControllerDisplay.left + buttonSize + offsetX,
			m_pControllerDisplay.top + buttonSize * 5.0f + offsetY* 2.0f + buttonDistance * 4.0f
		),
		10.f,
		10.f
	);

	m_pButtonBoxS = D2D1::RectF(
		m_pControllerDisplay.left + offsetX,
		m_pControllerDisplay.top + offsetY* 2.0f + buttonSize * 4.0f + buttonDistance * 4.0f + 20.0f,
		m_pControllerDisplay.left + buttonSize + offsetX,
		m_pControllerDisplay.top + buttonSize * 5.0f + offsetY* 2.0f + buttonDistance * 4.0f
	);

	m_pButtonBoxDescriptionS = D2D1::RectF(
		m_pButtonBoxS.right + 10.0f,
		m_pButtonBoxS.top + 5.0f,
		m_pButtonBoxS.right + 400.0f,
		m_pButtonBoxS.bottom
	);

	//Mouse
	m_pMousePosition = D2D1::RectF(
		m_pControllerDisplay.right - offsetX,
		m_pControllerDisplay.top + offsetX,
		m_pControllerDisplay.right - 450.0f - offsetX,
		m_pControllerDisplay.top + 450.0f + offsetX
	);

	m_pButtonBoxDescriptionLeft = D2D1::RectF(
		m_pControllerDisplay.right - offsetX,
		m_pControllerDisplay.top + offsetX + 90.0f,
		m_pControllerDisplay.right - 380.0f - offsetX,
		m_pMousePosition.top + 110.0f
	);

	m_pMouseBoxDescription = D2D1::RectF(
		m_pControllerDisplay.right - offsetX,
		m_pMousePosition.bottom - 25.0f,
		m_pControllerDisplay.right - 355.0f - offsetX,
		m_pControllerDisplay.bottom - offsetX
	);
	return true;
}

bool UpgradeScreenUI::UpdateResourceList() {
	float iconSize = 70.0f;
	float amountSize = 50.0f;
	float padding = 20.0f;
	float offsetX = 65.0f;
	float offsetY = 50.0f;

	m_pResourceDisplay = D2D1::RectF(
		(m_pWindowWidth / 2.0f) + m_pDisplayPadding,
		m_pControllerDisplay.bottom + m_pDisplayPadding,
		m_pWindowWidth - m_pDisplayPadding,
		m_pWindowHeight - m_pDisplayPadding
	);

	m_pInventoryTitleBox = D2D1::RectF(
		m_pResourceDisplay.left + 20.0f,
		m_pResourceDisplay.top - 20.0f,
		m_pResourceDisplay.right,
		m_pResourceDisplay.top + 20.0f
	);

	for (unsigned int i = 0; i < resources.size(); i++) {
		float y = static_cast<float>(m_pResourcePosition.size() % 2);
		float x = floor(static_cast<float>(m_pResourcePosition.size()) / 2.0f);

		//Create square for picture
		m_pResourcePosition.push_back(D2D1::RectF(
			m_pResourceDisplay.left + offsetX +(iconSize + iconSize + padding + offsetX) * x,
			m_pResourceDisplay.top + offsetY +(padding + iconSize + 10.0f) * y,
			m_pResourceDisplay.left + offsetX + iconSize + (iconSize + iconSize + padding + offsetX) * x,
			m_pResourceDisplay.top + offsetY + iconSize + (padding + iconSize + 10.0f) * y
		));

		//Create square for text
		m_pAmountTextbox.push_back(D2D1::RectF(
			m_pResourcePosition.at(m_pResourcePosition.size() - 1).right + 5.0f,
			m_pResourcePosition.at(m_pResourcePosition.size() - 1).top + 20.0f,
			m_pResourcePosition.at(m_pResourcePosition.size() - 1).right + iconSize + 5.0f,
			m_pResourcePosition.at(m_pResourcePosition.size() - 1).bottom
		));

		m_pDescriptionTextbox.push_back(D2D1::RectF(
			m_pResourcePosition.at(m_pResourcePosition.size() - 1).left - 20.0f,
			m_pResourcePosition.at(m_pResourcePosition.size() - 1).top - 25.0f,
			m_pResourcePosition.at(m_pResourcePosition.size() - 1).right + 20.0f,
			m_pResourcePosition.at(m_pResourcePosition.size() - 1).top + 5.0f
		));

		//Add text
		m_pAmount.push_back(L"0");
	}

	//Create science icon
	float scienceOffset = 50.0f;
	float scienceSize = 125.0f;
	m_pSciencePosition = D2D1::RectF(
		m_pResourceDisplay.right - scienceSize - scienceOffset,
		((m_pResourceDisplay.bottom - m_pResourceDisplay.top) / 2.0f) + m_pResourceDisplay.top - (scienceSize/2.0f),
		m_pResourceDisplay.right - scienceOffset,
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

bool UpgradeScreenUI::UpdateTools() {
	m_pMainGradientBrush->SetStartPoint(D2D1::Point2F(m_pControllerDisplay.left, m_pWindowHeight));
	m_pMainGradientBrush->SetEndPoint(D2D1::Point2F(m_pControllerDisplay.right, m_pWindowHeight));
	m_pCornerGradientBrush->SetStartPoint(D2D1::Point2F(0.0f, m_pWindowHeight));
	m_pCornerGradientBrush->SetEndPoint(D2D1::Point2F(0.0f, m_pWindowHeight));
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
	if (!UpdateTools()) {
		return false;
	}
    return true;
}

void UpgradeScreenUI::RenderScreen() {
	this->UpdateBrush(m_pGray, 0.25f);
	m_pRenderTarget2D->FillRectangle(m_pScreen, m_pBrush.Get());
	m_pRenderTarget2D->FillRectangle(m_pScreen, m_pCornerGradientBrush.Get());
}

void UpgradeScreenUI::RenderObjective() {
	m_pObjectiveFormat.Get()->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
	m_pFormat.Get()->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
	this->UpdateBrush(m_pGray, 1.0f);
	m_pRenderTarget2D->FillRectangle(m_pObjectiveDisplayBox, m_pBrush.Get());
	this->UpdateBrush(m_pDarkblue, 0.5f);
	m_pRenderTarget2D->FillRectangle(m_pObjectiveDisplayBox, m_pBrush.Get());

	//Grid for the square
	unsigned int gridSize = 25;
	this->UpdateBrush(m_pCyan, 0.25f);
	for (float x = m_pObjectiveDisplayBox.left; x < m_pObjectiveDisplayBox.right; x += gridSize)
	{
		m_pRenderTarget2D->DrawLine(
			D2D1::Point2F(static_cast<FLOAT>(x), m_pObjectiveDisplayBox.top),
			D2D1::Point2F(static_cast<FLOAT>(x), m_pObjectiveDisplayBox.bottom),
			m_pBrush.Get(),
			0.5f
		);
	}

	for (float y = m_pObjectiveDisplayBox.top; y < m_pObjectiveDisplayBox.bottom; y += gridSize)
	{
		m_pRenderTarget2D->DrawLine(
			D2D1::Point2F(m_pObjectiveDisplayBox.left, static_cast<FLOAT>(y)),
			D2D1::Point2F(m_pObjectiveDisplayBox.right, static_cast<FLOAT>(y)),
			m_pBrush.Get(),
			0.5f
		);
	}

	m_pRenderTarget2D->FillRectangle(&m_pObjectiveDisplayBox, m_pMainGradientBrush.Get());

	UpdateBrush(m_pWhite, 0.5f);
	m_pRenderTarget2D->DrawRectangle(m_pObjectiveDisplayBox, m_pBrush.Get());

	this->UpdateBrush(m_pWhite, 1.0f);
	m_pRenderTarget2D.Get()->DrawTextW(
		m_pObjectiveText.c_str(),
		(UINT32)m_pObjectiveText.length(),
		m_pObjectiveFormat.Get(),
		m_pObjectiveTextBox,
		m_pBrush.Get()
	);

	this->UpdateBrush(m_pYellow, 1.0f);
	m_pRenderTarget2D.Get()->DrawTextW(
		m_pObjectiveTitle.c_str(),
		(UINT32)m_pObjectiveTitle.length(),
		m_pTitleFormat.Get(),
		m_pObjectiveTitleBox,
		m_pBrush.Get()
	);
}

void UpgradeScreenUI::RenderUpgrades() {
	for (unsigned int i = 0; i < m_pUpgrades.size(); i++) {
		m_pUpgrades.at(i)->Render();
	}
}

void UpgradeScreenUI::RenderControllerDisplay() {
	m_pObjectiveFormat.Get()->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	this->UpdateBrush(m_pGray, 1.0f);
	m_pRenderTarget2D->FillRectangle(m_pControllerDisplay, m_pBrush.Get());
	this->UpdateBrush(m_pDarkblue, 0.5f);
	m_pRenderTarget2D->FillRectangle(m_pControllerDisplay, m_pBrush.Get());

	//Grid for the square
	unsigned int gridSize = 25;
	this->UpdateBrush(m_pCyan, 0.25f);
	for (float x = m_pControllerDisplay.left; x < m_pControllerDisplay.right; x += gridSize)
	{
		m_pRenderTarget2D->DrawLine(
			D2D1::Point2F(static_cast<FLOAT>(x), m_pControllerDisplay.top),
			D2D1::Point2F(static_cast<FLOAT>(x), m_pControllerDisplay.bottom),
			m_pBrush.Get(),
			0.5f
		);
	}

	for (float y = m_pControllerDisplay.top; y < m_pControllerDisplay.bottom; y += gridSize)
	{
		m_pRenderTarget2D->DrawLine(
			D2D1::Point2F(m_pControllerDisplay.left, static_cast<FLOAT>(y)),
			D2D1::Point2F(m_pControllerDisplay.right, static_cast<FLOAT>(y)),
			m_pBrush.Get(),
			0.5f
		);
	}

	m_pRenderTarget2D->FillRectangle(&m_pControllerDisplay, m_pMainGradientBrush.Get());

	UpdateBrush(m_pWhite, 0.5f);
	m_pRenderTarget2D->DrawRectangle(m_pControllerDisplay, m_pBrush.Get());


	this->UpdateBrush(m_pYellow, 1.0f);
	m_pRenderTarget2D.Get()->DrawTextW(
		m_pControllerTitle.c_str(),
		(UINT32)m_pControllerTitle.length(),
		m_pTitleFormat.Get(),
		m_pControllerTitleBox,
		m_pBrush.Get()
	);

	this->UpdateBrush(m_pLightGray, 1.0f);
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
		m_pObjectiveFormat.Get(),
		m_pButtonBoxE,
		m_pBrush.Get()
	);

	m_pRenderTarget2D.Get()->DrawTextW(
		m_pButtonTextDescriptionE.c_str(),
		(UINT32)m_pButtonTextDescriptionE.length(),
		m_pFormat.Get(),
		m_pButtonBoxDescriptionE,
		m_pBrush.Get()
	);

	//Button Q
	this->UpdateBrush(m_pWhite, 1.0f);
	m_pRenderTarget2D.Get()->DrawTextW(
		m_pButtonTextQ.c_str(),
		(UINT32)m_pButtonTextQ.length(),
		m_pObjectiveFormat.Get(),
		m_pButtonBoxQ,
		m_pBrush.Get()
	);

	m_pRenderTarget2D.Get()->DrawTextW(
		m_pButtonTextDescriptionQ.c_str(),
		(UINT32)m_pButtonTextDescriptionQ.length(),
		m_pFormat.Get(),
		m_pButtonBoxDescriptionQ,
		m_pBrush.Get()
	);

	//Button W
	this->UpdateBrush(m_pWhite, 1.0f);
	m_pRenderTarget2D.Get()->DrawTextW(
		m_pButtonTextW.c_str(),
		(UINT32)m_pButtonTextW.length(),
		m_pObjectiveFormat.Get(),
		m_pButtonBoxW,
		m_pBrush.Get()
	);

	m_pRenderTarget2D.Get()->DrawTextW(
		m_pButtonTextDescriptionW.c_str(),
		(UINT32)m_pButtonTextDescriptionW.length(),
		m_pFormat.Get(),
		m_pButtonBoxDescriptionW,
		m_pBrush.Get()
	);

	//Button S
	this->UpdateBrush(m_pWhite, 1.0f);
	m_pRenderTarget2D.Get()->DrawTextW(
		m_pButtonTextS.c_str(),
		(UINT32)m_pButtonTextS.length(),
		m_pObjectiveFormat.Get(),
		m_pButtonBoxS,
		m_pBrush.Get()
	);

	m_pRenderTarget2D.Get()->DrawTextW(
		m_pButtonTextDescriptionS.c_str(),
		(UINT32)m_pButtonTextDescriptionS.length(),
		m_pFormat.Get(),
		m_pButtonBoxDescriptionS,
		m_pBrush.Get()
	);

	//Button U
	this->UpdateBrush(m_pWhite, 1.0f);
	m_pRenderTarget2D.Get()->DrawTextW(
		m_pButtonTextU.c_str(),
		(UINT32)m_pButtonTextU.length(),
		m_pObjectiveFormat.Get(),
		m_pButtonBoxU,
		m_pBrush.Get()
	);

	m_pRenderTarget2D.Get()->DrawTextW(
		m_pButtonTextDescriptionU.c_str(),
		(UINT32)m_pButtonTextDescriptionU.length(),
		m_pFormat.Get(),
		m_pButtonBoxDescriptionU,
		m_pBrush.Get()
	);

	//Mouse
	if (m_pRenderBitmaps) {
		m_pRenderTarget2D->DrawBitmap(m_pMouseBitmap, m_pMousePosition);
	}

	this->UpdateBrush(m_pWhite, 1.0f);
	m_pRenderTarget2D.Get()->DrawTextW(
		m_pButtonTextDescriptionLeft.c_str(),
		(UINT32)m_pButtonTextDescriptionLeft.length(),
		m_pFormat.Get(),
		m_pButtonBoxDescriptionLeft,
		m_pBrush.Get()
	);

	m_pRenderTarget2D.Get()->DrawTextW(
		m_pMouseTextDescription.c_str(),
		(UINT32)m_pMouseTextDescription.length(),
		m_pFormat.Get(),
		m_pMouseBoxDescription,
		m_pBrush.Get()
	);
}

void UpgradeScreenUI::RenderResourceList() {
	this->UpdateBrush(m_pGray, 1.0f);
	m_pRenderTarget2D->FillRectangle(m_pResourceDisplay, m_pBrush.Get());
	this->UpdateBrush(m_pDarkblue, 0.5f);
	m_pRenderTarget2D->FillRectangle(m_pResourceDisplay, m_pBrush.Get());

	//Grid for the square
	unsigned int gridSize = 25;
	this->UpdateBrush(m_pCyan, 0.25f);
	for (float x = m_pResourceDisplay.left; x < m_pResourceDisplay.right; x += gridSize)
	{
		m_pRenderTarget2D->DrawLine(
			D2D1::Point2F(static_cast<FLOAT>(x), m_pResourceDisplay.top),
			D2D1::Point2F(static_cast<FLOAT>(x), m_pResourceDisplay.bottom),
			m_pBrush.Get(),
			0.5f
		);
	}

	for (float y = m_pResourceDisplay.top; y < m_pResourceDisplay.bottom; y += gridSize)
	{
		m_pRenderTarget2D->DrawLine(
			D2D1::Point2F(m_pResourceDisplay.left, static_cast<FLOAT>(y)),
			D2D1::Point2F(m_pResourceDisplay.right, static_cast<FLOAT>(y)),
			m_pBrush.Get(),
			0.5f
		);
	}

	m_pRenderTarget2D->FillRectangle(&m_pResourceDisplay, m_pMainGradientBrush.Get());

	UpdateBrush(m_pWhite, 0.5f);
	m_pRenderTarget2D->DrawRectangle(m_pResourceDisplay, m_pBrush.Get());

	unsigned int i = 0;
	for (auto const& bitmap : m_pResourceBitmap) {
		m_pRenderTarget2D->DrawBitmap(bitmap, m_pResourcePosition.at(i));
		this->UpdateBrush(m_pWhite, 1.0f);
		m_pFormat.Get()->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
		m_pRenderTarget2D.Get()->DrawTextW(
			m_pAmount.at(i).c_str(),
			(UINT32)m_pAmount.at(i).length(),
			m_pFormat.Get(),
			m_pAmountTextbox.at(i),
			m_pBrush.Get()
		);

		m_pFormat.Get()->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		m_pRenderTarget2D.Get()->DrawTextW(
			resources.at(i).c_str(),
			(UINT32)resources.at(i).length(),
			m_pFormat.Get(),
			m_pDescriptionTextbox.at(i),
			m_pBrush.Get()
		);
		i++;
	}

	if (m_pRenderBitmaps) {
		m_pRenderTarget2D->DrawBitmap(m_pScienceBitmap, m_pSciencePosition);
	}

	this->UpdateBrush(m_pWhite, 1.0f);
	m_pRenderTarget2D.Get()->DrawTextW(
		m_pRequirement.c_str(),
		(UINT32)m_pRequirement.length(),
		m_pScienceFormat.Get(),
		m_pRequirementTextbox,
		m_pBrush.Get()
	);

	this->UpdateBrush(m_pYellow, 1.0f);
	m_pRenderTarget2D.Get()->DrawTextW(
		m_pInventoryTitle.c_str(),
		(UINT32)m_pInventoryTitle.length(),
		m_pTitleFormat.Get(),
		m_pInventoryTitleBox,
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
	case EventType::DelegatePlayerInfoEvent:
	{
		DelegatePlayerInfoEvent& derivedEvent = static_cast<DelegatePlayerInfoEvent&>(event);
		PlayerInfo* PlayerInfo = derivedEvent.GetPlayerInfo();
		m_inventory = PlayerInfo->inventory;
		m_pAmount.at(0) = std::to_wstring(PlayerInfo->inventory.khionerite);
		m_pAmount.at(1) = std::to_wstring(PlayerInfo->inventory.nanotech);
		m_pAmount.at(2) = std::to_wstring(PlayerInfo->inventory.plasma);
		m_pAmount.at(3) = std::to_wstring(PlayerInfo->inventory.radium);
		m_pAmount.at(4) = std::to_wstring(PlayerInfo->inventory.scrapMetal);
		m_pAmount.at(5) = std::to_wstring(PlayerInfo->inventory.titanium);
		m_pRequirement = std::to_wstring(PlayerInfo->inventory.science);
		break;
	}
	case EventType::MouseButtonEvent:
	{
		m_pMouseX = static_cast<MouseButtonEvent*>(&event)->GetXCoord();
		m_pMouseY = static_cast<MouseButtonEvent*>(&event)->GetYCoord();
		KeyState state = static_cast<MouseButtonEvent*>(&event)->GetKeyState();
		int virKey = static_cast<MouseButtonEvent*>(&event)->GetVirtualKeyCode();
		if (virKey == VK_LBUTTON && state == KeyState::KeyPress && m_pOnScreen) {
			for (unsigned int i = 0; i < m_pUpgrades.size(); i++) {
				m_pUpgrades.at(i)->OnClick(m_pMouseX, m_pMouseY, m_inventory);
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
	if (m_pMouseBitmap) {
		m_pMouseBitmap->Release();
	}
	for (unsigned int i = 0; i < m_pUpgrades.size(); i++) {
		m_pUpgrades.at(i)->CleanUp();
	}
}
