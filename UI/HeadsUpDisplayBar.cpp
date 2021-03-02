#include "..\pch.h"
#include "HeadsUpDisplayBar.h"

HeadsUpDisplayBar::HeadsUpDisplayBar() noexcept {
	EventBuss::Get().AddListener(this, EventType::DelegateMouseCoordsEvent);
	m_left = 10.0f;
	m_top = 10.0f;
	m_right = 10.0f;
	m_bottom = 10.0f;
	m_blockSize = 10.0f;
	m_blockPadding = 10.0f;
	m_pBarPercentage = 0.5f;
}

bool HeadsUpDisplayBar::Initialize() {
	return this->CreateBrush();
}

void HeadsUpDisplayBar::SetFullBar(float left, float top, float right, float bottom, float blockPadding) {
	//Save variables for block recreation
	m_left = left;
	m_top = top;
	m_right = right;
	m_bottom = bottom;
	m_blockPadding = blockPadding;
	m_blockSize = ((m_right - m_left) - (m_blockPadding * 9.0f)) / 10.0f;

	//Create blocks
	for (unsigned int i = 0; i < 10; i++) {
		float currentBlock = m_left + (m_blockSize + m_blockPadding) * i;
		m_pFullBar.push_back(D2D1::RectF(
			currentBlock,
			m_top,
			currentBlock + m_blockSize,
			m_bottom
		));

		m_pCurrentBar.push_back(D2D1::RectF(
			currentBlock,
			m_top,
			currentBlock + m_blockSize,
			m_bottom
		));
	}
}

bool HeadsUpDisplayBar::UpdateModules() {
	return true;
}

void HeadsUpDisplayBar::ChangeBlockSize(unsigned int block, float amount) {
	float currentBlock = m_left + (m_blockSize + m_blockPadding) * block;
	m_pCurrentBar.at(block) = D2D1::RectF(
		currentBlock + (m_blockSize * amount),
		m_top,
		currentBlock + m_blockSize,
		m_bottom
	);
}

void HeadsUpDisplayBar::Render() {
	this->UpdateBrush(D2D1::ColorF::OrangeRed, 1.0f);

	//Calculate which block we need to edit based on player stats
	float block, amount;
	amount = std::modf(m_pBarPercentage, &block);
	amount = 1.0f - amount;
	unsigned int BlockID = static_cast<unsigned int>(m_pCurrentBar.size() - 1) - static_cast<unsigned int>(block);

	//Readjust and draw blocks based on current player stats
	this->ChangeBlockSize(BlockID, amount);

	for (unsigned int i = BlockID; i < m_pCurrentBar.size(); i++) {
		if (i != BlockID) {
			float currentBlock = m_left + (m_blockSize + m_blockPadding) * i;
			m_pCurrentBar.at(i) = D2D1::RectF(
				currentBlock,
				m_top,
				currentBlock + m_blockSize,
				m_bottom
			);
		}
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
		//Has to be in percentage form to work (example: 0,78)
		m_pBarPercentage = (static_cast<float>(static_cast<DelegateMouseCoordsEvent*>(&event)->GetXCoord()) / m_pWindowWidth)*10.0f;
		break;
	}
	default:
		break;
	}
}
