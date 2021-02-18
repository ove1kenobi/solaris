#include "pch.h"
#include "ModuleUI.h"

ModuleUI::ModuleUI() noexcept {
	EventBuss::Get().AddListener(this, EventType::DelegateDXEvent);
}

void ModuleUI::UpdateDXHandlers(IEvent& event) noexcept {
	DelegateDXEvent& derivedEvent = static_cast<DelegateDXEvent&>(event);
	m_pFactory2D = derivedEvent.GetFactory2D();
	m_pRenderTarget2D = derivedEvent.GetSurfaceRenderTarget();
	m_pTextFactory = derivedEvent.GetTextFactory();

	#if defined(DEBUG) | defined(_DEBUG)
	assert(m_pFactory2D && m_pRenderTarget2D && m_pTextFactory);
	#endif
}

int ModuleUI::GetWidth() {
	D2D1_SIZE_F rtSize = m_pRenderTarget2D->GetSize();
	return static_cast<int>(rtSize.width);
}

int ModuleUI::GetHeight() {
	D2D1_SIZE_F rtSize = m_pRenderTarget2D->GetSize();
	return static_cast<int>(rtSize.height);
}

bool ModuleUI::Initialize() {
	HRESULT hr = m_pRenderTarget2D->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Aqua, 0.5f), &m_pBrush);
	if (FAILED(hr)) {
		printf("Error!\n");
		return false;
	}

	return true;
}

void ModuleUI::UpdateBrush(D2D1::ColorF color, float opacity) {
	this->m_pBrush.Get()->SetColor(color);
	this->m_pBrush.Get()->SetOpacity(opacity);
}

void ModuleUI::BeginFrame() {
	m_pRenderTarget2D->BeginDraw();
}

void ModuleUI::RenderHelpGrid(int gridSize) {
	this->UpdateBrush(D2D1::ColorF::Aqua, 0.5f);
	for (int x = 0; x < this->GetWidth(); x += gridSize)
	{
		m_pRenderTarget2D->DrawLine(
			D2D1::Point2F(static_cast<FLOAT>(x), 0.0f),
			D2D1::Point2F(static_cast<FLOAT>(x), static_cast<float>(this->GetHeight())),
			m_pBrush.Get(),
			0.5f
		);
	}

	for (int y = 0; y < this->GetHeight(); y += gridSize)
	{
		m_pRenderTarget2D->DrawLine(
			D2D1::Point2F(0.0f, static_cast<FLOAT>(y)),
			D2D1::Point2F(static_cast<float>(this->GetWidth()), static_cast<FLOAT>(y)),
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
