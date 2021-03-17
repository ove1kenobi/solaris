#include "..\pch.h"
#include "HeadsUpDisplayUI.h"


HeadsUpDisplayUI::HeadsUpDisplayUI() {
	EventBuss::Get().AddListener(this, EventType::DelegatePlayerInfoEvent);
	EventBuss::Get().AddListener(this, EventType::DelegatePlanetDistanceEvent);
	EventBuss::Get().AddListener(this, EventType::ToggleDamageHUD);
	EventBuss::Get().AddListener(this, EventType::WindowCloseEvent);

	m_pHealthBitmap = NULL;
	m_pOxygenBitmap = NULL;
	m_pFuelBitmap = NULL;
	m_pCapacityBitmap = NULL;

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

	m_pRadiationIcon = D2D1::RectF();
	m_pRadiationScreen = D2D1::RectF();
	m_pRadiationTextBox = D2D1::RectF();
	m_pRadiationText = L"RADIATION AREA";

	m_pStabilizerText = L"Stabilizer";
	m_pStabilizer = true;

	m_pVelocityText = L"330";
	m_pUnitText = L"km/s";

	m_pWhite = 0xFFFDF9;
	m_pYellow = 0xFFB724;
	m_pBlue = 0x0BA4CC;
	m_pRed = 0xff4a24;

	m_pRenderBitmaps = true;
	m_pCapacityWarning = false;
	m_pRenderDistance = false;
	m_pRenderCold = false;
	m_pRenderHeat = false;
	m_pRenderRadiation = false;
}

HeadsUpDisplayUI::~HeadsUpDisplayUI() {
	EventBuss::Get().RemoveListener(this, EventType::DelegatePlayerInfoEvent);
	EventBuss::Get().RemoveListener(this, EventType::DelegatePlanetDistanceEvent);
	EventBuss::Get().RemoveListener(this, EventType::ToggleDamageHUD);
	EventBuss::Get().RemoveListener(this, EventType::WindowCloseEvent);
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
	if (!CreateDamageModules()) {
		return false;
	}
	if (!CreateVelocity()) {
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

bool HeadsUpDisplayUI::CreateDamageModules() {
	//Cold damage
	LoadBitmapFromFile(GetIconFilePath(L"FrostEffect.png").c_str(), &m_pFrostBitmap);

	//Warm damage
	//According to microsoft own example I'm suppose to do it like this
	D2D1_GRADIENT_STOP gradientStops[2];
	gradientStops[0].color = D2D1::ColorF(m_pYellow, 0.0f);
	gradientStops[0].position = 0.0f;
	gradientStops[1].color = D2D1::ColorF(m_pRed, 1.0f);
	gradientStops[1].position = 1.0f;

	ErrorCheck(m_pRenderTarget2D->CreateGradientStopCollection(
		gradientStops,
		2,
		D2D1_GAMMA_2_2,
		D2D1_EXTEND_MODE_CLAMP,
		&m_pHeatGradientStops
	), "GradientStops");

	ErrorCheck(m_pRenderTarget2D->CreateRadialGradientBrush(
		D2D1::RadialGradientBrushProperties(
			D2D1::Point2F(m_pWindowWidth / 2.0f, m_pWindowHeight /2.0f),
			D2D1::Point2F(0.0f, 0.0f),
			m_pWindowWidth,
			m_pWindowHeight),
		m_pHeatGradientStops.Get(),
		&m_pHeatRadialGradientBrush),
		"GradientBrush");
	
	//Radiation damage
	LoadBitmapFromFile(GetIconFilePath(L"Radioactive.png").c_str(), &m_pRadiationBitmap);

	ErrorCheck(m_pTextFactory->CreateTextFormat(
		L"Tenika",
		NULL,
		DWRITE_FONT_WEIGHT_REGULAR,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		40.0f,
		L"en-us",
		&m_pRadiationFormat
	), "TextFormat");
	return true;
}

bool HeadsUpDisplayUI::CreateVelocity() {
	ErrorCheck(m_pTextFactory->CreateTextFormat(
		L"Tenika",
		NULL,
		DWRITE_FONT_WEIGHT_REGULAR,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		80.0f,
		L"en-us",
		&m_pVelocityFormat
	), "TextFormat");
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
	m_pHealthBar.SetTools(D2D1::ColorF(m_pRed, 1.0f),
		D2D1::ColorF(0xffb824, 0.5f),
		D2D1::ColorF(0xff246c, 0.5f));

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
	m_pOxygenBar.SetTools(D2D1::ColorF(m_pBlue, 1.0f),
		D2D1::ColorF(0x0bcc94, 0.5f),
		D2D1::ColorF(0x0b44cc, 0.5f));

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
	m_pFuelBar.SetTools(D2D1::ColorF(m_pYellow, 1.0f),
		D2D1::ColorF(0xdaff24, 0.5f),
		D2D1::ColorF(0xff4a24, 0.5f));

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
		m_pRightDisplayScreen.left + 300.0f,
		m_pCapacityIcon.top,
		barRight,
		m_pCapacityIcon.bottom
	);
	return true;
}

bool HeadsUpDisplayUI::UpdateWarningModule() {
	bool updated = false;

	m_pWarningTextBox = D2D1::RectF(
		m_pCapacityTextBox.left - 30.0f,
		m_pCapacityTextBox.bottom,
		m_pCapacityTextBox.left - 0.0f,
		m_pCapacityTextBox.bottom - 22.0f
	);

	if (ErrorCheck(m_pWarningTriangle->Open(&m_pSink), "OpenGeometry")) {
		m_pSink->SetFillMode(D2D1_FILL_MODE_WINDING);

		m_pSink->BeginFigure(
			D2D1::Point2F(m_pCapacityTextBox.left - 30.0f, m_pCapacityTextBox.bottom),
			D2D1_FIGURE_BEGIN_FILLED
		);
		D2D1_POINT_2F points[] = {
			D2D1::Point2F(m_pCapacityTextBox.left - 0.0f, m_pCapacityTextBox.bottom),
			D2D1::Point2F(m_pCapacityTextBox.left - 15.0f, m_pCapacityTextBox.bottom - 26.0f)
		};
		m_pSink->AddLines(points, ARRAYSIZE(points));
		m_pSink->EndFigure(D2D1_FIGURE_END_CLOSED);

		updated = ErrorCheck(m_pSink->Close(), "CloseGeometry");
	}
	return updated;
}

bool HeadsUpDisplayUI::UpdatePlanetDistanceModule() {
	m_pPlanetNameTextBox = D2D1::RectF(
		(m_pWindowWidth / 2.0f) - 300.0f,
		0.0f,
		(m_pWindowWidth / 2.0f) + 300.0f,
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

bool HeadsUpDisplayUI::UpdateDamageModules() {
	m_pScreen = D2D1::RectF(
		0.0f,
		0.0f,
		m_pWindowWidth,
		m_pWindowHeight
	);

	m_pRadiationScreen = D2D1::RectF(
		(m_pWindowWidth/2.0f) - 180.0f,
		100.0f,
		(m_pWindowWidth / 2.0f) + 180.0f,
		200.0f
	);

	m_pRadiationIcon = D2D1::RectF(
		m_pRadiationScreen.left + 10.0f,
		m_pRadiationScreen.top + 10.0f,
		m_pRadiationScreen.left + 90.0f,
		m_pRadiationScreen.bottom - 10.0f
	);

	m_pRadiationTextBox = D2D1::RectF(
		m_pRadiationIcon.right,
		m_pRadiationScreen.top + 5.0f,
		m_pRadiationScreen.right,
		m_pRadiationScreen.bottom
	);

	m_pHeatRadialGradientBrush.Get()->SetCenter(D2D1::Point2F((m_pWindowWidth / 2.0f), (m_pWindowHeight /2.0f)));
	m_pHeatRadialGradientBrush.Get()->SetRadiusX(m_pWindowWidth);
	m_pHeatRadialGradientBrush.Get()->SetRadiusY(m_pWindowHeight);
	return true;
}

bool HeadsUpDisplayUI::UpdateStabilizer() {
	m_pStabilizerMode = D2D1::Ellipse(
		D2D1::Point2F(40.0f, m_pWindowHeight - 50.0f),
		10.0f,
		10.0f
	);

	m_pStabilizerTextBox = D2D1::RectF(
		60.0f,
		m_pWindowHeight - 60.0f,
		m_pWindowWidth / 2.0f,
		m_pWindowHeight - 40.0f
	);
	return true;
}

bool HeadsUpDisplayUI::UpdateVelocity() {
	m_pVelocityTextBox = D2D1::RectF(
		20.0f,
		m_pWindowHeight - 145.0f,
		m_pWindowWidth / 2.0f,
		m_pWindowHeight - 60.0f
	);

	m_pUnitTextBox = D2D1::RectF(
		215.0f,
		m_pWindowHeight - 95.0f,
		m_pWindowWidth / 2.0f,
		m_pWindowHeight - 60.0f
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
	if (!UpdateDamageModules()) {
		return false;
	}
	if (!UpdateTools()) {
		return false;
	}
	if (!UpdateStabilizer()) {
		return false;
	}
	if (!UpdateVelocity()) {
		return false;
	}
	return true;
}

//Render functions
void HeadsUpDisplayUI::RenderBars() {
	m_pHealthBar.Render();
	m_pOxygenBar.Render();
	m_pFuelBar.Render();

	if (m_pRenderBitmaps) {
		m_pRenderTarget2D->DrawBitmap(m_pHealthBitmap, m_pHealthIcon);
		m_pRenderTarget2D->DrawBitmap(m_pOxygenBitmap, m_pOxygenIcon);
		m_pRenderTarget2D->DrawBitmap(m_pFuelBitmap, m_pFuelIcon);
	}
}

void HeadsUpDisplayUI::RenderCapacity() {
	ErrorCheck(m_pHUDFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING), "TextAlignment");

	this->UpdateBrush(m_pWhite, 1.0f);
	m_pRenderTarget2D.Get()->DrawTextW(
		m_pCapacityText.c_str(),
		(UINT32)m_pCapacityText.length(),
		m_pHUDFormat.Get(),
		m_pCapacityTextBox,
		m_pBrush.Get()
	);
	if (m_pRenderBitmaps) {
		m_pRenderTarget2D->DrawBitmap(m_pCapacityBitmap, m_pCapacityIcon);
	}
}

void HeadsUpDisplayUI::RenderWarningModule() {
	this->UpdateBrush(m_pRed, 1.0f);
	m_pRenderTarget2D->FillGeometry(m_pWarningTriangle.Get(), m_pBrush.Get());
	this->UpdateBrush(m_pWhite, 1.0f);
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
	UpdateBrush(m_pWhite, 1.0f);
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

void HeadsUpDisplayUI::RenderDamageModule() {
	//Cold damage
	if (m_pRenderBitmaps && m_pRenderCold) {
		m_pRenderTarget2D->DrawBitmap(m_pFrostBitmap, m_pScreen);
	}

	//Warm damage
	if (m_pRenderHeat) {
		m_pRenderTarget2D->FillRectangle(m_pScreen, m_pHeatRadialGradientBrush.Get());
	}

	//Radiation damage
	if (m_pRenderRadiation) {
		UpdateBrush(m_pYellow, 1.0f);
		m_pRenderTarget2D->FillRectangle(m_pRadiationScreen, m_pBrush.Get());
		if (m_pRenderBitmaps) {
			m_pRenderTarget2D->DrawBitmap(m_pRadiationBitmap, m_pRadiationIcon);
		}

		ErrorCheck(m_pRadiationFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER), "TextAlignment");

		this->UpdateBrush(D2D1::ColorF::Black, 1.0f);
		m_pRenderTarget2D.Get()->DrawTextW(
			m_pRadiationText.c_str(),
			(UINT32)m_pRadiationText.length(),
			m_pRadiationFormat.Get(),
			m_pRadiationTextBox,
			m_pBrush.Get()
		);
		m_pRenderTarget2D->DrawRectangle(m_pRadiationScreen, m_pBrush.Get(), 10.0f);
	}
}

void HeadsUpDisplayUI::RenderStabilizer() {
	if (m_pStabilizer) {
		UpdateBrush(m_pBlue, 1.0f);
		m_pRenderTarget2D->FillEllipse(m_pStabilizerMode, m_pBrush.Get());
		m_pStabilizerText = L"Stabilizer ON";
	}
	else {
		UpdateBrush(m_pRed, 1.0f);
		m_pRenderTarget2D->FillEllipse(m_pStabilizerMode, m_pBrush.Get());
		m_pStabilizerText = L"Stabilizer OFF";
	}
	UpdateBrush(m_pWhite, 1.0f);
	m_pRenderTarget2D->DrawEllipse(m_pStabilizerMode, m_pBrush.Get(), 2.0f);

	ErrorCheck(m_pHUDFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING), "TextAlignment");

	this->UpdateBrush(m_pWhite, 1.0f);
	m_pRenderTarget2D.Get()->DrawTextW(
		m_pStabilizerText.c_str(),
		(UINT32)m_pStabilizerText.length(),
		m_pHUDFormat.Get(),
		m_pStabilizerTextBox,
		m_pBrush.Get()
	);
}

void HeadsUpDisplayUI::RenderVelocity() {
	UpdateBrush(m_pRed, 0.5f);

	this->UpdateBrush(m_pWhite, 1.0f);
	m_pRenderTarget2D.Get()->DrawTextW(
		m_pVelocityText.c_str(),
		(UINT32)m_pVelocityText.length(),
		m_pVelocityFormat.Get(),
		m_pVelocityTextBox,
		m_pBrush.Get()
	);

	m_pRenderTarget2D.Get()->DrawTextW(
		m_pUnitText.c_str(),
		(UINT32)m_pUnitText.length(),
		m_pHUDFormat.Get(),
		m_pUnitTextBox,
		m_pBrush.Get()
	);
}

void HeadsUpDisplayUI::Render() {
	BeginFrame();

	RenderDamageModule();

	if (m_pRenderDistance) {
		RenderPlanetDistanceModule();
	}

   RenderBars();
   RenderCapacity();
   RenderStabilizer();
   RenderVelocity();

   if (m_pCapacityWarning) {
	   RenderWarningModule();
   }
   EndFrame();
}

//Event functions
void HeadsUpDisplayUI::SetPlanetDistance(float distanceToPlanet, std::wstring planetName) {
	std::transform(planetName.begin(), planetName.end(), planetName.begin(), ::toupper);
	if (distanceToPlanet > 0.2)
	{
		m_pPlanetText = planetName;
		m_pDistanceText = std::to_wstring(static_cast<unsigned int>(distanceToPlanet));
		m_pDistanceText.append(L"km");
	}
	else
	{
		//Planet text will be the "error message" given if distance is too far.
		//distanceToPlanet = 0.1 simply means we are too far away from a planet to scan it correctly. 
		m_pPlanetText = planetName;
		m_pDistanceText = L"";
	}

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
	case EventType::DelegatePlayerInfoEvent:
	{
		DelegatePlayerInfoEvent& derivedEvent = static_cast<DelegatePlayerInfoEvent&>(event);
		PlayerInfo* PlayerInfo = derivedEvent.GetPlayerInfo();
		m_pHealthBar.SetCurrentBar(PlayerInfo->HealthPercentage);
		m_pOxygenBar.SetCurrentBar(PlayerInfo->oxygenPercentage);
		m_pFuelBar.SetCurrentBar(PlayerInfo->fuelPercentage);
		m_pStabilizer = PlayerInfo->stabilizerActive;
		m_pVelocityText = std::to_wstring(PlayerInfo->shipVelocity);
		SetCapacity(PlayerInfo->storageUsage, PlayerInfo->storageCapacity);
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
	}case EventType::ToggleDamageHUD:
	{
		m_pRenderCold = static_cast<ToggleDamageHUD*>(&event)->GetCold();
		m_pRenderHeat = static_cast<ToggleDamageHUD*>(&event)->GetHeat();
		m_pRenderRadiation = static_cast<ToggleDamageHUD*>(&event)->GetRadiation();
		break;
	}
	case EventType::WindowCloseEvent:
	{
		this->CleanUp();
	}
	default:
		break;
	}
}

void HeadsUpDisplayUI::CleanUp() {
	m_pRenderBitmaps = false;
	if (m_pHealthBitmap) {
		m_pHealthBitmap->Release();
	}
	if (m_pOxygenBitmap) {
		m_pOxygenBitmap->Release();
	}
	if (m_pFuelBitmap) {
		m_pFuelBitmap->Release();
	}
	if (m_pCapacityBitmap) {
		m_pCapacityBitmap->Release();
	}
	if (m_pFrostBitmap) {
		m_pFrostBitmap->Release();
	}
	if (m_pRadiationBitmap) {
		m_pRadiationBitmap->Release();
	}
}