#include "..\pch.h"
#include "UpgradeUI.h"

UpgradeUI::UpgradeUI() noexcept {
	m_pHoverBox = D2D1::RectF();
	m_pTextBox = D2D1::RectF();

	m_pID = 0;
	m_pBought = false;
	m_pRenderBitmaps = false;
}

UpgradeUI::~UpgradeUI() {
	for (auto const& bitmap : m_pResourceBitmap) {
		bitmap->Release();
	}
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
	return true;
}

bool UpgradeUI::CreateTitle() {
	ErrorCheck(m_pTextFactory->CreateTextFormat(
		L"Tenika",
		m_pFont.Get(),
		DWRITE_FONT_WEIGHT_REGULAR,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		20.0f,
		L"en-us",
		&m_pTitleFormat
	), "TextFormat");

	m_pTitleFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
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
	return true;
}

bool UpgradeUI::UpdateModules() {
	return true;
}

void UpgradeUI::RenderTitle() {
	this->UpdateBrush(D2D1::ColorF::Snow, 1.0f);
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
	if (m_pMouseX > m_pHoverBox.left && m_pMouseX < m_pHoverBox.right &&
		m_pMouseY > m_pHoverBox.top && m_pMouseY < m_pHoverBox.bottom &&
		!m_pBought) {
		this->UpdateBrush(D2D1::ColorF::Aquamarine, 0.5f);
	}
	else {
		this->UpdateBrush(D2D1::ColorF::Aquamarine, 0.25f);
	}
	m_pRenderTarget2D->FillRectangle(m_pHoverBox, m_pBrush.Get());

	this->UpdateBrush(D2D1::ColorF::Snow, 1.0f);
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
	for (auto const& bitmap : m_pResourceBitmap) {
		m_pRenderTarget2D->DrawBitmap(bitmap, m_pResourcePosition.at(i));
		this->UpdateBrush(D2D1::ColorF::Snow, 1.0f);
		m_pRenderTarget2D.Get()->DrawTextW(
			m_pCost.at(i).c_str(),
			(UINT32)m_pCost.at(i).length(),
			m_pCostFormat.Get(),
			m_pCostTextbox.at(i),
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

void UpgradeUI::Render() {
	RenderTitle();
	RenderDescription();
	RenderCost();

	if (m_pBought) {
		this->UpdateBrush(D2D1::ColorF::DimGray, 0.50f);
		m_pRenderTarget2D->FillRectangle(m_pHoverBox, m_pBrush.Get());
	}
}

void UpgradeUI::OnClick(int mouseX, int mouseY) {
	if (mouseX > m_pHoverBox.left && mouseX < m_pHoverBox.right &&
		mouseY > m_pHoverBox.top && mouseY < m_pHoverBox.bottom &&
		!m_pBought) {
		//Creates an event based on ID to figure out what upgrade was bought
		DelegateUpgradeID uID(m_pID);
		EventBuss::Get().Delegate(uID);
		m_pBought = true;
	}
}

void UpgradeUI::SetUpgrade(std::wstring upgrade, std::wstring description, unsigned int ID) {
	m_pTitle = upgrade;
	m_pDescription = description;
	m_pID = ID;
}

void UpgradeUI::SetScience(unsigned int science) {
	//Create science icon
	m_pSciencePosition = D2D1::RectF(
		m_pHoverBox.right - 50.0f,
		m_pHoverBox.top,
		m_pHoverBox.right,
		m_pHoverBox.top + 50.0f
	);
	//Create science bitmap
	LoadBitmapFromFile(GetIconFilePath(L"Science.png").c_str(), &m_pScienceBitmap);

	//Create text box
	m_pRequirementTextbox = D2D1::RectF(
		m_pSciencePosition.left,
		m_pSciencePosition.top,
		m_pSciencePosition.right,
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

	float x = static_cast<float>(m_pResourcePosition.size() % 2);
	float y = floor(static_cast<float>(m_pResourcePosition.size()) / 2.0f);

	//Add picture
	LoadBitmapFromFile(GetIconFilePath(resource).c_str(), &holder);
	m_pResourceBitmap.push_back(holder);

	//Add square for picture
	m_pResourcePosition.push_back(D2D1::RectF(
		(m_pHoverBox.right/2.0f) + (iconSize + iconSize + padding) * m_pResourcePosition.size(),
		m_pHoverBox.bottom - iconSize - 5.0f,
		(m_pHoverBox.right / 2.0f) + iconSize + (iconSize + iconSize + padding) * m_pResourcePosition.size(),
		m_pHoverBox.bottom - 5.0f
	));

	//Add square for text
	m_pCostTextbox.push_back(D2D1::RectF(
		m_pResourcePosition.at(m_pResourcePosition.size() - 1).right + 5.0f,
		m_pResourcePosition.at(m_pResourcePosition.size() - 1).top,
		m_pResourcePosition.at(m_pResourcePosition.size() - 1).right + iconSize + 5.0f,
		m_pResourcePosition.at(m_pResourcePosition.size() - 1).bottom
	));

	//Add text
	m_pCost.push_back(cost);
}

void UpgradeUI::SetHoverBox(D2D1_RECT_F hoverBox) {
	float textPadding = 10.0f;

	m_pHoverBox = hoverBox;

	m_pTextBox = D2D1::RectF(
		hoverBox.left + textPadding,
		hoverBox.top + textPadding,
		hoverBox.right - textPadding - 200.0f,
		hoverBox.bottom - textPadding
	);

	m_pTitleBox = D2D1::RectF(
		hoverBox.left + 20.0f,
		hoverBox.top - 20.0f,
		hoverBox.left + 200.0f,
		hoverBox.top + 10.0f
	);
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
    //Will need bitmap clean up
}
