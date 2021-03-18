#include "..\pch.h"
#include "UpgradeUI.h"

UpgradeUI::UpgradeUI() noexcept {
	m_pHoverBox = D2D1::RectF();
	m_pTextBox = D2D1::RectF();
	m_pRequirementTextbox = D2D1::RectF();
	m_pSciencePosition = D2D1::RectF();
	m_pTitleBox = D2D1::RectF();

	m_pID = 0;
	m_pBought = false;
	m_pRenderBitmaps = false;

	m_pCost.titanium = 0;
	m_pCost.scrapMetal = 0;
	m_pCost.nanotech = 0;
	m_pCost.plasma = 0;
	m_pCost.radium = 0;
	m_pCost.khionerite = 0;

	m_pYellow = 0xFFB724;
	m_pWhite = 0xFFFDF9;
	m_pCyan = 0x00ffff;
	m_pGray = 0x393b3d;
	m_pLightGray = 0x636363;
	m_pLightBlue = 0x0BA4CC;
	m_pDarkblue = 0x2741b4;
}

UpgradeUI::~UpgradeUI() {

}

bool UpgradeUI::Initialize() {
	if (!CreateBrush()) {
		return false;
	}
	if (!CreateDescription()) {
		return false;
	}
	if (!CreateCost()) {
		return false;
	}
	if (!CreateTitle()) {
		return false;
	}
	if (!CreateTools()) {
		return false;
	}
	return true;
}

bool UpgradeUI::CreateTitle() {
	ErrorCheck(m_pTextFactory->CreateTextFormat(
		L"Aware",
		m_pFont.Get(),
		DWRITE_FONT_WEIGHT_REGULAR,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		20.0f,
		L"en-us",
		&m_pTitleFormat
	), "TextFormat");

	m_pTitleFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
	return true;
}

bool UpgradeUI::CreateDescription() {
	ErrorCheck(m_pTextFactory->GetSystemFontCollection(&m_pFont, false), "GetSystemFont");

	ErrorCheck(m_pTextFactory->CreateTextFormat(
		L"Tenika",
		m_pFont.Get(),
		DWRITE_FONT_WEIGHT_REGULAR,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		18.0f,
		L"en-us",
		&m_pUpgradeFormat
	), "TextFormat");
	return true;
}

bool UpgradeUI::CreateCost() {
	ErrorCheck(m_pTextFactory->CreateTextFormat(
		L"Tenika",
		m_pFont.Get(),
		DWRITE_FONT_WEIGHT_REGULAR,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		14.0f,
		L"en-us",
		&m_pCostFormat
	), "TextFormat");


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

	m_pScienceFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);
	return true;
}

bool UpgradeUI::UpdateModules() {
	return true;
}

bool UpgradeUI::CreateTools() {
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
			D2D1::Point2F(m_pHoverBox.left, m_pWindowHeight),
			D2D1::Point2F(m_pHoverBox.right, m_pWindowHeight)),
		m_pMainGradientStops.Get(),
		&m_pMainGradientBrush), "GradientBrush");
	return true;
}

void UpgradeUI::RenderTitle() {
	this->UpdateBrush(m_pYellow, 1.0f);
	m_pRenderTarget2D.Get()->DrawTextW(
		m_pTitle.c_str(),
		(UINT32)m_pTitle.length(),
		m_pTitleFormat.Get(),
		m_pTitleBox,
		m_pBrush.Get()
	);
}

void UpgradeUI::RenderDescription() {
	//If hover
	this->UpdateBrush(m_pGray, 1.0f);
	m_pRenderTarget2D->FillRectangle(m_pHoverBox, m_pBrush.Get());

	if (m_pMouseX > m_pHoverBox.left && m_pMouseX < m_pHoverBox.right &&
		m_pMouseY > m_pHoverBox.top && m_pMouseY < m_pHoverBox.bottom &&
		!m_pBought) {
		this->UpdateBrush(m_pLightBlue, 0.5f);
	}
	else {
		this->UpdateBrush(m_pDarkblue, 0.5f);
	}
	m_pRenderTarget2D->FillRectangle(m_pHoverBox, m_pBrush.Get());

	//Grid for the square
	unsigned int gridSize = 25;
	this->UpdateBrush(m_pCyan, 0.25f);
	for (float x = m_pHoverBox.left; x < m_pHoverBox.right; x += gridSize)
	{
		m_pRenderTarget2D->DrawLine(
			D2D1::Point2F(static_cast<FLOAT>(x), m_pHoverBox.top),
			D2D1::Point2F(static_cast<FLOAT>(x), m_pHoverBox.bottom),
			m_pBrush.Get(),
			0.5f
		);
	}

	for (float y = m_pHoverBox.top; y < m_pHoverBox.bottom; y += gridSize)
	{
		m_pRenderTarget2D->DrawLine(
			D2D1::Point2F(m_pHoverBox.left, static_cast<FLOAT>(y)),
			D2D1::Point2F(m_pHoverBox.right, static_cast<FLOAT>(y)),
			m_pBrush.Get(),
			0.5f
		);
	}

	m_pRenderTarget2D->FillRectangle(&m_pHoverBox, m_pMainGradientBrush.Get());

	UpdateBrush(m_pWhite, 0.5f);
	m_pRenderTarget2D->DrawRectangle(m_pHoverBox, m_pBrush.Get());

	this->UpdateBrush(m_pWhite, 1.0f);

	m_pRenderTarget2D.Get()->DrawTextW(
		m_pDescription.c_str(),
		(UINT32)m_pDescription.length(),
		m_pUpgradeFormat.Get(),
		m_pTextBox,
		m_pBrush.Get()
	);
}

void UpgradeUI::RenderCost() {
	unsigned int i = 0;
	if (m_pRenderBitmaps) {
		for (auto const& bitmap : m_pResourceBitmap) {
			m_pRenderTarget2D->DrawBitmap(bitmap, m_pResourcePosition.at(i));
			this->UpdateBrush(m_pWhite, 1.0f);
			m_pRenderTarget2D.Get()->DrawTextW(
				m_pCostText.at(i).c_str(),
				(UINT32)m_pCostText.at(i).length(),
				m_pCostFormat.Get(),
				m_pCostTextbox.at(i),
				m_pBrush.Get()
			);
			i++;
		}

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
}

void UpgradeUI::InvertCost() {
	if(m_pCost.khionerite != 0)
		m_pCost.khionerite = -m_pCost.khionerite;
	if (m_pCost.nanotech != 0)
		m_pCost.nanotech = -m_pCost.nanotech;
	if (m_pCost.plasma != 0)
		m_pCost.plasma = -m_pCost.plasma;
	if (m_pCost.radium != 0)
		m_pCost.radium = -m_pCost.radium;
	if (m_pCost.scrapMetal != 0)
		m_pCost.scrapMetal = -m_pCost.scrapMetal;
	if (m_pCost.titanium != 0)
		m_pCost.titanium = -m_pCost.titanium;
}

bool UpgradeUI::CanAfford(Resources inventory) {
	if (inventory.khionerite < m_pCost.khionerite)
		return false;
	if (inventory.nanotech < m_pCost.nanotech)
		return false;
	if (inventory.plasma < m_pCost.plasma)
		return false;
	if (inventory.radium < m_pCost.radium)
		return false;
	if (inventory.scrapMetal < m_pCost.scrapMetal)
		return false;
	if (inventory.titanium < m_pCost.titanium)
		return false;

	return true;
}

void UpgradeUI::Render() {
	RenderDescription();
	RenderTitle();
	RenderCost();

	if (m_pBought) {
		this->UpdateBrush(D2D1::ColorF::DimGray, 0.50f);
		m_pRenderTarget2D->FillRectangle(m_pHoverBox, m_pBrush.Get());
	}
}

void UpgradeUI::OnClick(int mouseX, int mouseY, Resources inventory) {
	//If player clicked within the box
	if (mouseX > m_pHoverBox.left && mouseX < m_pHoverBox.right &&
		mouseY > m_pHoverBox.top && mouseY < m_pHoverBox.bottom &&
		!m_pBought) {
		//If player has enough science
		if (inventory.science >= std::stof(m_pRequirement)) {
			//And if player has enough resources
			if (CanAfford(inventory)) {
				//Creates an event based on ID to figure out what upgrade was bought
				//Event also contains cost to remove items from inventory
				InvertCost();
				DelegateUpgradeID uID(m_pID, m_pCost);
				EventBuss::Get().Delegate(uID);
				m_pBought = true;
			}
			else {
				//Not enough resources!
			}
		}
		else {
			//Not enough science!
		}
	}
}

void UpgradeUI::SetUpgrade(std::wstring upgrade, std::wstring description, unsigned int ID) {
	m_pTitle = upgrade;
	m_pDescription = description;
	m_pID = ID;
}

void UpgradeUI::SetScience(unsigned int science) {
	//Create science icon
	float offsetX = 10.0f;
	float offsetY = 15.0f;
	float scienceSize = 50.0f;
	
	m_pSciencePosition = D2D1::RectF(
		m_pHoverBox.right - scienceSize - offsetX,
		((m_pHoverBox.bottom - m_pHoverBox.top) / 2.0f) + m_pHoverBox.top - (scienceSize / 2.0f) - offsetY,
		m_pHoverBox.right - offsetX,
		((m_pHoverBox.bottom - m_pHoverBox.top) / 2.0f) + m_pHoverBox.top + (scienceSize / 2.0f) - offsetY
	);
	//Create science bitmap
	LoadBitmapFromFile(GetIconFilePath(L"Science.png").c_str(), &m_pScienceBitmap);

	//Create text box
	m_pRequirementTextbox = D2D1::RectF(
		m_pSciencePosition.left - 50.0f,
		m_pSciencePosition.top + 12.0f,
		m_pSciencePosition.left - 2.5f,
		m_pSciencePosition.bottom
	);

	//Load in text
	m_pRequirement = std::to_wstring(science);
	m_pRenderBitmaps = true;
}

void UpgradeUI::AddCost(std::wstring resource, std::wstring cost) {
	ID2D1Bitmap* holder = NULL;

	float iconSize = 25.0f;
	float amountSize = 25.0f;
	float padding = 10.0f;
	float offset = 5.0f;
	float pushIn = 180.0f;

	//Add picture
	LoadBitmapFromFile(GetIconFilePath(resource).c_str(), &holder);
	m_pResourceBitmap.push_back(holder);

	//Add square for picture
	m_pResourcePosition.push_back(D2D1::RectF(
		m_pHoverBox.right + (amountSize + iconSize + padding) * m_pResourcePosition.size() - pushIn,
		m_pHoverBox.bottom - iconSize - offset,
		m_pHoverBox.right + iconSize + (amountSize + iconSize + padding) * m_pResourcePosition.size() - pushIn,
		m_pHoverBox.bottom - offset
	));

	//Add square for text
	m_pCostTextbox.push_back(D2D1::RectF(
		m_pResourcePosition.at(m_pResourcePosition.size() - 1).right + offset,
		m_pResourcePosition.at(m_pResourcePosition.size() - 1).top + 7.5f,
		m_pResourcePosition.at(m_pResourcePosition.size() - 1).right + amountSize + offset,
		m_pResourcePosition.at(m_pResourcePosition.size() - 1).bottom
	));

	//Add text
	m_pCostText.push_back(cost);

	if (resource == L"Khionerite.png")
		m_pCost.khionerite = std::stoi(cost);
	if (resource == L"Nanotech.png")
		m_pCost.nanotech = std::stoi(cost);
	if (resource == L"Plasma.png")
		m_pCost.plasma = std::stoi(cost);
	if (resource == L"Radium.png")
		m_pCost.radium = std::stoi(cost);
	if (resource == L"Scrap.png")
		m_pCost.scrapMetal = std::stoi(cost);
	if (resource == L"Titanium.png")
		m_pCost.titanium = std::stoi(cost);
}

void UpgradeUI::SetHoverBox(D2D1_RECT_F hoverBox) {
	float textPadding = 10.0f;

	m_pHoverBox = hoverBox;

	m_pTextBox = D2D1::RectF(
		hoverBox.left + textPadding,
		hoverBox.top + textPadding + 20.0f,
		hoverBox.right - textPadding - 200.0f,
		hoverBox.bottom - textPadding
	);

	m_pTitleBox = D2D1::RectF(
		hoverBox.left + textPadding,
		hoverBox.top + 5.0f,
		hoverBox.right,
		hoverBox.top + 25.0f
	);

	m_pMainGradientBrush->SetStartPoint(D2D1::Point2F(m_pHoverBox.left, m_pWindowHeight));
	m_pMainGradientBrush->SetEndPoint(D2D1::Point2F(m_pHoverBox.right, m_pWindowHeight));
}

void UpgradeUI::OnEvent(IEvent& event) noexcept {
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

void UpgradeUI::CleanUp() {
	m_pRenderBitmaps = false;
	if (m_pScienceBitmap) {
		m_pScienceBitmap->Release();
	}
	for (auto const& bitmap : m_pResourceBitmap) {
		bitmap->Release();
	}
}
