#include "..\pch.h"
#include "HeadsUpDisplayUI.h"

HeadsUpDisplayUI::HeadsUpDisplayUI() {
	EventBuss::Get().AddListener(this, EventType::DelegateMouseCoordsEvent, EventType::DelegatePlanetDistanceEvent);

	m_pPlanetText = L"TATOOINE";
	m_pDistanceText = L"100000m";
	m_pPlanetNameTextBox = D2D1::RectF();
	m_pDistanceTextBox = D2D1::RectF();

	m_pRightDisplayScreen = D2D1::RectF();
	m_pLeftDisplayScreen = D2D1::RectF();

	m_pHealthIcon = D2D1::RectF();
	m_pOxygenIcon = D2D1::RectF();
	m_pFuelIcon = D2D1::RectF();

	m_pCapacityIcon = D2D1::RectF();
	m_pCapacityTextBox = D2D1::RectF();
	m_pCapacityText = L"500 / 1225";

	m_pWarningTextBox = D2D1::RectF();
	m_pWarningText = L"!";
	m_pCapacityWarning = false;

	m_pRenderDistance = false;

	m_pMouseX = 10;
	m_pMouseY = 10;
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
	//Health bar
	m_pHealthBar.Initialize();
	LoadBitmapFromFile(GetIconFilePath(L"Health.png").c_str(), &m_pHealthBitmap);

	//OxygenBar
	m_pOxygenBar.Initialize();
	LoadBitmapFromFile(GetIconFilePath(L"Oxygen.png").c_str(), &m_pOxygenBitmap);

	//FuelBar
	m_pFuelBar.Initialize();
	LoadBitmapFromFile(GetIconFilePath(L"Fuel.png").c_str(), &m_pFuelBitmap);
	return true;
}

bool HeadsUpDisplayUI::CreateCapacity() {
	LoadBitmapFromFile(GetIconFilePath(L"Weight.png").c_str(), &m_pCapacityBitmap);
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
		m_pWindowWidth - 500.0f,
		m_pWindowHeight - 300.0f,
		m_pWindowWidth,
		m_pWindowHeight
	);

	//Left display screen
	m_pLeftDisplayScreen = D2D1::RectF(
		0.0f,
		m_pWindowHeight - 300.0f,
		500.0f,
		m_pWindowHeight
	);
	return true;
}

bool HeadsUpDisplayUI::UpdateBars() {
	float padding = 20.0f;
	float barLeft = m_pRightDisplayScreen.left + 25.0f;
	float barRight = m_pRightDisplayScreen.right - 65.0f;
	float iconLeft = barRight + 5.0f;
	float iconRight = iconLeft + 25.0f;
	float barSize = 25.0f;

	//Health bar
	m_pHealthBar.SetTools(D2D1::ColorF(1.0f, 0.0f, 0.0f, 1.0f),
		D2D1::ColorF(1.0f, 1.0f, 1.0f, 0.5f),
		D2D1::ColorF(0.0f, 0.0f, 0.0f, 0.5f));

	m_pHealthIcon = D2D1::RectF(
		iconLeft,
		m_pRightDisplayScreen.bottom - 40.0f - barSize,
		iconRight,
		m_pRightDisplayScreen.bottom - 40.0f
	);

	m_pHealthBar.SetFullBar(
		barLeft,
		m_pHealthIcon.top,
		barRight,
		m_pHealthIcon.bottom,
		5.0f);

	//Oxygen bar
	m_pOxygenBar.SetTools(D2D1::ColorF(0.0f, 0.0f, 1.0f, 1.0f),
		D2D1::ColorF(1.0f, 1.0f, 1.0f, 0.5f),
		D2D1::ColorF(0.0f, 0.0f, 0.0f, 0.5f));

	m_pOxygenIcon = D2D1::RectF(
		iconLeft,
		m_pHealthIcon.top - padding - barSize,
		iconRight,
		m_pHealthIcon.top - padding
	);

	m_pOxygenBar.SetFullBar(
		barLeft,
		m_pOxygenIcon.top,
		barRight,
		m_pOxygenIcon.bottom,
		5.0f);

	//Fuel bar
	m_pFuelBar.SetTools(D2D1::ColorF(0.0f, 1.0f, 0.0f, 1.0f),
		D2D1::ColorF(1.0f, 1.0f, 1.0f, 0.5f),
		D2D1::ColorF(0.0f, 0.0f, 0.0f, 0.5f));

	m_pFuelIcon = D2D1::RectF(
		iconLeft,
		m_pOxygenIcon.top - padding - barSize,
		iconRight,
		m_pOxygenIcon.top - padding
	);

	m_pFuelBar.SetFullBar(
		barLeft,
		m_pFuelIcon.top,
		barRight,
		m_pFuelIcon.bottom,
		5.0f);

	//Capacity "bar"
	m_pCapacityIcon = D2D1::RectF(
		iconLeft,
		m_pFuelIcon.top - padding - barSize,
		iconRight,
		m_pFuelIcon.top - padding
	);

	m_pCapacityTextBox = D2D1::RectF(
		m_pRightDisplayScreen.left + ((m_pRightDisplayScreen.right - m_pRightDisplayScreen.left) / 2.0f),
		m_pCapacityIcon.top,
		barRight,
		m_pCapacityIcon.bottom
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
		(m_pWindowWidth / 2.0f) - 200.0f,
		0.0f,
		(m_pWindowWidth / 2.0f) + 200.0f,
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

	m_pRenderTarget2D->DrawBitmap(m_pHealthBitmap, m_pHealthIcon);
	m_pRenderTarget2D->DrawBitmap(m_pOxygenBitmap, m_pOxygenIcon);
	m_pRenderTarget2D->DrawBitmap(m_pFuelBitmap, m_pFuelIcon);
}

void HeadsUpDisplayUI::RenderCapacity() {
	ErrorCheck(m_pHUDFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING), "TextAlignment");

	this->UpdateBrush(D2D1::ColorF::Snow, 1.0f);
	m_pRenderTarget2D.Get()->DrawTextW(
		m_pCapacityText.c_str(),
		(UINT32)m_pCapacityText.length(),
		m_pHUDFormat.Get(),
		m_pCapacityTextBox,
		m_pBrush.Get()
	);

	m_pRenderTarget2D->DrawBitmap(m_pCapacityBitmap, m_pCapacityIcon);
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

void HeadsUpDisplayUI::Render() {
	if (m_pRenderDistance) {
		RenderPlanetDistanceModule();
	}

   RenderBars();
   RenderCapacity();

   if (m_pCapacityWarning) {
	   RenderWarningModule();
   }
}

//Event functions
void HeadsUpDisplayUI::SetPlanetDistance(float distanceToPlanet, std::wstring planetName) {
	std::transform(planetName.begin(), planetName.end(), planetName.begin(), ::toupper);
	m_pPlanetText = planetName;
	m_pDistanceText = std::to_wstring(static_cast<unsigned int>(distanceToPlanet));
	m_pDistanceText.append(L"m");
}

void HeadsUpDisplayUI::SetCapacity(unsigned int currentAmount, unsigned int maximumAmount) {
	m_pCapacityText = std::to_wstring(currentAmount);
	m_pCapacityText.append(L" / ");
	m_pCapacityText.append(std::to_wstring(maximumAmount));

	if (maximumAmount - currentAmount <= 100) {
		m_pCapacityWarning = true;
	}
	else {
		m_pCapacityWarning = false;
	}
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
	//EXAMPLE: Only for showcasing dynamic change at the moment
	case EventType::DelegateMouseCoordsEvent:
	{
		m_pMouseX = static_cast<DelegateMouseCoordsEvent*>(&event)->GetXCoord();
		m_pMouseY = static_cast<DelegateMouseCoordsEvent*>(&event)->GetYCoord();
		m_pHealthBar.SetCurrentBar(static_cast<float>(m_pMouseX / m_pWindowWidth));
		m_pOxygenBar.SetCurrentBar(static_cast<float>(m_pMouseY / m_pWindowHeight));
		m_pFuelBar.SetCurrentBar(static_cast<float>(m_pMouseX / m_pWindowWidth));
		break;
	}
	case EventType::DelegatePlanetDistanceEvent:
	{
		float distance = static_cast<DelegatePlanetDistanceEvent*>(&event)->GetDistanceToObject();
		if (distance != 0.0f) {
			m_pRenderDistance = true;
			SetPlanetDistance(
				static_cast<DelegatePlanetDistanceEvent*>(&event)->GetDistanceToObject(),
				static_cast<DelegatePlanetDistanceEvent*>(&event)->GetPlanetName()
			);
		}
		else {
			m_pRenderDistance = false;
		}
		break;
	}
	/*
	example case: EventType::DelegateCapacity:
	{
	SetCapacity(10, 100);
	}
	*/
	default:
		break;
	}
}
