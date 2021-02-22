#include "..\pch.h"
#include "RandomEventUI.h"

RandomEventUI::RandomEventUI() {
	m_pHoverTextBox = D2D1::RectF();
	m_pHoverText = L"Select";
}

RandomEventUI::~RandomEventUI()
{
}

bool RandomEventUI::CreateDetails() {
	ErrorCheck(m_pFactory2D->CreatePathGeometry(&m_pLeftHover), "PathGeometry");
	ErrorCheck(m_pFactory2D->CreatePathGeometry(&m_pRightHover), "PathGeometry");
	ErrorCheck(m_pFactory2D->CreatePathGeometry(&m_pBottomHover), "PathGeometry");
	return true;
}

bool RandomEventUI::UpdateDetails()
{
	return true;
}

bool RandomEventUI::Initialize() {
	m_pHoverBox = D2D1::RectF(
		100.0f,
		50.0f,
		m_pWindowWidth - 100.0f,
		m_pWindowHeight - 200.0f
	);
	if (!CreateBrush()) {
		return false;
	}
	if (!CreateDetails()) {
		return false;
	}
	return true;
}

void RandomEventUI::AddIcon(std::wstring amount)
{
}

bool RandomEventUI::UpdateModules() {
	m_pHoverBox = D2D1::RectF(
		100.0f,
		50.0f,
		m_pWindowWidth - 100.0f,
		m_pWindowHeight - 200.0f
	);

	if (!UpdateDetails()) {
		return false;
	}
	return false;
}

void RandomEventUI::Render() {
	this->UpdateBrush(D2D1::ColorF::Green, 0.25f);
	m_pRenderTarget2D->FillRectangle(m_pHoverBox, m_pBrush.Get());
}
