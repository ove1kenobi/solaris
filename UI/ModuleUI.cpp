#include "..\pch.h"
#include "ModuleUI.h"

ModuleUI::ModuleUI() noexcept {
	EventBuss::Get().AddListener(this, EventType::DelegateDXEvent);
	EventBuss::Get().AddListener(this, EventType::DelegateResolutionEvent);
	m_pWindowWidth = 0.0f;
	m_pWindowHeight = 0.0f;
}

bool ModuleUI::CreateBrush() {
	return ErrorCheck(m_pRenderTarget2D->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::Aqua, 0.5f),
		&m_pBrush),
		"SolidColorBrush"
	);
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

bool ModuleUI::ErrorCheck(HRESULT handle, std::string type) {
	std::string error;
	if (FAILED(handle)) {
		error.append("Error: ");												\
		error.append(type);													\
		error.append(" couldn't be created.");
		printf(error.c_str());
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
	for (unsigned int x = 0; x < m_pWindowWidth; x += gridSize)
	{
		m_pRenderTarget2D->DrawLine(
			D2D1::Point2F(static_cast<FLOAT>(x), 0.0f),
			D2D1::Point2F(static_cast<FLOAT>(x), static_cast<float>(m_pWindowHeight)),
			m_pBrush.Get(),
			0.5f
		);
	}

	for (unsigned int y = 0; y < m_pWindowHeight; y += gridSize)
	{
		m_pRenderTarget2D->DrawLine(
			D2D1::Point2F(0.0f, static_cast<FLOAT>(y)),
			D2D1::Point2F(static_cast<float>(m_pWindowWidth), static_cast<FLOAT>(y)),
			m_pBrush.Get(),
			0.5f
		);
	}
}

void ModuleUI::EndFrame() {
	ErrorCheck(m_pRenderTarget2D->EndDraw(), "EndDraw");
}
