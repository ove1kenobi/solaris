#include "..\pch.h"
#include "RandomEventUI.h"

RandomEventUI::RandomEventUI() {
	m_pHoverTextBox = D2D1::RectF();
	m_pHoverText = L"Select";
}

RandomEventUI::~RandomEventUI() {

}

bool RandomEventUI::CreateText() {
	//Event text
	ErrorCheck(m_pTextFactory->CreateTextFormat(
		L"Arial",
		NULL,
		DWRITE_FONT_WEIGHT_REGULAR,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		14.0f,
		L"en-us",
		&m_pTextFormat
	), "TextFormat");

	//Hover text
	ErrorCheck(m_pTextFactory->CreateTextFormat(
		L"Arial",
		NULL,
		DWRITE_FONT_WEIGHT_REGULAR,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		16.0f,
		L"en-us",
		&m_pHoverTextFormat
	), "TextFormat");

	//Icon text
	ErrorCheck(m_pTextFactory->CreateTextFormat(
		L"Arial",
		NULL,
		DWRITE_FONT_WEIGHT_REGULAR,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		16.0f,
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

bool RandomEventUI::UpdateDetails() {
	return true;
}

bool RandomEventUI::UpdateTextBoxes() {
	m_pTextBox = D2D1::RectF(
		m_pHoverBox.left + m_pTextPadding,
		m_pHoverBox.top + m_pTextPadding,
		m_pHoverBox.right - m_pTextPadding,
		m_pHoverBox.bottom - m_pTextPadding
	);
	return true;
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

void RandomEventUI::SetHoverBox(D2D1_RECT_F hoverBox, float textPadding) {
	m_pHoverBox = hoverBox;
	m_pTextPadding = textPadding;
	UpdateModules();
}

void RandomEventUI::AddIcon(std::wstring amount) {

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

void RandomEventUI::Render() {
	this->UpdateBrush(D2D1::ColorF::Snow, 1.0f);

	m_pRenderTarget2D.Get()->DrawTextW(
		m_pText.c_str(),
		(UINT32)m_pText.length(),
		m_pTextFormat.Get(),
		m_pTextBox,
		m_pBrush.Get()
	);

	m_pRenderTarget2D->DrawRectangle(m_pHoverBox, m_pBrush.Get(), 5.0f);

	this->UpdateBrush(D2D1::ColorF::BlueViolet, 1.0f);
	//m_pRenderTarget2D->DrawRectangle(m_pTextBox, m_pBrush.Get(), 5.0f);
	/*
	If x and y mouse coord is within hover box:
	- render extra geometry
	- render hover text
	*/
}
