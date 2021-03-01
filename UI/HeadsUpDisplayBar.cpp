#include "..\pch.h"
#include "HeadsUpDisplayBar.h"

HeadsUpDisplayBar::HeadsUpDisplayBar() noexcept {
	EventBuss::Get().AddListener(this, EventType::DelegateMouseCoordsEvent);
	m_pBarPercentage = 100.0f;
}

bool HeadsUpDisplayBar::Initialize() {
	return this->CreateBrush();
}

void HeadsUpDisplayBar::SetFullBar(float left, float top, float right, float bottom, float blockPadding) {
	float width = right - left;
	float blockSize = (width - (blockPadding * 9.0f)) / 10.0f;
	for (unsigned int i = 0; i < 10; i++) {
		float currentBlock = left + (blockSize + blockPadding) * i;
		m_pFullBar.push_back(D2D1::RectF(
			currentBlock,
			top,
			currentBlock + blockSize,
			bottom
		));

		m_pCurrentBar.push_back(D2D1::RectF(
			currentBlock,
			top,
			currentBlock + blockSize,
			bottom
		));
	}
}

bool HeadsUpDisplayBar::UpdateModules() {
	return true;
}

void HeadsUpDisplayBar::Render() {
	this->UpdateBrush(D2D1::ColorF::OrangeRed, 1.0f);
	for (unsigned int i = 0; i < m_pCurrentBar.size(); i++) {
		m_pRenderTarget2D->FillRectangle(m_pCurrentBar.at(i), m_pBrush.Get());
	}

	this->UpdateBrush(D2D1::ColorF::Snow, 1.0f);
	for (unsigned int i = 0; i < m_pFullBar.size(); i++) {
		m_pRenderTarget2D->DrawRectangle(m_pFullBar.at(i), m_pBrush.Get(), 2.0f);
	}
}

void HeadsUpDisplayBar::OnEvent(IEvent& event) noexcept {
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
		m_pBarPercentage = static_cast<DelegateMouseCoordsEvent*>(&event)->GetXCoord()/m_pWindowWidth;
		break;
	}
	default:
		break;
	}
}
