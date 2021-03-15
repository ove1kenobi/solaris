#include "..\pch.h"
#include "HeadsUpDisplayBar.h"

HeadsUpDisplayBar::HeadsUpDisplayBar() noexcept {
	m_left = 10.0f;
	m_top = 10.0f;
	m_right = 10.0f;
	m_bottom = 10.0f;
	m_blockSize = 10.0f;
	m_blockPadding = 10.0f;
	m_pBarPercentage = 0.5f;
	m_pMainColor = { 0.0f, 0.0f, 0.0f, 1.0f };
	m_pWarningTextBox = D2D1::RectF();
	m_pWarningText = L"!";
}

bool HeadsUpDisplayBar::Initialize() {
	CreateBrush();
	ErrorCheck(m_pFactory2D->CreatePathGeometry(&m_pWarningTriangle), "PathGeometry");

	ErrorCheck(m_pTextFactory->CreateTextFormat(
		L"Tenika",
		NULL,
		DWRITE_FONT_WEIGHT_REGULAR,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		20.0f,
		L"en-us",
		&m_pWarningFormat
	), "TextFormat");


	ErrorCheck(m_pWarningFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER), "TextAlignment");

	return true;
}

void HeadsUpDisplayBar::SetTools(D2D_COLOR_F mainColor, D2D_COLOR_F gradientColorLight, D2D_COLOR_F gradientColorDark) {
	m_pMainColor = mainColor;

	//Main gradient
	D2D1_GRADIENT_STOP gradientStops[2];
	gradientStops[0].color = gradientColorLight;
	gradientStops[0].position = 0.0f;
	gradientStops[1].color = gradientColorDark;
	gradientStops[1].position = 1.0f;

	ErrorCheck(m_pRenderTarget2D->CreateGradientStopCollection(
		gradientStops,
		2,
		D2D1_GAMMA_1_0,
		D2D1_EXTEND_MODE_CLAMP,
		&m_pGradientStops
	), "GradientStops");

	ErrorCheck(m_pRenderTarget2D->CreateLinearGradientBrush(
		D2D1::LinearGradientBrushProperties(
			D2D1::Point2F(0.0f, 0.0f),
			D2D1::Point2F(0.0f, 0.0f)),
		m_pGradientStops.Get(),
		&m_pGradientBrush), "GradientBrush");
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

	CreateWarningModule();
}

bool HeadsUpDisplayBar::CreateWarningModule() {
	bool updated = false;

	m_pWarningTextBox = D2D1::RectF(
		m_left - 50.0f,
		m_bottom,
		m_left - 20.0f,
		m_bottom - 22.0f
	);

	if (ErrorCheck(m_pWarningTriangle->Open(&m_pSink), "OpenGeometry")) {
		m_pSink->SetFillMode(D2D1_FILL_MODE_WINDING);

		m_pSink->BeginFigure(
			D2D1::Point2F(m_left - 50.0f, m_bottom),
			D2D1_FIGURE_BEGIN_FILLED
		);
		D2D1_POINT_2F points[] = {
			D2D1::Point2F(m_left - 20.0f, m_bottom),
			D2D1::Point2F(m_left - 35.0f, m_bottom - 26.0f)
		};
		m_pSink->AddLines(points, ARRAYSIZE(points));
		m_pSink->EndFigure(D2D1_FIGURE_END_CLOSED);

		updated = ErrorCheck(m_pSink->Close(), "CloseGeometry");
	}
	return updated;
}

void HeadsUpDisplayBar::SetCurrentBar(float percentage) {
	m_pBarPercentage = percentage * 10.0f;
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
	this->m_pBrush.Get()->SetColor(m_pMainColor);

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
		m_pGradientBrush.Get()->SetStartPoint(D2D1::Point2F(m_pCurrentBar.at(i).left, m_pCurrentBar.at(i).top));
		m_pGradientBrush.Get()->SetEndPoint(D2D1::Point2F(m_pCurrentBar.at(i).right, m_pCurrentBar.at(i).top));

		m_pRenderTarget2D->FillRectangle(m_pCurrentBar.at(i), m_pBrush.Get());
		m_pRenderTarget2D->FillRectangle(m_pCurrentBar.at(i), m_pGradientBrush.Get());
	}

	//Draw outlines on all blocks
	this->UpdateBrush(D2D1::ColorF::Snow, 1.0f);
	for (unsigned int i = 0; i < m_pFullBar.size(); i++) {
		m_pRenderTarget2D->DrawRectangle(m_pFullBar.at(i), m_pBrush.Get(), 2.0f);
	}

	//Draw warning module
	if (m_pBarPercentage < 5.0f) {
		this->UpdateBrush(D2D1::ColorF::OrangeRed, 1.0f);
		m_pRenderTarget2D->FillGeometry(m_pWarningTriangle.Get(), m_pBrush.Get());
		this->UpdateBrush(D2D1::ColorF::Snow, 1.0f);
		m_pRenderTarget2D->DrawGeometry(m_pWarningTriangle.Get(), m_pBrush.Get(), 2.0f);

		m_pRenderTarget2D.Get()->DrawTextW(
			m_pWarningText.c_str(),
			(UINT32)m_pWarningText.length(),
			m_pWarningFormat.Get(),
			m_pWarningTextBox,
			m_pBrush.Get()
		);
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
	default:
		break;
	}
}

void HeadsUpDisplayBar::CleanUp() {

}