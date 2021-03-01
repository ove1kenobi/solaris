#include "..\pch.h"
#include "HeadsUpDisplayUI.h"

HeadsUpDisplayUI::HeadsUpDisplayUI() {
	EventBuss::Get().AddListener(this, EventType::DelegateMouseCoordsEvent);

	m_pCrosshairDistance = 12.0f;
	m_pCrosshairLength = 2.5f;
	m_pCrosshairSize = 2.5f;

	m_pDistanceText = L"5000m";
	m_pDistanceTextBox = D2D1::RectF();

	m_pRightDisplayScreen = D2D1::RectF();
	m_pLeftDisplayScreen = D2D1::RectF();

	m_pFullHealthBar = D2D1::RectF();
	m_pCurrentHealthBar = D2D1::RectF();
	m_pFullCO2Bar = D2D1::RectF();
	m_pCurrentCO2Bar = D2D1::RectF();
	m_pFullFuelBar = D2D1::RectF();
	m_pCurrentFuelBar = D2D1::RectF();

	m_pCapacityTextBox = D2D1::RectF();
	m_pCapacityText = L"500 / 1225kg";

	m_pWarningTextBox = D2D1::RectF();
	m_pWarningText = L"!";

	m_pMiniMap = D2D1::RectF();

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
	if (!CreateCapacity()) {
		return false;
	}
	if (!CreateWarningModule()) {
		return false;
	}
	if (!CreateMiniMap()) {
		return false;
	}
	return true;
}

bool HeadsUpDisplayUI::CreateDisplayScreens() {
	//Create general text modules
	ErrorCheck(m_pTextFactory->GetSystemFontCollection(&m_pHUDFont, false), "GetSystemFont");

	ErrorCheck(m_pTextFactory->CreateTextFormat(
		L"Tenika",
		NULL,
		DWRITE_FONT_WEIGHT_REGULAR,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		30.0f,
		L"en-us",
		&m_pHUDFormat
	), "TextFormat");
	ErrorCheck(m_pHUDFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER), "TextAlignment");

	//Create screens
	UpdateDisplayScreens();
	return true;
}

//Creation functions

bool HeadsUpDisplayUI::CreateBars() {
	UpdateBars();
	return true;
}

bool HeadsUpDisplayUI::CreateCapacity() {
	UpdateCapacity();
	return true;
}

bool HeadsUpDisplayUI::CreateWarningModule() {
	ErrorCheck(m_pFactory2D->CreatePathGeometry(&m_pWarningTriangle), "PathGeometry");
	return true;
}

bool HeadsUpDisplayUI::CreateMiniMap() {
	UpdateMiniMap();
	return true;
}

bool HeadsUpDisplayUI::CreatePlanetDistanceModule() {
	//Create the text modules
	ErrorCheck(m_pTextFactory->GetSystemFontCollection(&m_pDistanceFont, false), "GetSystemFont");

	ErrorCheck(m_pTextFactory->CreateTextFormat(
		L"Tenika",
		NULL,
		DWRITE_FONT_WEIGHT_REGULAR,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		30.0f,
		L"en-us",
		&m_pDistanceFormat
	), "TextFormat");
	ErrorCheck(m_pDistanceFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER), "TextAlignment");
	return true;
}

bool HeadsUpDisplayUI::CreateCrosshair() {
	return true;
}

bool HeadsUpDisplayUI::CreateTools() {
	this->CreateBrush();
	return true;
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
	//Health bar
	m_pFullHealthBar = D2D1::RectF(
		m_pRightDisplayScreen.left + 50.0f,
		m_pRightDisplayScreen.bottom - 50.0f,
		m_pRightDisplayScreen.right - 50.0f,
		m_pRightDisplayScreen.bottom - 25.0f
	);

	m_pCurrentHealthBar = D2D1::RectF(
		m_pFullHealthBar.left + 100.0f,
		m_pFullHealthBar.top,
		m_pFullHealthBar.right,
		m_pFullHealthBar.bottom
	);

	//CO2 bar
	m_pFullCO2Bar = D2D1::RectF(
		m_pFullHealthBar.left,
		m_pFullHealthBar.top - 50.0f,
		m_pFullHealthBar.right,
		m_pFullHealthBar.top - 25.0f
	);

	m_pCurrentCO2Bar = D2D1::RectF(
		m_pFullCO2Bar.left + 150.0f,
		m_pFullCO2Bar.top,
		m_pFullCO2Bar.right,
		m_pFullCO2Bar.bottom
	);

	//Fuel bar
	m_pFullFuelBar = D2D1::RectF(
		m_pFullCO2Bar.left,
		m_pFullCO2Bar.top - 50.0f,
		m_pFullCO2Bar.right,
		m_pFullCO2Bar.top - 25.0f
	);

	m_pCurrentFuelBar = D2D1::RectF(
		m_pFullFuelBar.left + 75.0f,
		m_pFullFuelBar.top,
		m_pFullFuelBar.right,
		m_pFullFuelBar.bottom
	);
	return true;
}

bool HeadsUpDisplayUI::UpdateCapacity() {
	m_pCapacityTextBox = D2D1::RectF(
		m_pFullFuelBar.left,
		m_pFullFuelBar.top - 50.0f,
		m_pFullFuelBar.right,
		m_pFullFuelBar.top - 25.0f
	);
	return true;
}

bool HeadsUpDisplayUI::UpdateWarningModule() {
	bool updated = false;

	m_pWarningTextBox = D2D1::RectF(
		(m_pWindowWidth / 2.0f) - 50.0f,
		(m_pWindowHeight / 2.0f),
		(m_pWindowWidth / 2.0f) + 50.0f,
		(m_pWindowHeight / 2.0f) + 50.0f
	);

	if (ErrorCheck(m_pWarningTriangle->Open(&m_pSink), "OpenGeometry")) {
		m_pSink->SetFillMode(D2D1_FILL_MODE_WINDING);

		m_pSink->BeginFigure(
			D2D1::Point2F((m_pWindowWidth / 2.0f), (m_pWindowHeight / 2.0f) - 50.0f),
			D2D1_FIGURE_BEGIN_FILLED
		);
		D2D1_POINT_2F points[] = {
			D2D1::Point2F((m_pWindowWidth / 2.0f) - 50.0f, (m_pWindowHeight / 2.0f) + 50.0f),
			D2D1::Point2F((m_pWindowWidth / 2.0f) + 50.0f, (m_pWindowHeight / 2.0f) + 50.0f)
		};
		m_pSink->AddLines(points, ARRAYSIZE(points));
		m_pSink->EndFigure(D2D1_FIGURE_END_CLOSED);

		updated = ErrorCheck(m_pSink->Close(), "CloseGeometry");
	}
	return updated;
}

bool HeadsUpDisplayUI::UpdateMiniMap() {
	m_pMiniMap = D2D1::RectF(
		m_pLeftDisplayScreen.left + 25.0f,
		m_pLeftDisplayScreen.bottom - 225.0f,
		m_pLeftDisplayScreen.left + 225.0f,
		m_pLeftDisplayScreen.bottom - 25.0f
	);
	return true;
}

bool HeadsUpDisplayUI::UpdatePlanetDistanceModule() {
	m_pDistanceTextBox = D2D1::RectF(
		(m_pWindowWidth / 2.0f) - 75.0f,
		0.0f,
		(m_pWindowWidth / 2.0f) + 75.0f,
		60.0f
	);

	return true;
}

bool HeadsUpDisplayUI::UpdateCrosshair() {
	return true;
}

bool HeadsUpDisplayUI::UpdateTools() {
	return true;
}

bool HeadsUpDisplayUI::UpdateModules() {
	if (!UpdateDisplayScreens()) {
		return false;
	}
	if (!UpdateCrosshair()) {
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
	if (!UpdateMiniMap()) {
		return false;
	}
	if (!UpdateTools()) {
		return false;
	}
	return true;
}

//Render functions
void HeadsUpDisplayUI::RenderRightDisplayScreen() {
	this->UpdateBrush(D2D1::ColorF::Teal, 0.5f);
	m_pRenderTarget2D->FillRectangle(m_pRightDisplayScreen, m_pBrush.Get());
}

void HeadsUpDisplayUI::RenderLeftDisplayScreen() {
	this->UpdateBrush(D2D1::ColorF::Teal, 0.5f);
	m_pRenderTarget2D->FillRectangle(m_pLeftDisplayScreen, m_pBrush.Get());
}

void HeadsUpDisplayUI::RenderBars() {
	this->UpdateBrush(D2D1::ColorF::OrangeRed, 1.0f);
	m_pRenderTarget2D->FillRectangle(m_pCurrentHealthBar, m_pBrush.Get());

	this->UpdateBrush(D2D1::ColorF::Aqua, 1.0f);
	m_pRenderTarget2D->FillRectangle(m_pCurrentCO2Bar, m_pBrush.Get());

	this->UpdateBrush(D2D1::ColorF::Orange, 1.0f);
	m_pRenderTarget2D->FillRectangle(m_pCurrentFuelBar, m_pBrush.Get());

	this->UpdateBrush(D2D1::ColorF::Snow, 1.0f);
	m_pRenderTarget2D->DrawRectangle(m_pFullHealthBar, m_pBrush.Get(), 5.0f);
	m_pRenderTarget2D->DrawRectangle(m_pFullCO2Bar, m_pBrush.Get(), 5.0f);
	m_pRenderTarget2D->DrawRectangle(m_pFullFuelBar, m_pBrush.Get(), 5.0f);
}

void HeadsUpDisplayUI::RenderCapacity() {
	this->UpdateBrush(D2D1::ColorF::DarkBlue, 0.5f);
	m_pRenderTarget2D->FillRectangle(m_pCapacityTextBox, m_pBrush.Get());
}

void HeadsUpDisplayUI::RenderWarningModule() {
	this->UpdateBrush(D2D1::ColorF::OrangeRed, 1.0f);
	m_pRenderTarget2D->FillGeometry(m_pWarningTriangle.Get(), m_pBrush.Get());
	this->UpdateBrush(D2D1::ColorF::Snow, 1.0f);
	m_pRenderTarget2D->DrawGeometry(m_pWarningTriangle.Get(), m_pBrush.Get());

	//TODO: NEED TO CHANGE FORMAT
	m_pRenderTarget2D.Get()->DrawTextW(
		m_pWarningText.c_str(),
		(UINT32)m_pWarningText.length(),
		m_pDistanceFormat.Get(),
		m_pWarningTextBox,
		m_pBrush.Get()
	);
}

void HeadsUpDisplayUI::RenderMiniMap() {
	this->UpdateBrush(D2D1::ColorF::DarkBlue, 1.0f);
	m_pRenderTarget2D->FillRectangle(m_pMiniMap, m_pBrush.Get());
}

void HeadsUpDisplayUI::RenderPlanetDistanceModule() {
	//Render display
	this->UpdateBrush(D2D1::ColorF::Teal, 0.5f);
	m_pRenderTarget2D->FillRectangle(m_pDistanceTextBox, m_pBrush.Get());

	//Render the text on top of the display
	UpdateBrush(D2D1::ColorF::White, 1.0f);
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
	this->UpdateBrush(D2D1::ColorF::SteelBlue, 0.5f);
	RenderHelpGrid(50);

   RenderCrosshair();
   RenderPlanetDistanceModule();
   RenderRightDisplayScreen();
   RenderLeftDisplayScreen();
   RenderBars();
   RenderCapacity();
   //RenderWarningModule();
   //RenderMiniMap();
}

//Event functions
void HeadsUpDisplayUI::SetPlanetDistance(float distanceToPlanet, std::wstring planetName) {
	m_pDistanceText = planetName;
	m_pDistanceText.append(L": ");
	m_pDistanceText.append(std::to_wstring(distanceToPlanet));
}

void HeadsUpDisplayUI::SetHealth() {

}

void HeadsUpDisplayUI::SetCO2() {

}

void HeadsUpDisplayUI::SetFuel() {

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
		break;
	}
	case EventType::DelegatePlanetDistanceEvent:
	{
		this->SetPlanetDistance(
			static_cast<DelegatePlanetDistanceEvent*>(&event)->GetDistanceToObject(),
			static_cast<DelegatePlanetDistanceEvent*>(&event)->GetPlanetName()
		);
		break;
	}
	default:
		break;
	}
}
