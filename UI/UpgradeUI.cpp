#include "..\pch.h"
#include "UpgradeUI.h"

UpgradeUI::UpgradeUI() noexcept {
	m_pHoverBox = D2D1::RectF();
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
		18.0f,
		L"en-us",
		&m_pCostFormat
	), "TextFormat");
	return true;
}

bool UpgradeUI::UpdateDescription() {
	return true;
}

bool UpgradeUI::UpdateCost() {
	return true;
}

bool UpgradeUI::UpdateModules() {
	if (!UpdateDescription()) {
		return false;
	}
	if (!UpdateCost()) {
		return false;
	}
	return true;
}

void UpgradeUI::RenderDescription() {
	this->UpdateBrush(D2D1::ColorF::Aquamarine, 0.25f);
	m_pRenderTarget2D->FillRectangle(m_pHoverBox, m_pBrush.Get());
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
}

void UpgradeUI::Render() {
	RenderDescription();
	RenderCost();
}

void UpgradeUI::OnClick(int mouseX, int mouseY)
{
}

void UpgradeUI::SetUpgrade(std::wstring description, unsigned int ID)
{
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
		m_pHoverBox.left + (iconSize + iconSize + padding) * x,
		m_pHoverBox.top + (padding + iconSize) * y,
		m_pHoverBox.left + iconSize + (iconSize + iconSize + padding) * x,
		m_pHoverBox.top + iconSize + (padding + iconSize) * y
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
	m_pHoverBox = hoverBox;
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
    //Will need bitmap clean up
}
