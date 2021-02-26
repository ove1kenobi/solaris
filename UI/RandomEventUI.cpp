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
	if (AddFontResource(this->GetFontFilePath(L"Tenika400Regular-Rpyql.ttf").c_str()) == 0) {
		OutputDebugString(L"Could not get path for Tenika.\n");
	}
	if (!ErrorCheck(m_pTextFactory->GetSystemFontCollection(&m_pTextFont, true), L"GetSystemFont")) {
		OutputDebugString(L"Failed to retrieve system font collection.\n");
	}
	UINT32 index;
	BOOL exists;
	if (!ErrorCheck(m_pTextFont.Get()->FindFamilyName(L"Tenika", &index, &exists), L"FindFamilyName")) {
		OutputDebugString(L"Could not find Tenika within the font collection.\n");
	}
	IDWriteFontFamily* family;
	if (exists) {
		if (!ErrorCheck(m_pTextFont.Get()->GetFontFamily(index, &family), L"GetFontFamily")) {
			OutputDebugString(L"Could not get Tenika within the font collection.\n");
		}
	}

	if (!ErrorCheck(m_pTextFactory->CreateTextFormat(
		L"Tenika",
		m_pTextFont.Get(),
		DWRITE_FONT_WEIGHT_REGULAR,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		14.0f,
		L"en-us",
		&m_pTextFormat
	), L"TextFormat")) {
		OutputDebugString(L"Failed to create DirectWrite Tenika text format at line 44.\n");
	}

	//Hover text
	if (!ErrorCheck(m_pTextFactory->GetSystemFontCollection(&m_pHoverFont, true), L"GetSystemFont")) {
		OutputDebugString(L"Failed to retrieve system font collection at line 49.\n");
	}

	if(!ErrorCheck(m_pTextFactory->CreateTextFormat(
		L"NEOTERIQUE",
		m_pHoverFont.Get(),
		DWRITE_FONT_WEIGHT_REGULAR,
		DWRITE_FONT_STYLE_ITALIC,
		DWRITE_FONT_STRETCH_NORMAL,
		20.0f,
		L"en-us",
		&m_pHoverTextFormat
	), L"TextFormat")) {
		OutputDebugString(L"Failed to create NEOTERIQUE text format at line 62.\n");
	}

	ErrorCheck(m_pHoverTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER), L"TextAlignment");


	//Icon text
	if (!ErrorCheck(m_pTextFactory->CreateTextFormat(
		L"Tenika",
		m_pTextFont.Get(),
		DWRITE_FONT_WEIGHT_REGULAR,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		8.0f,
		L"en-us",
		&m_pIconTextFormat
	), L"TextFormat")) {
		OutputDebugString(L"Failed to create DirectWrite Tenika text format at line 69.\n");
	}

	return true;
}

bool RandomEventUI::CreateDetails() {
	ErrorCheck(m_pFactory2D->CreatePathGeometry(&m_pLeftHover), L"PathGeometry");
	ErrorCheck(m_pFactory2D->CreatePathGeometry(&m_pRightHover), L"PathGeometry");
	ErrorCheck(m_pFactory2D->CreatePathGeometry(&m_pBottomHover), L"PathGeometry");
	return true;
}

//Update functions
bool RandomEventUI::UpdateDetails() {
	bool updated = false;
	if (ErrorCheck(m_pLeftHover->Open(&m_pSink), L"OpenGeometry line 95")) {
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

		updated = ErrorCheck(m_pSink->Close(), L"CloseGeometry");
		//m_pSink.Get()->Release();
	}

	if (ErrorCheck(m_pRightHover->Open(&m_pSink), L"OpenGeometry line 117") && updated) {
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

		updated = ErrorCheck(m_pSink->Close(), L"CloseGeometry");
		//m_pSink.Get()->Release();
	}

	if (ErrorCheck(m_pBottomHover->Open(&m_pSink), L"OpenGeometry 139") && updated) {
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

		updated = ErrorCheck(m_pSink->Close(), L"CloseGeometry");
		//m_pSink.Get()->Release();
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

	//error with rendering event text
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
		//Create event here to readjust the player inventory based on an random event ID
	}
}

void RandomEventUI::AddIcon(std::wstring amount) {
	/*Will in the future take in a picture and a string,
	which will be stored in the icon vectors */
}

