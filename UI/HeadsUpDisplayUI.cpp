#include "..\pch.h"
#include "HeadsUpDisplayUI.h"

HeadsUpDisplayUI::HeadsUpDisplayUI() {
	EventBuss::Get().AddListener(this, EventType::DelegateMouseCoordsEvent);

	m_pCrosshairDistance = 12.0f;
	m_pCrosshairLength = 2.5f;
	m_pCrosshairSize = 2.5f;

	m_pPlanetText = L"TATOOINE";
	m_pDistanceText = L"100000m";
	m_pPlanetNameTextBox = D2D1::RectF();
	m_pDistanceTextBox = D2D1::RectF();

	m_pRightDisplayScreen = D2D1::RectF();
	m_pLeftDisplayScreen = D2D1::RectF();

	m_pCapacityTextBox = D2D1::RectF();
	m_pCapacityText = L"500 / 1225";

	m_pWarningTextBox = D2D1::RectF();
	m_pWarningText = L"!";

	m_pMouseX = 10;
	m_pMouseY = 10;
}

HeadsUpDisplayUI::~HeadsUpDisplayUI() {

}

bool HeadsUpDisplayUI::Initialize() {
	if (!CreateTools()) {
		return false;
	}
	if (!CreateDisplayScreens()) {
		return false;
	}
	if (!CreatePlanetDistanceModule()) {
		return false;
	}
	if (!CreateBars()) {
		return false;
	}
	if (!CreateWarningModule()) {
		return false;
	}
	return true;
}

//Creation functions
bool HeadsUpDisplayUI::CreateDisplayScreens() {
	//Create general text
	ErrorCheck(m_pTextFactory->CreateTextFormat(
		L"Tenika",
		NULL,
		DWRITE_FONT_WEIGHT_REGULAR,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		20.0f,
		L"en-us",
		&m_pHUDFormat
	), "TextFormat");
	return true;
}

bool HeadsUpDisplayUI::CreateBars() {
	m_pHealthBar.Initialize();
	m_pOxygenBar.Initialize();
	m_pFuelBar.Initialize();
	return true;
}

bool HeadsUpDisplayUI::CreateWarningModule() {
	ErrorCheck(m_pFactory2D->CreatePathGeometry(&m_pWarningTriangle), "PathGeometry");
	return true;
}

bool HeadsUpDisplayUI::CreatePlanetDistanceModule() {
	//Planet distance
	ErrorCheck(m_pTextFactory->CreateTextFormat(
		L"Tenika",
		NULL,
		DWRITE_FONT_WEIGHT_REGULAR,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		24.0f,
		L"en-us",
		&m_pDistanceFormat
	), "TextFormat");
	ErrorCheck(m_pDistanceFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER), "TextAlignment");

	//Planet name
	ErrorCheck(m_pTextFactory->CreateTextFormat(
		L"Aware",
		NULL,
		DWRITE_FONT_WEIGHT_REGULAR,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		36.0f,
		L"en-us",
		&m_pPlanetNameFormat
	), "TextFormat");
	ErrorCheck(m_pPlanetNameFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER), "TextAlignment");
	return true;
}

bool HeadsUpDisplayUI::CreateTools() {
	return this->CreateBrush();
}

//Update functions
bool HeadsUpDisplayUI::UpdateDisplayScreens() {
	//Right display screen
	m_pRightDisplayScreen = D2D1::RectF(
		m_pWindowWidth - 400.0f,
		m_pWindowHeight - 200.0f,
		m_pWindowWidth,
		m_pWindowHeight
	);

	//Left display screen
	m_pLeftDisplayScreen = D2D1::RectF(
		0.0f,
		m_pWindowHeight - 200.0f,
		300.0f,
		m_pWindowHeight
	);
	return true;
}

bool HeadsUpDisplayUI::UpdateBars() {
	m_pHealthBar.SetTools(D2D1::ColorF(1.0f, 0.0f, 0.0f, 1.0f),
		D2D1::ColorF(1.0f, 1.0f, 1.0f, 0.5f),
		D2D1::ColorF(0.0f, 0.0f, 0.0f, 0.5f));

	m_pHealthBar.SetFullBar(
		m_pRightDisplayScreen.left + 25.0f, 
		m_pRightDisplayScreen.bottom - 60.0f,
		m_pRightDisplayScreen.right - 55.0f,
		m_pRightDisplayScreen.bottom - 40.0f,
		5.0f);

	m_pOxygenBar.SetTools(D2D1::ColorF(0.0f, 0.0f, 1.0f, 1.0f),
		D2D1::ColorF(1.0f, 1.0f, 1.0f, 0.5f),
		D2D1::ColorF(0.0f, 0.0f, 0.0f, 0.5f));

	m_pOxygenBar.SetFullBar(
		m_pRightDisplayScreen.left + 25.0f,
		m_pRightDisplayScreen.bottom - 95.0f,
		m_pRightDisplayScreen.right - 55.0f,
		m_pRightDisplayScreen.bottom - 75.0f,
		5.0f);

	m_pFuelBar.SetTools(D2D1::ColorF(0.0f, 1.0f, 0.0f, 1.0f),
		D2D1::ColorF(1.0f, 1.0f, 1.0f, 0.5f),
		D2D1::ColorF(0.0f, 0.0f, 0.0f, 0.5f));

	m_pFuelBar.SetFullBar(
		m_pRightDisplayScreen.left + 25.0f,
		m_pRightDisplayScreen.bottom - 130.0f,
		m_pRightDisplayScreen.right - 55.0f,
		m_pRightDisplayScreen.bottom - 110.0f,
		5.0f);
	return true;
}

bool HeadsUpDisplayUI::UpdateCapacity() {
	m_pCapacityTextBox = D2D1::RectF(
		m_pRightDisplayScreen.left + ((m_pRightDisplayScreen.right - m_pRightDisplayScreen.left)/2.0f),
		m_pRightDisplayScreen.bottom - 165.0f,
		m_pRightDisplayScreen.right - 55.0f,
		m_pRightDisplayScreen.bottom - 145.0f
	);
	return true;
}

bool HeadsUpDisplayUI::UpdateWarningModule() {
	bool updated = false;

	m_pWarningTextBox = D2D1::RectF(
		m_pCapacityTextBox.left - 50.0f,
		m_pCapacityTextBox.bottom,
		m_pCapacityTextBox.left - 20.0f,
		m_pCapacityTextBox.bottom - 22.0f
	);

	if (ErrorCheck(m_pWarningTriangle->Open(&m_pSink), "OpenGeometry")) {
		m_pSink->SetFillMode(D2D1_FILL_MODE_WINDING);

		m_pSink->BeginFigure(
			D2D1::Point2F(m_pCapacityTextBox.left - 50.0f, m_pCapacityTextBox.bottom),
			D2D1_FIGURE_BEGIN_FILLED
		);
		D2D1_POINT_2F points[] = {
			D2D1::Point2F(m_pCapacityTextBox.left - 20.0f, m_pCapacityTextBox.bottom),
			D2D1::Point2F(m_pCapacityTextBox.left - 35.0f, m_pCapacityTextBox.bottom - 26.0f)
		};
		m_pSink->AddLines(points, ARRAYSIZE(points));
		m_pSink->EndFigure(D2D1_FIGURE_END_CLOSED);

		updated = ErrorCheck(m_pSink->Close(), "CloseGeometry");
	}
	return updated;
}

bool HeadsUpDisplayUI::UpdatePlanetDistanceModule() {
	m_pPlanetNameTextBox = D2D1::RectF(
		(m_pWindowWidth / 2.0f) - 150.0f,
		0.0f,
		(m_pWindowWidth / 2.0f) + 150.0f,
		40.0f
	);

	m_pDistanceTextBox = D2D1::RectF(
		(m_pWindowWidth / 2.0f) - 100.0f,
		m_pPlanetNameTextBox.bottom,
		(m_pWindowWidth / 2.0f) + 100.0f,
		m_pPlanetNameTextBox.bottom + 30.0f
	);

	return true;
}

bool HeadsUpDisplayUI::UpdateTools() {
	return true;
}

bool HeadsUpDisplayUI::UpdateModules() {
	if (!UpdateDisplayScreens()) {
		return false;
	}
	if (!UpdatePlanetDistanceModule()) {
		return false;
	}
	if (!UpdateBars()) {
		return false;
	}
	if (!UpdateCapacity()) {
		return false;
	}
	if (!UpdateWarningModule()) {
		return false;
	}
	if (!UpdateTools()) {
		return false;
	}
	return true;
}

//Render functions
void HeadsUpDisplayUI::RenderBars() {
	m_pHealthBar.Render();
	m_pOxygenBar.Render();
	m_pFuelBar.Render();
}

void HeadsUpDisplayUI::RenderCapacity() {
	this->UpdateBrush(D2D1::ColorF::DarkBlue, 0.5f);
	m_pRenderTarget2D->FillRectangle(m_pCapacityTextBox, m_pBrush.Get());

	ErrorCheck(m_pHUDFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING), "TextAlignment");

	this->UpdateBrush(D2D1::ColorF::Snow, 1.0f);
	m_pRenderTarget2D.Get()->DrawTextW(
		m_pCapacityText.c_str(),
		(UINT32)m_pCapacityText.length(),
		m_pHUDFormat.Get(),
		m_pCapacityTextBox,
		m_pBrush.Get()
	);
}

void HeadsUpDisplayUI::RenderWarningModule() {
	this->UpdateBrush(D2D1::ColorF::OrangeRed, 1.0f);
	m_pRenderTarget2D->FillGeometry(m_pWarningTriangle.Get(), m_pBrush.Get());
	this->UpdateBrush(D2D1::ColorF::Snow, 1.0f);
	m_pRenderTarget2D->DrawGeometry(m_pWarningTriangle.Get(), m_pBrush.Get(), 2.0f);

	ErrorCheck(m_pHUDFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER), "TextAlignment");

	m_pRenderTarget2D.Get()->DrawTextW(
		m_pWarningText.c_str(),
		(UINT32)m_pWarningText.length(),
		m_pHUDFormat.Get(),
		m_pWarningTextBox,
		m_pBrush.Get()
	);
}

void HeadsUpDisplayUI::RenderPlanetDistanceModule() {
	//Render the text
	UpdateBrush(D2D1::ColorF::White, 1.0f);
	m_pRenderTarget2D.Get()->DrawTextW(
		m_pPlanetText.c_str(),
		(UINT32)m_pPlanetText.length(),
		m_pPlanetNameFormat.Get(),
		m_pPlanetNameTextBox,
		m_pBrush.Get()
	);

	m_pRenderTarget2D.Get()->DrawTextW(
		m_pDistanceText.c_str(),
		(UINT32)m_pDistanceText.length(),
		m_pDistanceFormat.Get(),
		m_pDistanceTextBox,
		m_pBrush.Get()
	);
}

void HeadsUpDisplayUI::RenderCrosshair() {
	this->UpdateBrush(D2D1::ColorF::Snow, 0.5f);
	m_pRenderTarget2D->DrawLine(
		D2D1::Point2F((m_pWindowWidth / 2.0f) - m_pCrosshairLength, (m_pWindowHeight / 2.0f) - m_pCrosshairDistance),
		D2D1::Point2F((m_pWindowWidth / 2.0f) + m_pCrosshairLength, (m_pWindowHeight / 2.0f) - m_pCrosshairDistance),
		m_pBrush.Get(),
		m_pCrosshairSize
	);


	m_pRenderTarget2D->DrawLine(
		D2D1::Point2F((m_pWindowWidth / 2.0f) + m_pCrosshairDistance, (m_pWindowHeight / 2.0f) - m_pCrosshairLength),
		D2D1::Point2F((m_pWindowWidth / 2.0f) + m_pCrosshairDistance, (m_pWindowHeight / 2.0f) + m_pCrosshairLength),
		m_pBrush.Get(),
		m_pCrosshairSize
	);

	m_pRenderTarget2D->DrawLine(
		D2D1::Point2F((m_pWindowWidth / 2.0f) - m_pCrosshairLength, (m_pWindowHeight / 2.0f) + m_pCrosshairDistance),
		D2D1::Point2F((m_pWindowWidth / 2.0f) + m_pCrosshairLength, (m_pWindowHeight / 2.0f) + m_pCrosshairDistance),
		m_pBrush.Get(),
		m_pCrosshairSize
	);

	m_pRenderTarget2D->DrawLine(
		D2D1::Point2F((m_pWindowWidth / 2.0f) - m_pCrosshairDistance, (m_pWindowHeight / 2.0f) - m_pCrosshairLength),
		D2D1::Point2F((m_pWindowWidth / 2.0f) - m_pCrosshairDistance, (m_pWindowHeight / 2.0f) + m_pCrosshairLength),
		m_pBrush.Get(),
		m_pCrosshairSize
	);
}

void HeadsUpDisplayUI::Render() {
	//For helping with layout
	//this->UpdateBrush(D2D1::ColorF::SteelBlue, 0.5f);
	//RenderHelpGrid(10);

   RenderCrosshair();
   RenderPlanetDistanceModule();
   RenderBars();
   RenderCapacity();

   //Will work more on warning module later
   //RenderWarningModule();
}

//Event functions
void HeadsUpDisplayUI::SetPlanetDistance(unsigned int distanceToPlanet, std::wstring planetName) {
	std::transform(planetName.begin(), planetName.end(), planetName.begin(), ::toupper);
	m_pPlanetText = planetName;
	m_pDistanceText = std::to_wstring(distanceToPlanet);
	m_pDistanceText.append(L"m");
}

void HeadsUpDisplayUI::OnEvent(IEvent& event) noexcept {
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
		m_pMouseX = static_cast<DelegateMouseCoordsEvent*>(&event)->GetXCoord();
		m_pMouseY = static_cast<DelegateMouseCoordsEvent*>(&event)->GetYCoord();
		SetPlanetDistance(m_pMouseX, L"Tatooine");
		m_pHealthBar.SetCurrentBar(static_cast<float>(m_pMouseX / m_pWindowWidth));
		m_pOxygenBar.SetCurrentBar(static_cast<float>(m_pMouseY / m_pWindowHeight));
		m_pFuelBar.SetCurrentBar(static_cast<float>(m_pMouseX / m_pWindowWidth));
		break;
	}
	case EventType::DelegatePlanetDistanceEvent:
	{
		m_pDistanceText = static_cast<DelegatePlanetDistanceEvent*>(&event)->GetPlanetName();
		break;
	}
	default:
		break;
	}
}
