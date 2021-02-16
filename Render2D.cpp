#include "pch.h"
#include "Render2D.h"

Render2D::Render2D() noexcept 
	: m_pBrushDarkBlue{ nullptr }
{
	EventBuss::Get().AddListener(this, EventType::DelegateDXEvent);
}

const bool Render2D::Initialize() noexcept {
	//Brush for the square
	HRESULT hr = m_pRenderTarget2D->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Aqua, 0.15f), &m_pBrushDarkBlue);
	if (FAILED(hr)) {
		printf("Error!\n");
		return false;
	}

	//Brush for the corners
	hr = m_pRenderTarget2D->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::DarkCyan, 1.0f), &m_pBrushLightBlue);
	if (FAILED(hr)) {
		printf("Error!\n");
		return false;
	}

	//Get screen size
	D2D1_SIZE_F rtSize = m_pRenderTarget2D->GetSize();
	int width = static_cast<int>(rtSize.width);
	int height = static_cast<int>(rtSize.height);

	//Create main rectangle
	m_pMainRectangle = D2D1::RectF(
		100.0f,
		50.0f,
		rtSize.width - 100.0f,
		rtSize.height - 200.0f
	);

	//Create bottom left corner
	hr = m_pFactory->CreatePathGeometry(&m_pBottomLeft);
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

void Render2D::UpdateDXHandlers(IEvent& event) noexcept {
	DelegateDXEvent& derivedEvent = static_cast<DelegateDXEvent&>(event);
	m_pFactory = derivedEvent.GetFactory();
	m_pRenderTarget2D = derivedEvent.GetSurfaceRenderTarget();

#if defined(DEBUG) | defined(_DEBUG)
	assert(m_pFactory && m_pRenderTarget2D);
#endif
}

void Render2D::RenderUI() {
	//Draw call
	m_pRenderTarget2D->BeginDraw();

	D2D1_SIZE_F rtSize = m_pRenderTarget2D->GetSize();

	// Draw a grid background. (helps with layouts)
	int width = static_cast<int>(rtSize.width);
	int height = static_cast<int>(rtSize.height);

	int gridSize = 10;
	/*
	for (int x = 0; x < width; x += gridSize)
	{
		m_pRenderTarget2D->DrawLine(
			D2D1::Point2F(static_cast<FLOAT>(x), 0.0f),
			D2D1::Point2F(static_cast<FLOAT>(x), rtSize.height),
			m_pBrushLightBlue.Get(),
			0.5f
		);
	}

	for (int y = 0; y < height; y += gridSize)
	{
		m_pRenderTarget2D->DrawLine(
			D2D1::Point2F(0.0f, static_cast<FLOAT>(y)),
			D2D1::Point2F(rtSize.width, static_cast<FLOAT>(y)),
			m_pBrushLightBlue.Get(),
			0.5f
		);
	}*/

	//Planet interaction UI
	//Main Rectangle
	m_pRenderTarget2D->FillRectangle(&m_pMainRectangle, m_pBrushDarkBlue.Get());

	//Bottom Left
	m_pRenderTarget2D->FillGeometry(m_pBottomLeft.Get(), m_pBrushLightBlue.Get());

	//Bottom Right
	m_pRenderTarget2D->FillGeometry(m_pBottomRight.Get(), m_pBrushLightBlue.Get());

	//Top
	m_pRenderTarget2D->FillGeometry(m_pTop.Get(), m_pBrushLightBlue.Get());

	//---------------------
	/*TODO: clean up of Render2D
	- Create a class for PlanetInteraction UI
	- Store and create each shape in the PlanetInteraction class for Planet interaction
	- Brushes once defined will most likely be reused for other UI elements, move to DXCore
	- Render2D needs to know what brush to bind to what shape (event handler)
	- It's the 2D render who request what UI should be rendered based on the current gamestate
	*/
	//---------------------

	//End draw call
	HRESULT hr = m_pRenderTarget2D->EndDraw();
	if (FAILED(hr)) {
		printf("Error!\n");
	}
}

void Render2D::OnEvent(IEvent& event) noexcept {
	switch (event.GetEventType()) {
	case EventType::DelegateDXEvent:
	{
		UpdateDXHandlers(event);
		break;
	}
	default:
		break;
	}

	//Other possible events/states
	//case: inPlanetInteraction	
	//case: inMenu	
	//case: inGame
	//case: inSettings
	//case: inControllerDisplay
	//case: inUpgrades
	//case: inPause
	//case: inMap
}