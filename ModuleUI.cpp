#include "pch.h"
#include "ModuleUI.h"

void ModuleUI::UpdateDXHandlers(IEvent& event) noexcept {
	DelegateDXEvent& derivedEvent = static_cast<DelegateDXEvent&>(event);
	m_pFactory = derivedEvent.GetFactory();
	m_pRenderTarget2D = derivedEvent.GetSurfaceRenderTarget();

	#if defined(DEBUG) | defined(_DEBUG)
	assert(m_pFactory && m_pRenderTarget2D);
	#endif
}

int ModuleUI::GetWidth() {
	return static_cast<int>(m_pRenderTarget2D->GetSize().width);
}

int ModuleUI::GetHeight() {
	return static_cast<int>(m_pRenderTarget2D->GetSize().height);
}

ModuleUI::ModuleUI() noexcept {

}

const bool ModuleUI::Initialize() noexcept {
	HRESULT hr = m_pRenderTarget2D->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Aqua, 1.0f), &m_pBrush);
	if (FAILED(hr)) {
		printf("Error!\n");
		return false;
	}
    return false;
}

bool ModuleUI::UpdateBrush()
{
    return false;
}

void ModuleUI::BeginFrame() {
	m_pRenderTarget2D->BeginDraw();
}

void ModuleUI::RenderHelpGrid(int gridSize) {
	for (int x = 0; x < this->GetWidth(); x += gridSize)
	{
		m_pRenderTarget2D->DrawLine(
			D2D1::Point2F(static_cast<FLOAT>(x), 0.0f),
			D2D1::Point2F(static_cast<FLOAT>(x), this->GetHeight()),
			m_pBrush.Get(),
			0.5f
		);
	}

	for (int y = 0; y < this->GetHeight(); y += gridSize)
	{
		m_pRenderTarget2D->DrawLine(
			D2D1::Point2F(0.0f, static_cast<FLOAT>(y)),
			D2D1::Point2F(this->GetWidth(), static_cast<FLOAT>(y)),
			m_pBrush.Get(),
			0.5f
		);
	}
}

void ModuleUI::EndFrame() {
	HRESULT hr = m_pRenderTarget2D->EndDraw();
	if (FAILED(hr)) {
		printf("Error!\n");
	}
}

void ModuleUI::OnEvent(IEvent& event) noexcept {
	switch (event.GetEventType()) {
		case EventType::DelegateDXEvent:
		{
			UpdateDXHandlers(event);
			break;
		}
		default:
			break;
	}
}
