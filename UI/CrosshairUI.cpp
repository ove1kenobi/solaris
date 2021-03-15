#include "..\pch.h"
#include "CrosshairUI.h"

CrosshairUI::CrosshairUI() noexcept {
    m_pCrosshairDistance = 12.0f;
    m_pCrosshairLength = 2.5f;
    m_pCrosshairSize = 2.5f;
}

CrosshairUI::~CrosshairUI() {

}

bool CrosshairUI::Initialize() {
    return CreateBrush();
}

bool CrosshairUI::UpdateModules() {
    return true;
}

void CrosshairUI::Render() {
	this->BeginFrame();

	this->UpdateBrush(D2D1::ColorF::Snow, 0.75f);
	m_pRenderTarget2D->DrawLine(
		D2D1::Point2F(m_pMouseX - m_pCrosshairLength, m_pMouseY - m_pCrosshairDistance),
		D2D1::Point2F(m_pMouseX + m_pCrosshairLength, m_pMouseY - m_pCrosshairDistance),
		m_pBrush.Get(),
		m_pCrosshairSize
	);


	m_pRenderTarget2D->DrawLine(
		D2D1::Point2F(m_pMouseX + m_pCrosshairDistance, m_pMouseY - m_pCrosshairLength),
		D2D1::Point2F(m_pMouseX + m_pCrosshairDistance, m_pMouseY + m_pCrosshairLength),
		m_pBrush.Get(),
		m_pCrosshairSize
	);

	m_pRenderTarget2D->DrawLine(
		D2D1::Point2F(m_pMouseX - m_pCrosshairLength, m_pMouseY + m_pCrosshairDistance),
		D2D1::Point2F(m_pMouseX + m_pCrosshairLength, m_pMouseY + m_pCrosshairDistance),
		m_pBrush.Get(),
		m_pCrosshairSize
	);

	m_pRenderTarget2D->DrawLine(
		D2D1::Point2F(m_pMouseX - m_pCrosshairDistance, m_pMouseY - m_pCrosshairLength),
		D2D1::Point2F(m_pMouseX - m_pCrosshairDistance, m_pMouseY + m_pCrosshairLength),
		m_pBrush.Get(),
		m_pCrosshairSize
	);

	this->EndFrame();
}

void CrosshairUI::OnEvent(IEvent& event) noexcept {
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

void CrosshairUI::CleanUp() {
	//Does not need any clean up
}
