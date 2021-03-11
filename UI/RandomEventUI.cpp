#include "..\pch.h"
#include "RandomEventUI.h"

RandomEventUI::RandomEventUI() {
	m_pHoverTextBox = D2D1::RectF();
	m_pHoverText = L"SELECT";
}

bool RandomEventUI::Initialize() {
	if (!CreateBrush()) {
		return false;
	}
	if (!CreateText()) {
		return false;
	}
	if (!CreateDetails()) {
		return false;
	}
	return true;
}

RandomEventUI::~RandomEventUI() {
	for (auto const& bitmap : m_pIconBitmap) {
		bitmap->Release();
	}
}

//Create functions
bool RandomEventUI::CreateText() {
	//Event text
	ErrorCheck(m_pTextFactory->GetSystemFontCollection(&m_pTextFont, false), "GetSystemFont");

	ErrorCheck(m_pTextFactory->CreateTextFormat(
		L"Tenika",
		m_pTextFont.Get(),
		DWRITE_FONT_WEIGHT_REGULAR,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		16.0f,
		L"en-us",
		&m_pTextFormat
	), "TextFormat");

	//Hover text
	ErrorCheck(m_pTextFactory->GetSystemFontCollection(&m_pHoverFont, false), "GetSystemFont");

	ErrorCheck(m_pTextFactory->CreateTextFormat(
		L"NEOTERIQUE",
		m_pHoverFont.Get(),
		DWRITE_FONT_WEIGHT_REGULAR,
		DWRITE_FONT_STYLE_ITALIC,
		DWRITE_FONT_STRETCH_NORMAL,
		22.0f,
		L"en-us",
		&m_pHoverTextFormat
	), "TextFormat");

	ErrorCheck(m_pHoverTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER), "TextAlignment");


	//Icon text
	ErrorCheck(m_pTextFactory->CreateTextFormat(
		L"Tenika",
		m_pTextFont.Get(),
		DWRITE_FONT_WEIGHT_REGULAR,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		14.0f,
		L"en-us",
		&m_pIconTextFormat
	), "TextFormat");

	return true;
}

bool RandomEventUI::CreateDetails() {
	ErrorCheck(m_pFactory2D->CreatePathGeometry(&m_pLeftHover), "PathGeometry");
	ErrorCheck(m_pFactory2D->CreatePathGeometry(&m_pRightHover), "PathGeometry");
	ErrorCheck(m_pFactory2D->CreatePathGeometry(&m_pBottomHover), "PathGeometry");
	return true;
}

//Update functions
bool RandomEventUI::UpdateDetails() {
	bool updated = false;
	if (ErrorCheck(m_pLeftHover->Open(&m_pSink), "OpenGeometry")) {
		m_pSink->SetFillMode(D2D1_FILL_MODE_WINDING);

		m_pSink->BeginFigure(
			D2D1::Point2F(m_pHoverBox.left - 2.5f, m_pHoverBox.bottom + 2.5f),
			D2D1_FIGURE_BEGIN_FILLED
		);
		D2D1_POINT_2F points[] = {
			D2D1::Point2F(m_pHoverBox.left - 2.5f, m_pHoverBox.top - 2.5f),
		   D2D1::Point2F(m_pHoverBox.left + 130.0f, m_pHoverBox.top - 2.5f),
		   D2D1::Point2F(m_pHoverBox.left + 125.0f, m_pHoverBox.top + 2.5f),
		   D2D1::Point2F(m_pHoverBox.left + 2.5f, m_pHoverBox.top + 2.5f),
		   D2D1::Point2F(m_pHoverBox.left + 2.5f, m_pHoverBox.bottom - 2.5f),
		   D2D1::Point2F(m_pHoverBox.left + 17.5f, m_pHoverBox.bottom - 2.5f),
		   D2D1::Point2F(m_pHoverBox.left + 15.5f, m_pHoverBox.bottom + 2.5f),
		};
		m_pSink->AddLines(points, ARRAYSIZE(points));
		m_pSink->EndFigure(D2D1_FIGURE_END_CLOSED);

		updated = ErrorCheck(m_pSink->Close(), "CloseGeometry");
	}

	if (ErrorCheck(m_pRightHover->Open(&m_pSink), "OpenGeometry") && updated) {
		m_pSink->SetFillMode(D2D1_FILL_MODE_WINDING);

		m_pSink->BeginFigure(
			D2D1::Point2F(m_pHoverBox.right + 2.5f, m_pHoverBox.bottom + 2.5f),
			D2D1_FIGURE_BEGIN_FILLED
		);
		D2D1_POINT_2F points[] = {
			D2D1::Point2F(m_pHoverBox.right + 2.5f, m_pHoverBox.top - 2.5f),
		   D2D1::Point2F(m_pHoverBox.right - 75.0f, m_pHoverBox.top - 2.5f),
		   D2D1::Point2F(m_pHoverBox.right - 80.0f, m_pHoverBox.top + 2.5f),
		   D2D1::Point2F(m_pHoverBox.right - 2.5f, m_pHoverBox.top + 2.5f),
		   D2D1::Point2F(m_pHoverBox.right - 2.5f, m_pHoverBox.bottom - 2.5f),
		   D2D1::Point2F(m_pHoverTextBox.right - 9.0f, m_pHoverBox.bottom - 2.5f),
			D2D1::Point2F(m_pHoverTextBox.right - 11.5f, m_pHoverBox.bottom + 2.5f),
		};
		m_pSink->AddLines(points, ARRAYSIZE(points));
		m_pSink->EndFigure(D2D1_FIGURE_END_CLOSED);

		updated = ErrorCheck(m_pSink->Close(), "CloseGeometry");
	}

	if (ErrorCheck(m_pBottomHover->Open(&m_pSink), "OpenGeometry") && updated) {
		m_pSink->SetFillMode(D2D1_FILL_MODE_WINDING);

		m_pSink->BeginFigure(
			D2D1::Point2F(m_pHoverTextBox.left + 8.0f, m_pHoverBox.bottom + 2.5f),
			D2D1_FIGURE_BEGIN_FILLED
		);
		D2D1_POINT_2F points[] = {
		   D2D1::Point2F(m_pHoverTextBox.left + 10.5f, m_pHoverBox.bottom - 2.5f),
		   D2D1::Point2F(m_pHoverTextBox.left - 75.0f, m_pHoverBox.bottom - 2.5f),
			D2D1::Point2F(m_pHoverTextBox.left - 80.0f, m_pHoverBox.bottom + 2.5f),
		};
		m_pSink->AddLines(points, ARRAYSIZE(points));
		m_pSink->EndFigure(D2D1_FIGURE_END_CLOSED);

		updated = ErrorCheck(m_pSink->Close(), "CloseGeometry");
	}
	return updated;
}

bool RandomEventUI::UpdateTextBoxes() {
	m_pTextBox = D2D1::RectF(
		m_pHoverBox.left + m_pTextPadding,
		m_pHoverBox.top + m_pTextPadding,
		m_pHoverBox.right - m_pTextPadding - 120.0f,
		m_pHoverBox.bottom - m_pTextPadding
	);

	m_pHoverTextBox = D2D1::RectF(
		m_pHoverBox.right - 150.0f,
		m_pHoverBox.bottom - 15.0f,
		m_pHoverBox.right - 50.0f,
		m_pHoverBox.bottom + 10.0f
	);
	return true;
}

bool RandomEventUI::UpdateModules() {
	if (!UpdateTextBoxes()) {
		return false;
	}
	if (!UpdateDetails()) {
		return false;
	}
	return true;
}

//Render functions
void RandomEventUI::Render(int mouseX, int mouseY) {
	//What should always be rendered
	this->UpdateBrush(D2D1::ColorF::Aqua, 0.05f);
	m_pRenderTarget2D->FillRectangle(m_pHoverBox, m_pBrush.Get());

	unsigned int i = 0;
	if (m_pDrawBitmaps) {
		for (auto const& bitmap : m_pIconBitmap) {
			m_pRenderTarget2D->DrawBitmap(bitmap, m_pIconPosition.at(i));
			this->UpdateBrush(D2D1::ColorF::Snow, 1.0f);
			m_pRenderTarget2D.Get()->DrawTextW(
				m_pIconAmount.at(i).c_str(),
				(UINT32)m_pIconAmount.at(i).length(),
				m_pIconTextFormat.Get(),
				m_pIconTextbox.at(i),
				m_pBrush.Get()
			);
			i++;
		}
	}

	this->UpdateBrush(D2D1::ColorF::Snow, 1.0f);

	m_pRenderTarget2D.Get()->DrawTextW(
		m_pText.c_str(),
		(UINT32)m_pText.length(),
		m_pTextFormat.Get(),
		m_pTextBox,
		m_pBrush.Get()
	);

	//if hover
	if (mouseX > m_pHoverBox.left && mouseX < m_pHoverBox.right &&
		mouseY > m_pHoverBox.top && mouseY < m_pHoverBox.bottom) {
		m_pRenderTarget2D->FillGeometry(m_pLeftHover.Get(), m_pBrush.Get());
		m_pRenderTarget2D->FillGeometry(m_pRightHover.Get(), m_pBrush.Get());
		m_pRenderTarget2D->FillGeometry(m_pBottomHover.Get(), m_pBrush.Get());

		m_pRenderTarget2D.Get()->DrawTextW(
			m_pHoverText.c_str(),
			(UINT32)m_pHoverText.length(),
			m_pHoverTextFormat.Get(),
			m_pHoverTextBox,
			m_pBrush.Get()
		);
	}
}

//Event related functions
void RandomEventUI::OnClick(int mouseX, int mouseY) {
	if (mouseX > m_pHoverBox.left && mouseX < m_pHoverBox.right &&
		mouseY > m_pHoverBox.top && mouseY < m_pHoverBox.bottom) {
		this->SetText(L"This event was clicked on! Time to edit player inventory.");
		//Create event here to readjust the player inventory based on an random event, 
		//(the information stored in vectors OR an ID that is tied to each event)
	}
}

void RandomEventUI::OnEvent(IEvent& event) noexcept {
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

//To add resources to the event
void RandomEventUI::AddIcon(std::wstring resource, std::wstring amount) {
	ID2D1Bitmap* holder = NULL;

	float iconSize = 25.0f;
	float amountSize = 25.0f;
	float padding = 10.0f;

	float x = static_cast<float>(m_pIconPosition.size() % 2);
	float y = floor(static_cast<float>(m_pIconPosition.size()) / 2.0f);

	//Add picture
	LoadBitmapFromFile(GetIconFilePath(resource).c_str(), &holder);
	m_pIconBitmap.push_back(holder);
	
	//Add square for picture
	m_pIconPosition.push_back(D2D1::RectF(
		m_pTextBox.right + (iconSize + iconSize + padding)*x,
		m_pTextBox.top + (padding + iconSize)*y,
		m_pTextBox.right + iconSize + (iconSize + iconSize + padding)*x,
		m_pTextBox.top + iconSize + (padding + iconSize)*y
	));

	//Add square for text
	m_pIconTextbox.push_back(D2D1::RectF(
		m_pIconPosition.at(m_pIconPosition.size() - 1).right + 5.0f,
		m_pIconPosition.at(m_pIconPosition.size() - 1).top,
		m_pIconPosition.at(m_pIconPosition.size() - 1).right + iconSize + 5.0f,
		m_pIconPosition.at(m_pIconPosition.size() - 1).bottom
	));

	//Add text
	m_pIconAmount.push_back(amount);
}



