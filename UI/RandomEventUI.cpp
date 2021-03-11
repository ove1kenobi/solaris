#include "..\pch.h"
#include "RandomEventUI.h"
#include <atlstr.h>

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
		14.0f,
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
		20.0f,
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
		8.0f,
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
		m_pHoverBox.right - m_pTextPadding,
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
void RandomEventUI::OnClick(int mouseX, int mouseY, UINT gameEventID) {
	if (mouseX > m_pHoverBox.left && mouseX < m_pHoverBox.right &&
		mouseY > m_pHoverBox.top && mouseY < m_pHoverBox.bottom) {
		this->SetText(GetGameEvent(gameEventID).consequence);
		GameEventSelectedEvent ev(gameEventID);
		EventBuss::Get().Delegate(ev);
		//Create event here to readjust the player inventory based on an random event ID
	}
}

void RandomEventUI::AddIcon(std::wstring amount) {
	/*Will in the future take in a picture and a string,
	which will be stored in the icon vectors */
}

