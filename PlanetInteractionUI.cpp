#include "pch.h"
#include "PlanetInteractionUI.h"

bool PlanetInteractionUI::CreateMainScreen() {
	//Create main rectangle
	m_pMainRectangle = D2D1::RectF(
		100.0f,
		50.0f,
		this->GetWidth() - 100.0f,
		this->GetHeight() - 200.0f
	);

	//Create bottom left corner
	HRESULT hr = m_pFactory->CreatePathGeometry(&m_pBottomLeft);
	if (FAILED(hr)) {
		printf("Error!\n");
		return false;
	}

	hr = m_pBottomLeft->Open(&m_pSink);
	if (SUCCEEDED(hr))
	{
		m_pSink->SetFillMode(D2D1_FILL_MODE_WINDING);

		m_pSink->BeginFigure(
			D2D1::Point2F(m_pMainRectangle.left + 10.0f, m_pMainRectangle.bottom + 10.0f),
			D2D1_FIGURE_BEGIN_FILLED
		);
		D2D1_POINT_2F points[] = {
		   D2D1::Point2F(m_pMainRectangle.left + 420.0f, m_pMainRectangle.bottom + 10.0f),	//Trinagle
		   D2D1::Point2F(m_pMainRectangle.left + 400.0f, m_pMainRectangle.bottom - 10.0f),
		   D2D1::Point2F(m_pMainRectangle.left + 170.0f, m_pMainRectangle.bottom - 10.0f),	//Trinagle
		   D2D1::Point2F(m_pMainRectangle.left + 150.0f, m_pMainRectangle.bottom - 30.0f),
		   D2D1::Point2F(m_pMainRectangle.left + 30.0f, m_pMainRectangle.bottom - 30.0f),
		   D2D1::Point2F(m_pMainRectangle.left + 30.0f, m_pMainRectangle.bottom - 150.0f),
		   D2D1::Point2F(m_pMainRectangle.left + 10.0f, m_pMainRectangle.bottom - 170.0f),	//Triangle
		   D2D1::Point2F(m_pMainRectangle.left + 10.0f, m_pMainRectangle.bottom - 230.0f),
		   D2D1::Point2F(m_pMainRectangle.left - 10.0f, m_pMainRectangle.bottom - 250.0f),	//Triangle
		   D2D1::Point2F(m_pMainRectangle.left - 10.0f, m_pMainRectangle.bottom - 10.0f),
		};
		m_pSink->AddLines(points, ARRAYSIZE(points));
		m_pSink->EndFigure(D2D1_FIGURE_END_CLOSED);
	}
	hr = m_pSink->Close();
	if (FAILED(hr)) {
		printf("Error!\n");
		return false;
	}

	//Create bottom right corner
	hr = m_pFactory->CreatePathGeometry(&m_pBottomRight);
	if (FAILED(hr)) {
		printf("Error!\n");
		return false;
	}

	hr = m_pBottomRight->Open(&m_pSink);
	if (SUCCEEDED(hr))
	{
		m_pSink->SetFillMode(D2D1_FILL_MODE_WINDING);

		m_pSink->BeginFigure(
			D2D1::Point2F(m_pMainRectangle.right - 10.0f, m_pMainRectangle.bottom + 10.0f),
			D2D1_FIGURE_BEGIN_FILLED
		);
		D2D1_POINT_2F points[] = {
		   D2D1::Point2F(m_pMainRectangle.right - 420.0f, m_pMainRectangle.bottom + 10.0f),	//Trinagle
		   D2D1::Point2F(m_pMainRectangle.right - 400.0f, m_pMainRectangle.bottom - 10.0f),
		   D2D1::Point2F(m_pMainRectangle.right - 170.0f, m_pMainRectangle.bottom - 10.0f),	//Trinagle
		   D2D1::Point2F(m_pMainRectangle.right - 150.0f, m_pMainRectangle.bottom - 30.0f),
		   D2D1::Point2F(m_pMainRectangle.right - 30.0f, m_pMainRectangle.bottom - 30.0f),
		   D2D1::Point2F(m_pMainRectangle.right - 30.0f, m_pMainRectangle.bottom - 150.0f),
		   D2D1::Point2F(m_pMainRectangle.right - 10.0f, m_pMainRectangle.bottom - 170.0f),	//Triangle
		   D2D1::Point2F(m_pMainRectangle.right - 10.0f, m_pMainRectangle.bottom - 230.0f),
		   D2D1::Point2F(m_pMainRectangle.right + 10.0f, m_pMainRectangle.bottom - 250.0f),	//Triangle
		   D2D1::Point2F(m_pMainRectangle.right + 10.0f, m_pMainRectangle.bottom - 10.0f),
		};
		m_pSink->AddLines(points, ARRAYSIZE(points));
		m_pSink->EndFigure(D2D1_FIGURE_END_CLOSED);
	}
	hr = m_pSink->Close();
	if (FAILED(hr)) {
		printf("Error!\n");
		return false;
	}

	//Create top
	hr = m_pFactory->CreatePathGeometry(&m_pTop);
	if (FAILED(hr)) {
		printf("Error!\n");
		return false;
	}

	hr = m_pTop->Open(&m_pSink);
	if (SUCCEEDED(hr))
	{
		m_pSink->SetFillMode(D2D1_FILL_MODE_WINDING);

		m_pSink->BeginFigure(
			D2D1::Point2F(m_pMainRectangle.right - 10.0f, m_pMainRectangle.top - 10.0f),
			D2D1_FIGURE_BEGIN_FILLED
		);
		D2D1_POINT_2F points[] = {
			//Title area
		   D2D1::Point2F(m_pMainRectangle.right - 280.0f, m_pMainRectangle.top - 10.0f),
		   D2D1::Point2F(m_pMainRectangle.right - 300.0f, m_pMainRectangle.top - 30.0f),
		   D2D1::Point2F(m_pMainRectangle.left + 300.0f, m_pMainRectangle.top - 30.0f),
		   D2D1::Point2F(m_pMainRectangle.left + 280.0f, m_pMainRectangle.top - 10.0f),

		   //Left corner
		   D2D1::Point2F(m_pMainRectangle.left + 10.0f, m_pMainRectangle.top - 10.0f),
		   D2D1::Point2F(m_pMainRectangle.left - 10.0f, m_pMainRectangle.top + 10.0f),
		   D2D1::Point2F(m_pMainRectangle.left - 10.0f, m_pMainRectangle.top + 120.0f),
		   D2D1::Point2F(m_pMainRectangle.left + 10.0f, m_pMainRectangle.top + 100.0f),
		   D2D1::Point2F(m_pMainRectangle.left + 10.0f, m_pMainRectangle.top + 80.0f),
		   D2D1::Point2F(m_pMainRectangle.left + 30.0f, m_pMainRectangle.top + 60.0f),
		   D2D1::Point2F(m_pMainRectangle.left + 30.0f, m_pMainRectangle.top + 30.0f),

		   //Right corner
		   D2D1::Point2F(m_pMainRectangle.right - 30.0f, m_pMainRectangle.top + 30.0f),

		   D2D1::Point2F(m_pMainRectangle.right - 30.0f, m_pMainRectangle.top + 60.0f),
		   D2D1::Point2F(m_pMainRectangle.right - 10.0f, m_pMainRectangle.top + 80.0f),
		   D2D1::Point2F(m_pMainRectangle.right - 10.0f, m_pMainRectangle.top + 100.0f),
		   D2D1::Point2F(m_pMainRectangle.right + 10.0f, m_pMainRectangle.top + 120.0f),

		   D2D1::Point2F(m_pMainRectangle.right + 10.0f, m_pMainRectangle.top + 10.0f),
		};
		m_pSink->AddLines(points, ARRAYSIZE(points));
		m_pSink->EndFigure(D2D1_FIGURE_END_CLOSED);
	}
	hr = m_pSink->Close();
	if (FAILED(hr)) {
		printf("Error!\n");
		return false;
	}
	return true;
}

bool PlanetInteractionUI::CreateTextElements()
{
	return true;
}

bool PlanetInteractionUI::CreateDetails()
{
	return true;
}

PlanetInteractionUI::PlanetInteractionUI() noexcept {
	m_pMainRectangle = D2D1::RectF();
}

void PlanetInteractionUI::RenderUI() {
	//Main screen
	//setBrush
	m_pRenderTarget2D->FillRectangle(&m_pMainRectangle, m_pBrush.Get());

	//setBrush
	m_pRenderTarget2D->FillGeometry(m_pBottomLeft.Get(), m_pBrush.Get());
	m_pRenderTarget2D->FillGeometry(m_pBottomRight.Get(), m_pBrush.Get());
	m_pRenderTarget2D->FillGeometry(m_pTop.Get(), m_pBrush.Get());

	//Text UI elements

	//Details
}

const bool PlanetInteractionUI::Initialize() noexcept {
	if (!this->CreateMainScreen()) {
		return false;
	}

	if (!this->CreateTextElements()) {
		return false;
	}

	if (!this->CreateDetails()) {
		return false;
	}
	return true;
}
