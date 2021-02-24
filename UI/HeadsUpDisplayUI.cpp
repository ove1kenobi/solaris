#include "..\pch.h"
#include "HeadsUpDisplayUI.h"

HeadsUpDisplayUI::HeadsUpDisplayUI() {

}

HeadsUpDisplayUI::~HeadsUpDisplayUI() {

}

bool HeadsUpDisplayUI::Initialize() {
	return this->CreateBrush();
}

bool HeadsUpDisplayUI::UpdateModules() {
	return true;
}

void HeadsUpDisplayUI::Render() {
this->UpdateBrush(D2D1::ColorF::PaleVioletRed, 1.0f);
   m_pRenderTarget2D->FillRectangle(D2D1::RectF(
		m_pWindowWidth / 2 - 100.0f,
		m_pWindowHeight / 2 - 100.0f,
		m_pWindowWidth / 2 + 100.0f,
		m_pWindowHeight / 2 + 100.0f
	), m_pBrush.Get());
}

void HeadsUpDisplayUI::OnEvent(IEvent& event) noexcept {
	switch (event.GetEventType()) {
	case EventType::DelegateDXEvent:
	{
		UpdateDXHandlers(event);
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
