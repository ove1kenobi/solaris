#include "..\pch.h"
#include "HeadsUpDisplayUI.h"

HeadsUpDisplayUI::HeadsUpDisplayUI() {
	//only for showing that things can dynamically update
	EventBuss::Get().AddListener(this, EventType::DelegateMouseCoordsEvent);

	m_pCrosshairDistance = 12.0f;
	m_pCrosshairLength = 2.5f;
	m_pCrosshairSize = 2.5f;

	m_pMouseX = 10;
	m_pMouseY = 10;
}

HeadsUpDisplayUI::~HeadsUpDisplayUI() {

}

bool HeadsUpDisplayUI::Initialize() {
	if (!CreateTools()) {
		return false;
	}
	return true;
}

bool HeadsUpDisplayUI::CreateCrosshair()
{
	return true;
}

bool HeadsUpDisplayUI::CreateTools() {
	this->CreateBrush();
	return true;
}

bool HeadsUpDisplayUI::UpdateCrosshair() {
	return true;
}

bool HeadsUpDisplayUI::UpdateTools() {
	return true;
}

//Keep it like this for now until we get to picking
void HeadsUpDisplayUI::RenderCrosshair() {
	this->UpdateBrush(D2D1::ColorF::Snow, 0.5f);
	m_pRenderTarget2D->DrawLine(
		D2D1::Point2F((m_pWindowWidth / 2.0f) - m_pCrosshairLength, (m_pWindowHeight / 2.0f) - m_pCrosshairDistance),
		D2D1::Point2F((m_pWindowWidth / 2.0f) + m_pCrosshairLength, (m_pWindowHeight / 2.0f) - m_pCrosshairDistance),
		m_pBrush.Get(),
		m_pCrosshairSize
	);


	m_pRenderTarget2D->DrawLine(
		D2D1::Point2F((m_pWindowWidth / 2.0f) + m_pCrosshairDistance, (m_pWindowHeight / 2.0f) - m_pCrosshairLength),
		D2D1::Point2F((m_pWindowWidth / 2.0f) + m_pCrosshairDistance, (m_pWindowHeight / 2.0f) + m_pCrosshairLength),
		m_pBrush.Get(),
		m_pCrosshairSize
	);

	m_pRenderTarget2D->DrawLine(
		D2D1::Point2F((m_pWindowWidth / 2.0f) - m_pCrosshairLength, (m_pWindowHeight / 2.0f) + m_pCrosshairDistance),
		D2D1::Point2F((m_pWindowWidth / 2.0f) + m_pCrosshairLength, (m_pWindowHeight / 2.0f) + m_pCrosshairDistance),
		m_pBrush.Get(),
		m_pCrosshairSize
	);

	m_pRenderTarget2D->DrawLine(
		D2D1::Point2F((m_pWindowWidth / 2.0f) - m_pCrosshairDistance, (m_pWindowHeight / 2.0f) - m_pCrosshairLength),
		D2D1::Point2F((m_pWindowWidth / 2.0f) - m_pCrosshairDistance, (m_pWindowHeight / 2.0f) + m_pCrosshairLength),
		m_pBrush.Get(),
		m_pCrosshairSize
	);
}

bool HeadsUpDisplayUI::UpdateModules() {
	if (!UpdateCrosshair()) {
		false;
	}
	return true;
}

void HeadsUpDisplayUI::Render() {
	this->UpdateBrush(D2D1::ColorF::SteelBlue, 0.5f);
	//RenderHelpGrid(10);

   this->RenderCrosshair();
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
	//Only for showcasing dynamic change at the moment
	case EventType::DelegateMouseCoordsEvent:
	{
		m_pMouseX = static_cast<DelegateMouseCoordsEvent*>(&event)->GetXCoord();
		m_pMouseY = static_cast<DelegateMouseCoordsEvent*>(&event)->GetYCoord();
		break;
	}
	default:
		break;
	}
}
