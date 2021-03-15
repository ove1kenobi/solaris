#include "..\pch.h"
#include "UpgradeUI.h"

UpgradeUI::UpgradeUI() noexcept {
	m_pHoverBox = D2D1::RectF();
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
	return true;
}

bool UpgradeUI::CreateDescription() {
	return true;
}

bool UpgradeUI::CreateCost() {
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
	this->UpdateBrush(D2D1::ColorF::PowderBlue, 0.5f);
	m_pRenderTarget2D->FillRectangle(m_pHoverBox, m_pBrush.Get());
}

void UpgradeUI::RenderCost() {

}

void UpgradeUI::Render() {
	RenderDescription();
	RenderCost();
	//RenderHover

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
