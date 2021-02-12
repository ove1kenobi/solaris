#include "pch.h"
#include "Render2D.h"

Render2D::Render2D() noexcept 
	: m_pBrush{ nullptr }
{
	EventBuss::Get().AddListener(this, EventType::DelegateDXEvent);
}

const bool Render2D::Initialize() noexcept {
	//Brush created and only used for example
	HRESULT hr = m_pRenderTarget2D->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Aqua), &m_pBrush);
	if (FAILED(hr)) {
		printf("Error!\n");
		return false;
	}
	return true;
}

void Render2D::UpdateDXHandlers(IEvent& event) noexcept {
	DelegateDXEvent& derivedEvent = static_cast<DelegateDXEvent&>(event);
	m_pFactory = derivedEvent.GetFactory();
	m_pRenderTarget2D = derivedEvent.GetSurfaceRenderTarget();

#if defined(DEBUG) | defined(_DEBUG)
	assert(m_pFactory && m_pRenderTarget2D);
#endif
}

void Render2D::RenderUI() {
	//Draw call
	m_pRenderTarget2D->BeginDraw();

	//-----Example------
	//m_pRenderTarget2D->DrawRectangle(D2D1::RectF(100.0f, 50.0f, 800.0f, 200.0f), m_pBrush.Get());
	//-----Example------

	//End draw call
	HRESULT hr = m_pRenderTarget2D->EndDraw();
	if (FAILED(hr)) {
		printf("Error!\n");
	}
}

void Render2D::OnEvent(IEvent& event) noexcept {
	switch (event.GetEventType()) {
	case EventType::DelegateDXEvent:
	{
		UpdateDXHandlers(event);
		break;
	}
	default:
		break;
	}

	//Other possible events/states
	//case: inPlanetInteraction	
	//case: inMenu	
	//case: inGame
	//case: inSettings
	//case: inControllerDisplay
	//case: inUpgrades
	//case: inPause
	//case: inMap
}