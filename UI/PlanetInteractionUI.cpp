#include "..\pch.h"
#include "PlanetInteractionUI.h"

//Initialize functions
PlanetInteractionUI::PlanetInteractionUI() noexcept {		
	EventBuss::Get().AddListener(this, EventType::MouseButtonEvent);
	EventBuss::Get().AddListener(this, EventType::DelegatePlayerInfoEvent);
	m_pMainRectangle = D2D1::RectF();
	m_pPlanetNameTextBox = D2D1::RectF();
	m_pPlanetFlavourTextBox = D2D1::RectF();

	m_pRandomEvents.push_back(new RandomEventUI());
	m_pRandomEvents.push_back(new RandomEventUI());
	m_pRandomEvents.push_back(new RandomEventUI());

	//Example text: should be removed once event system is in place.
	m_pPlanetNameText = L"Place holder";

	m_pPlanetFlavourText = L"Luke Skywalker has returned to his home planet of Tatooine in an attempt "
		L"to rescue his friend Han Solo from the clutches of the vile gangster Jabba the Hutt."
		L" Little does Luke know that the GALACTIC EMPIRE has secretly begun construction on a new "
		L"armored space station even more powerful than the first dreaded Death Star."
		L"When completed, this ultimate weapon will spell certain doom for the small band "
		L"of rebels struggling to restore freedom to the galaxy...";

	m_pRandomEvents.at(0)->SetText(L"It is a period of civil war. Rebel spaceships, striking from a hidden base, "
		L"have won their first victory against the evil Galactic Empire. During the battle, "
		L"Rebel spies managed to steal secret plans to the Empire?s ultimate weapon, the death star, "
		L"an armored space station with enough power to destroy an entire planet. ");

	m_pRandomEvents.at(1)->SetText(L"It is a dark time for the Rebellion.Although the Death Star has been destroyed, "
		L"Imperial troops have driven the Rebel forces from their hidden baseand pursued them across the galaxy."
		L"Evading the dreaded Imperial Starfleet, a group of freedom fighters led by Luke Skywalker has"
		L" established a new secret base on the remote ice world of Hoth.");

	m_pRandomEvents.at(2)->SetText(L"Turmoil has engulfed the Galactic Republic. "
		L"The taxation of trade routes to outlying star systems is in dispute."
		L"Hoping to resolve the matter with a blockade of deadly battleships, "
		L"the greedy Trade Federation has stopped all shipping to the small planet of Naboo.");
}

bool PlanetInteractionUI::Initialize() {
	if (!this->CreateScreen()) {
		return false;
	}
	if (!this->CreateTextElements()) {
		return false;
	}
	if (!this->CreateTools()) {
		return false;
	}
	return true;
}

PlanetInteractionUI::~PlanetInteractionUI() {
	//Release memory
	for (unsigned int i = 0; i < m_pRandomEvents.size(); i++) {
		delete m_pRandomEvents.at(i);
	}

	EventBuss::Get().RemoveListener(this, EventType::MouseButtonEvent);
	EventBuss::Get().RemoveListener(this, EventType::DelegatePlayerInfoEvent);
}

void PlanetInteractionUI::SetIcons(int randomEventIndex, Resources resources)
{
	if (resources.health != 0)
		m_pRandomEvents[randomEventIndex]->AddIcon(L"health.png", std::to_wstring(resources.health));
	if (resources.fuel != 0)
		m_pRandomEvents[randomEventIndex]->AddIcon(L"Fuel.png", std::to_wstring(resources.fuel));
	if (resources.oxygen != 0)
		m_pRandomEvents[randomEventIndex]->AddIcon(L"Oxygen.png", std::to_wstring(resources.oxygen));
	if (resources.titanium != 0)
		m_pRandomEvents[randomEventIndex]->AddIcon(L"Titanium.png", std::to_wstring(resources.titanium));
	if (resources.scrapMetal != 0)
		m_pRandomEvents[randomEventIndex]->AddIcon(L"Scrap.png", std::to_wstring(resources.scrapMetal));
	if (resources.nanotech != 0)
		m_pRandomEvents[randomEventIndex]->AddIcon(L"nanotech.png", std::to_wstring(resources.nanotech));
	if (resources.plasma != 0)
		m_pRandomEvents[randomEventIndex]->AddIcon(L"plasma.png", std::to_wstring(resources.plasma));
	if (resources.radium != 0)
		m_pRandomEvents[randomEventIndex]->AddIcon(L"radium.png", std::to_wstring(resources.radium));
	if (resources.khionerite != 0)
		m_pRandomEvents[randomEventIndex]->AddIcon(L"Khionerite.png", std::to_wstring(resources.khionerite));
}

//Creation functions
bool PlanetInteractionUI::CreateScreen() {
	//Main screen
	UpdateScreen();

	//Left corner
	ErrorCheck(m_pFactory2D->CreatePathGeometry(&m_pBottomLeft), "PathGeometry");
	ErrorCheck(m_pFactory2D->CreatePathGeometry(&m_pBottomLeftDetailsOne), "PathGeometry");
	ErrorCheck(m_pFactory2D->CreatePathGeometry(&m_pBottomLeftDetailsTwo), "PathGeometry");

	//Right corner
	ErrorCheck(m_pFactory2D->CreatePathGeometry(&m_pBottomRight), "PathGeometry");
	ErrorCheck(m_pFactory2D->CreatePathGeometry(&m_pBottomRightDetailsOne), "PathGeometry");
	ErrorCheck(m_pFactory2D->CreatePathGeometry(&m_pBottomRightDetailsTwo), "PathGeometry");

	//Top corners
	ErrorCheck(m_pFactory2D->CreatePathGeometry(&m_pTop), "PathGeometry");
	ErrorCheck(m_pFactory2D->CreatePathGeometry(&m_pTopDetailsLeft), "PathGeometry");
	ErrorCheck(m_pFactory2D->CreatePathGeometry(&m_pTopDetailsRight), "PathGeometry");

	return true;
}

bool PlanetInteractionUI::CreateTextElements() {
	//Planet name
	ErrorCheck(m_pTextFactory->GetSystemFontCollection(&m_pTitleFont, false), "GetSystemFont");

	ErrorCheck(m_pTextFactory->CreateTextFormat(
		L"Aware",
		NULL,
		DWRITE_FONT_WEIGHT_REGULAR,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		41.0f,
		L"en-us",
		&m_pTitleFormat
	), "TextFormat");
	ErrorCheck(m_pTitleFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER), "TextAlignment");

	//Screen text
	ErrorCheck(m_pTextFactory->GetSystemFontCollection(&m_pBodyFont, false), "GetSystemFont");

	ErrorCheck(m_pTextFactory->CreateTextFormat(
		L"Tenika",
		m_pBodyFont.Get(),
		DWRITE_FONT_WEIGHT_REGULAR,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		18.0f,
		L"en-us",
		&m_pBodyFormat
	), "TextFormat");

	for (unsigned int i = 0; i < m_pRandomEvents.size(); i++) {
		if (!m_pRandomEvents.at(i)->Initialize()) {
			return false;
		};
	}
	return true;
}

bool PlanetInteractionUI::CreateTools() {
	this->CreateBrush();

	//Main gradient
	D2D1_GRADIENT_STOP gradientStops[3];
	gradientStops[0].color = D2D1::ColorF(D2D1::ColorF::DarkSlateBlue, 0.25f);
	gradientStops[0].position = 0.0f;
	gradientStops[1].color = D2D1::ColorF(D2D1::ColorF::Aqua, 0.25f);
	gradientStops[1].position = 0.5f;
	gradientStops[2].color = D2D1::ColorF(D2D1::ColorF::DarkSlateBlue, 0.25f);
	gradientStops[2].position = 1.0f;

	ErrorCheck(m_pRenderTarget2D->CreateGradientStopCollection(
		gradientStops,
		3,
		D2D1_GAMMA_1_0,
		D2D1_EXTEND_MODE_CLAMP,
		&m_pMainGradientStops
	), "GradientStops");

	ErrorCheck(m_pRenderTarget2D->CreateLinearGradientBrush(
		D2D1::LinearGradientBrushProperties(
			D2D1::Point2F(m_pMainRectangle.left, m_pWindowHeight),
			D2D1::Point2F(m_pMainRectangle.right, m_pWindowHeight)),
		m_pMainGradientStops.Get(),
		&m_pMainGradientBrush),"GradientBrush");

	//Corner gradient
	gradientStops[0].color = D2D1::ColorF(D2D1::ColorF::Aqua, 0.25f);
	gradientStops[0].position = 0.0f;
	gradientStops[1].color = D2D1::ColorF(D2D1::ColorF::DarkSlateBlue, 0.25f);
	gradientStops[1].position = 0.5f;
	gradientStops[2].color = D2D1::ColorF(D2D1::ColorF::Aqua, 0.25f);
	gradientStops[2].position = 1.0f;

	ErrorCheck(m_pRenderTarget2D->CreateGradientStopCollection(
		gradientStops,
		3,
		D2D1_GAMMA_1_0,
		D2D1_EXTEND_MODE_CLAMP,
		&m_pCornerGradientStops
	), "GradientStops");

	ErrorCheck(m_pRenderTarget2D->CreateLinearGradientBrush(
		D2D1::LinearGradientBrushProperties(
			D2D1::Point2F(m_pMainRectangle.left, m_pWindowHeight),
			D2D1::Point2F(m_pMainRectangle.right, m_pWindowHeight)),
		m_pCornerGradientStops.Get(),
		&m_pCornerGradientBrush), "GradientBrush");
	return true;
}

//Update functions
bool PlanetInteractionUI::UpdateScreen() {
	m_pMainRectangle = D2D1::RectF(
		(m_pWindowWidth/2.0f) - 650.0f,
		(m_pWindowHeight / 2.0f) - 450.0f,
		(m_pWindowWidth/2.0f) + 650.0f,
		(m_pWindowHeight/2.0f) + 250.0f
	);

	//Module information
	m_pScreenOffset = 40.0f;
	m_pPadding = 10.0f;
	m_pBlockSize = ((m_pMainRectangle.bottom - m_pScreenOffset) - (m_pMainRectangle.top + m_pScreenOffset)) / 3.0f;
	return true;
}

bool PlanetInteractionUI::UpdateLeftCorner() {
	bool updated = false;

	//Set corner
	if (ErrorCheck(m_pBottomLeft->Open(&m_pSink), "OpenGeometry")) {
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

		updated = ErrorCheck(m_pSink->Close(), "CloseGeometry");
	}

	//Set corner details
	if (ErrorCheck(m_pBottomLeftDetailsOne->Open(&m_pSink), "OpenGeometry") && updated) {
		m_pSink->SetFillMode(D2D1_FILL_MODE_WINDING);

		m_pSink->BeginFigure(
			D2D1::Point2F(m_pMainRectangle.left + 70.0f, m_pMainRectangle.bottom),
			D2D1_FIGURE_BEGIN_FILLED
		);
		D2D1_POINT_2F points[] = {
			D2D1::Point2F(m_pMainRectangle.left + 140.0f, m_pMainRectangle.bottom),
		   D2D1::Point2F(m_pMainRectangle.left + 125.0f, m_pMainRectangle.bottom - 15.0f),
		   D2D1::Point2F(m_pMainRectangle.left + 55.0f, m_pMainRectangle.bottom - 15.0f),
		};
		m_pSink->AddLines(points, ARRAYSIZE(points));
		m_pSink->EndFigure(D2D1_FIGURE_END_CLOSED);

		updated = ErrorCheck(m_pSink->Close(), "CloseGeometry");
	}

	if (ErrorCheck(m_pBottomLeftDetailsTwo->Open(&m_pSink), "OpenGeometry") && updated) {
		m_pSink->SetFillMode(D2D1_FILL_MODE_WINDING);

		m_pSink->BeginFigure(
			D2D1::Point2F(m_pMainRectangle.left, m_pMainRectangle.bottom - 65.0f),
			D2D1_FIGURE_BEGIN_FILLED
		);
		D2D1_POINT_2F points[] = {
			D2D1::Point2F(m_pMainRectangle.left + 15.0f, m_pMainRectangle.bottom - 50.0f),
		   D2D1::Point2F(m_pMainRectangle.left + 15.0f, m_pMainRectangle.bottom - 110.0f),
		   D2D1::Point2F(m_pMainRectangle.left, m_pMainRectangle.bottom - 125.0f),
		};
		m_pSink->AddLines(points, ARRAYSIZE(points));
		m_pSink->EndFigure(D2D1_FIGURE_END_CLOSED);

		updated = ErrorCheck(m_pSink->Close(), "CloseGeometry");
	}
	return updated;
}

bool PlanetInteractionUI::UpdateRightCorner() {
	bool updated = false;

	//Set corner
	if (ErrorCheck(m_pBottomRight->Open(&m_pSink), "OpenGeometry")) {
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

		updated = ErrorCheck(m_pSink->Close(), "CloseGeometry");
	}

	//Set corner details
	if (ErrorCheck(m_pBottomRightDetailsOne->Open(&m_pSink), "OpenGeometry") && updated) {
		m_pSink->SetFillMode(D2D1_FILL_MODE_WINDING);

		m_pSink->BeginFigure(
			D2D1::Point2F(m_pMainRectangle.right - 70.0f, m_pMainRectangle.bottom),
			D2D1_FIGURE_BEGIN_FILLED
		);
		D2D1_POINT_2F points[] = {
			D2D1::Point2F(m_pMainRectangle.right - 140.0f, m_pMainRectangle.bottom),
			D2D1::Point2F(m_pMainRectangle.right - 125.0f, m_pMainRectangle.bottom - 15.0f),
			D2D1::Point2F(m_pMainRectangle.right - 55.0f, m_pMainRectangle.bottom - 15.0f),
		};
		m_pSink->AddLines(points, ARRAYSIZE(points));
		m_pSink->EndFigure(D2D1_FIGURE_END_CLOSED);

		updated = ErrorCheck(m_pSink->Close(), "CloseGeometry");
	}

	if (ErrorCheck(m_pBottomRightDetailsTwo->Open(&m_pSink), "OpenGeometry") && updated) {
		m_pSink->SetFillMode(D2D1_FILL_MODE_WINDING);

		m_pSink->BeginFigure(
			D2D1::Point2F(m_pMainRectangle.right, m_pMainRectangle.bottom - 65.0f),
			D2D1_FIGURE_BEGIN_FILLED
		);
		D2D1_POINT_2F points[] = {
			D2D1::Point2F(m_pMainRectangle.right - 15.0f, m_pMainRectangle.bottom - 50.0f),
			D2D1::Point2F(m_pMainRectangle.right - 15.0f, m_pMainRectangle.bottom - 110.0f),
			D2D1::Point2F(m_pMainRectangle.right, m_pMainRectangle.bottom - 125.0f),
		};
		m_pSink->AddLines(points, ARRAYSIZE(points));
		m_pSink->EndFigure(D2D1_FIGURE_END_CLOSED);

		updated = ErrorCheck(m_pSink->Close(), "CloseGeometry");
	}

	return updated;
}

bool PlanetInteractionUI::UpdateTopCorners() {
	bool updated = false;

	//Set top corners
	if (ErrorCheck(m_pTop->Open(&m_pSink), "OpenGeometry")) {
		m_pSink->SetFillMode(D2D1_FILL_MODE_WINDING);

		m_pSink->BeginFigure(
			D2D1::Point2F(m_pMainRectangle.right - 10.0f, m_pMainRectangle.top - 10.0f),
			D2D1_FIGURE_BEGIN_FILLED
		);
		D2D1_POINT_2F points[] = {
			//Title area
		   D2D1::Point2F(m_pMainRectangle.right - 380.0f, m_pMainRectangle.top - 10.0f),
		   D2D1::Point2F(m_pMainRectangle.right - 400.0f, m_pMainRectangle.top - 30.0f),

		   D2D1::Point2F(m_pMainRectangle.left + 400.0f, m_pMainRectangle.top - 30.0f),
		   D2D1::Point2F(m_pMainRectangle.left + 380.0f, m_pMainRectangle.top - 10.0f),

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

		updated = ErrorCheck(m_pSink->Close(), "CloseGeometry");
	}

	if (ErrorCheck(m_pTopDetailsLeft->Open(&m_pSink), "OpenGeometry") && updated) {
		m_pSink->SetFillMode(D2D1_FILL_MODE_WINDING);

		m_pSink->BeginFigure(
			D2D1::Point2F(m_pMainRectangle.left, m_pMainRectangle.top + 25.0f),
			D2D1_FIGURE_BEGIN_FILLED
		);
		D2D1_POINT_2F points[] = {
			D2D1::Point2F(m_pMainRectangle.left + 15.0f, m_pMainRectangle.top + 10.0f),
		   D2D1::Point2F(m_pMainRectangle.left + 15.0f, m_pMainRectangle.top + 55.0f),
		   D2D1::Point2F(m_pMainRectangle.left, m_pMainRectangle.top + 70.0f),
		};
		m_pSink->AddLines(points, ARRAYSIZE(points));
		m_pSink->EndFigure(D2D1_FIGURE_END_CLOSED);

		updated = ErrorCheck(m_pSink->Close(), "CloseGeometry");
	}


	if (ErrorCheck(m_pTopDetailsRight->Open(&m_pSink), "OpenGeometry") && updated) {
		m_pSink->SetFillMode(D2D1_FILL_MODE_WINDING);

		m_pSink->BeginFigure(
			D2D1::Point2F(m_pMainRectangle.right, m_pMainRectangle.top + 25.0f),
			D2D1_FIGURE_BEGIN_FILLED
		);
		D2D1_POINT_2F points[] = {
			D2D1::Point2F(m_pMainRectangle.right - 15.0f, m_pMainRectangle.top + 10.0f),
		   D2D1::Point2F(m_pMainRectangle.right - 15.0f, m_pMainRectangle.top + 55.0f),
		   D2D1::Point2F(m_pMainRectangle.right, m_pMainRectangle.top + 70.0f),
		};
		m_pSink->AddLines(points, ARRAYSIZE(points));
		m_pSink->EndFigure(D2D1_FIGURE_END_CLOSED);

		updated = ErrorCheck(m_pSink->Close(), "CloseGeometry");
	}

	return updated;
}

bool PlanetInteractionUI::UpdateTools() {
	m_pMainGradientBrush->SetStartPoint(D2D1::Point2F(m_pMainRectangle.left, m_pWindowHeight));
	m_pMainGradientBrush->SetEndPoint(D2D1::Point2F(m_pMainRectangle.right, m_pWindowHeight));
	m_pCornerGradientBrush->SetStartPoint(D2D1::Point2F(m_pMainRectangle.left, m_pWindowHeight));
	m_pCornerGradientBrush->SetEndPoint(D2D1::Point2F(m_pMainRectangle.right, m_pWindowHeight));
	return true;
}

bool PlanetInteractionUI::UpdateTextElements() {
	m_pPlanetNameTextBox = D2D1::RectF(
		m_pMainRectangle.left + 310.0f,
		m_pMainRectangle.top - 35.0f,
		m_pMainRectangle.right - 310.0f,
		m_pMainRectangle.top + 15.0f
	);

	m_pPlanetFlavourTextBox = D2D1::RectF(
		(m_pWindowWidth / 2.0f) + m_pScreenOffset - 30.0f,
		m_pMainRectangle.top + m_pScreenOffset,
		m_pMainRectangle.right - m_pScreenOffset,
		m_pMainRectangle.bottom - m_pScreenOffset
	);

	m_pRandomEvents.at(0)->SetHoverBox(D2D1::RectF(
		m_pMainRectangle.left + m_pScreenOffset,
		m_pMainRectangle.top + m_pScreenOffset,
		(m_pWindowWidth / 2.0f) - m_pPadding,
		m_pMainRectangle.top + m_pScreenOffset + m_pBlockSize - m_pPadding - (m_pPadding / 2.0f)
	), m_pPadding);

	m_pRandomEvents.at(2)->SetHoverBox(D2D1::RectF(
		m_pMainRectangle.left + m_pScreenOffset,
		m_pMainRectangle.bottom - m_pScreenOffset - m_pBlockSize + m_pPadding + (m_pPadding / 2.0f),
		(static_cast<float>(m_pWindowWidth) / 2.0f) - m_pPadding,
		m_pMainRectangle.bottom - m_pScreenOffset
	), m_pPadding);

	m_pRandomEvents.at(1)->SetHoverBox(D2D1::RectF(
		m_pMainRectangle.left + m_pScreenOffset,
		m_pMainRectangle.top + m_pScreenOffset + m_pBlockSize + m_pPadding,
		(static_cast<float>(m_pWindowWidth) / 2.0f) - m_pPadding,
		m_pMainRectangle.bottom - m_pScreenOffset - m_pBlockSize - m_pPadding
	), m_pPadding);

	return true;
}

bool PlanetInteractionUI::UpdateModules() {
	//Recreate the shapes based on new screen size
	if (!this->UpdateScreen()) {
		return false;
	}
	if (!this->UpdateLeftCorner()) {
		return false;
	}
	if (!this->UpdateRightCorner()) {
		return false;
	}
	if (!this->UpdateTopCorners()) {
		return false;
	}
	if (!this->UpdateTextElements()) {
		return false;
	}
	if (!this->UpdateTools()) {
		return false;
	}

	return true;
}

//Render functions
void PlanetInteractionUI::RenderScreen() {
	//Main square
	this->UpdateBrush(D2D1::ColorF::Aqua, 0.05f);
	m_pRenderTarget2D->FillRectangle(m_pMainRectangle, m_pBrush.Get());

	//Grid for the square
	unsigned int gridSize = 25;
	this->UpdateBrush(D2D1::ColorF::Aqua, 0.25f);
	for (float x = m_pMainRectangle.left; x < m_pMainRectangle.right; x += gridSize)
	{
		m_pRenderTarget2D->DrawLine(
			D2D1::Point2F(static_cast<FLOAT>(x), m_pMainRectangle.top),
			D2D1::Point2F(static_cast<FLOAT>(x), m_pMainRectangle.bottom),
			m_pBrush.Get(),
			0.5f
		);
	}

	for (float y = m_pMainRectangle.top; y < m_pMainRectangle.bottom; y += gridSize)
	{
		m_pRenderTarget2D->DrawLine(
			D2D1::Point2F(m_pMainRectangle.left, static_cast<FLOAT>(y)),
			D2D1::Point2F(m_pMainRectangle.right, static_cast<FLOAT>(y)),
			m_pBrush.Get(),
			0.5f
		);
	}

	//Use gradient brush to add effect
	m_pRenderTarget2D->FillRectangle(&m_pMainRectangle, m_pMainGradientBrush.Get());

	//Add outline to main square
	UpdateBrush(D2D1::ColorF::White, 0.5f);
	m_pRenderTarget2D->DrawRectangle(m_pMainRectangle, m_pBrush.Get());

	m_pRenderTarget2D->DrawLine(
		D2D1::Point2F(static_cast<FLOAT>(m_pWindowWidth) / 2.0f, m_pMainRectangle.top),
		D2D1::Point2F(static_cast<FLOAT>(m_pWindowWidth) / 2.0f, m_pMainRectangle.bottom),
		m_pBrush.Get(),
		1.0f
	);
}

void PlanetInteractionUI::RenderCorners() {
	//Left corner
	this->UpdateBrush(D2D1::ColorF::Teal, 1.0f);
	m_pRenderTarget2D->FillGeometry(m_pBottomLeft.Get(), m_pBrush.Get());
	m_pRenderTarget2D->FillGeometry(m_pBottomLeft.Get(), m_pCornerGradientBrush.Get());
	m_pRenderTarget2D->DrawGeometry(m_pBottomLeft.Get(), m_pBrush.Get(), 2.0f);

	this->UpdateBrush(D2D1::ColorF::Snow, 1.0f);
	this->UpdateBrush(D2D1::ColorF::Orange, 1.0f);
	m_pRenderTarget2D->FillGeometry(m_pBottomLeftDetailsOne.Get(), m_pBrush.Get());
	m_pRenderTarget2D->FillGeometry(m_pBottomLeftDetailsTwo.Get(), m_pBrush.Get());
	this->UpdateBrush(D2D1::ColorF::Orange, 1.0f);
	this->UpdateBrush(D2D1::ColorF::Snow, 1.0f);
	m_pRenderTarget2D->DrawGeometry(m_pBottomLeftDetailsOne.Get(), m_pBrush.Get(), 2.0f);
	m_pRenderTarget2D->DrawGeometry(m_pBottomLeftDetailsTwo.Get(), m_pBrush.Get(), 2.0f);

	m_pRenderTarget2D->DrawLine(
		D2D1::Point2F(m_pMainRectangle.left + 70.0f, m_pMainRectangle.bottom),
		D2D1::Point2F(m_pMainRectangle.left + 250.0f, m_pMainRectangle.bottom),
		m_pBrush.Get(),
		2.0f
	);

	m_pRenderTarget2D->DrawLine(
		D2D1::Point2F(m_pMainRectangle.left + 250.0f, m_pMainRectangle.bottom),
		D2D1::Point2F(m_pMainRectangle.left + 265.0f, m_pMainRectangle.bottom - 10.0f),
		m_pBrush.Get(),
		2.0f
	);

	m_pRenderTarget2D->DrawLine(
		D2D1::Point2F(m_pMainRectangle.left, m_pMainRectangle.bottom - 65.0f),
		D2D1::Point2F(m_pMainRectangle.left, m_pMainRectangle.bottom - 240.0f),
		m_pBrush.Get(),
		2.0f
	);

	m_pRenderTarget2D->DrawLine(
		D2D1::Point2F(m_pMainRectangle.left + 15.0f, m_pMainRectangle.bottom - 110.0f),
		D2D1::Point2F(m_pMainRectangle.left + 15.0f, m_pMainRectangle.bottom - 30.0f),
		m_pBrush.Get(),
		2.0f
	);

	m_pRenderTarget2D->DrawLine(
		D2D1::Point2F(m_pMainRectangle.left + 15.0f, m_pMainRectangle.bottom - 30.0f),
		D2D1::Point2F(m_pMainRectangle.left + 35.0f, m_pMainRectangle.bottom - 15.0f),
		m_pBrush.Get(),
		2.0f
	);

	m_pRenderTarget2D->DrawLine(
		D2D1::Point2F(m_pMainRectangle.left + 35.0f, m_pMainRectangle.bottom - 15.0f),
		D2D1::Point2F(m_pMainRectangle.left + 125.0f, m_pMainRectangle.bottom - 15.0f),
		m_pBrush.Get(),
		2.0f
	);

	//Right corner
	this->UpdateBrush(D2D1::ColorF::Teal, 1.0f);
	m_pRenderTarget2D->FillGeometry(m_pBottomRight.Get(), m_pBrush.Get());
	m_pRenderTarget2D->FillGeometry(m_pBottomRight.Get(), m_pCornerGradientBrush.Get());
	m_pRenderTarget2D->DrawGeometry(m_pBottomRight.Get(), m_pBrush.Get(), 2.0f);

	this->UpdateBrush(D2D1::ColorF::Orange, 1.0f);
	m_pRenderTarget2D->FillGeometry(m_pBottomRightDetailsOne.Get(), m_pBrush.Get());
	m_pRenderTarget2D->FillGeometry(m_pBottomRightDetailsTwo.Get(), m_pBrush.Get());
	this->UpdateBrush(D2D1::ColorF::Snow, 1.0f);
	m_pRenderTarget2D->DrawGeometry(m_pBottomRightDetailsOne.Get(), m_pBrush.Get(), 2.0f);
	m_pRenderTarget2D->DrawGeometry(m_pBottomRightDetailsTwo.Get(), m_pBrush.Get(), 2.0f);

	m_pRenderTarget2D->DrawLine(
		D2D1::Point2F(m_pMainRectangle.right - 70.0f, m_pMainRectangle.bottom),
		D2D1::Point2F(m_pMainRectangle.right - 250.0f, m_pMainRectangle.bottom),
		m_pBrush.Get(),
		2.0f
	);

	m_pRenderTarget2D->DrawLine(
		D2D1::Point2F(m_pMainRectangle.right - 250.0f, m_pMainRectangle.bottom),
		D2D1::Point2F(m_pMainRectangle.right - 265.0f, m_pMainRectangle.bottom - 10.0f),
		m_pBrush.Get(),
		2.0f
	);

	m_pRenderTarget2D->DrawLine(
		D2D1::Point2F(m_pMainRectangle.right, m_pMainRectangle.bottom - 65.0f),
		D2D1::Point2F(m_pMainRectangle.right, m_pMainRectangle.bottom - 240.0f),
		m_pBrush.Get(),
		2.0f
	);

	m_pRenderTarget2D->DrawLine(
		D2D1::Point2F(m_pMainRectangle.right - 15.0f, m_pMainRectangle.bottom - 110.0f),
		D2D1::Point2F(m_pMainRectangle.right - 15.0f, m_pMainRectangle.bottom - 30.0f),
		m_pBrush.Get(),
		2.0f
	);

	m_pRenderTarget2D->DrawLine(
		D2D1::Point2F(m_pMainRectangle.right - 15.0f, m_pMainRectangle.bottom - 30.0f),
		D2D1::Point2F(m_pMainRectangle.right - 35.0f, m_pMainRectangle.bottom - 15.0f),
		m_pBrush.Get(),
		2.0f
	);

	m_pRenderTarget2D->DrawLine(
		D2D1::Point2F(m_pMainRectangle.right - 35.0f, m_pMainRectangle.bottom - 15.0f),
		D2D1::Point2F(m_pMainRectangle.right - 125.0f, m_pMainRectangle.bottom - 15.0f),
		m_pBrush.Get(),
		2.0f
	);

	//Top corners
	this->UpdateBrush(D2D1::ColorF::Teal, 1.0f);
	m_pRenderTarget2D->FillGeometry(m_pTop.Get(), m_pBrush.Get());
	m_pRenderTarget2D->FillGeometry(m_pTop.Get(), m_pCornerGradientBrush.Get());
	m_pRenderTarget2D->DrawGeometry(m_pTop.Get(), m_pBrush.Get(), 2.0f);

	this->UpdateBrush(D2D1::ColorF::Orange, 1.0f);
	m_pRenderTarget2D->FillGeometry(m_pTopDetailsLeft.Get(), m_pBrush.Get());
	m_pRenderTarget2D->FillGeometry(m_pTopDetailsRight.Get(), m_pBrush.Get());
	this->UpdateBrush(D2D1::ColorF::Snow, 1.0f);
	m_pRenderTarget2D->DrawGeometry(m_pTopDetailsLeft.Get(), m_pBrush.Get(), 2.0f);
	m_pRenderTarget2D->DrawGeometry(m_pTopDetailsRight.Get(), m_pBrush.Get(), 2.0f);

	m_pRenderTarget2D->DrawLine(
		D2D1::Point2F(m_pMainRectangle.left + 15.0f, m_pMainRectangle.top + 9.0f),
		D2D1::Point2F(m_pMainRectangle.right - 15.0f, m_pMainRectangle.top + 9.0f),
		m_pBrush.Get(),
		2.0f
	);

	m_pRenderTarget2D->DrawLine(
		D2D1::Point2F(m_pMainRectangle.left + 15.0f, m_pMainRectangle.top + 55.0f),
		D2D1::Point2F(m_pMainRectangle.left - 10.0f, m_pMainRectangle.top + 79.0f),
		m_pBrush.Get(),
		2.0f
	);

	m_pRenderTarget2D->DrawLine(
		D2D1::Point2F(m_pMainRectangle.right - 15.0f, m_pMainRectangle.top + 55.0f),
		D2D1::Point2F(m_pMainRectangle.right + 10.0f, m_pMainRectangle.top + 79.0f),
		m_pBrush.Get(),
		2.0f
	);
}

void PlanetInteractionUI::RenderPlanetText() {
	this->UpdateBrush(D2D1::ColorF::Aqua, 0.05f);
	m_pRenderTarget2D->FillRectangle(m_pPlanetFlavourTextBox, m_pBrush.Get());

	//Planet name
	UpdateBrush(D2D1::ColorF::White, 1.0f);
	m_pRenderTarget2D.Get()->DrawTextW(
		m_pPlanetNameText.c_str(),
		(UINT32)m_pPlanetNameText.length(),
		m_pTitleFormat.Get(),
		m_pPlanetNameTextBox,
		m_pBrush.Get()
	);

	//Planet flavour text
	m_pRenderTarget2D.Get()->DrawTextW(
		m_pPlanetFlavourText.c_str(),
		(UINT32)m_pPlanetFlavourText.length(),
		m_pBodyFormat.Get(),
		m_pPlanetFlavourTextBox,
		m_pBrush.Get()
	);
}

void PlanetInteractionUI::RenderRandomEvents() {
	for (unsigned int i = 0; i < m_pRandomEvents.size(); i++) {
		m_pRandomEvents.at(i)->Render(m_pMouseX, m_pMouseY);
	}
}

void PlanetInteractionUI::Render() {
	this->BeginFrame();

	this->RenderScreen();
	this->RenderCorners();
	this->RenderPlanetText();
	this->RenderRandomEvents();

	this->EndFrame();
}

void PlanetInteractionUI::SetGameEvents(GameEvent gameEvents[3])
{
	for (int i = 0; i < 3; i++) {
		m_availableGameEvents[i] = gameEvents[i];
		m_pRandomEvents[i]->SetText(gameEvents[i].prologue);
	}
}

//Event functions
void PlanetInteractionUI::SetPlanetName(std::wstring text) {
	std::transform(text.begin(), text.end(), text.begin(), ::toupper);
	m_pPlanetNameText = text;
}

void PlanetInteractionUI::SetPlanetFlavourText(std::wstring text) {
	m_pPlanetFlavourText = text;
}

void PlanetInteractionUI::OnEvent(IEvent& event) noexcept {
	switch (event.GetEventType()) {
	case EventType::DelegateDXEvent:
	{
		UpdateDXHandlers(event);
		break;
	}
	//For hovering over UI elements
	case EventType::DelegateMouseCoordsEvent:
	{
		m_pMouseX = static_cast<DelegateMouseCoordsEvent*>(&event)->GetXCoord();
		m_pMouseY = static_cast<DelegateMouseCoordsEvent*>(&event)->GetYCoord();
		break;
	}
	//For clicking on UI elements
	case EventType::MouseButtonEvent:
	{
		m_pMouseX = static_cast<MouseButtonEvent*>(&event)->GetXCoord();
		m_pMouseY = static_cast<MouseButtonEvent*>(&event)->GetYCoord();
		KeyState state = static_cast<MouseButtonEvent*>(&event)->GetKeyState();
		int virKey = static_cast<MouseButtonEvent*>(&event)->GetVirtualKeyCode();
		if (virKey == VK_LBUTTON && state == KeyState::KeyPress && m_pOnScreen) {
			for (unsigned int i = 0; i < m_pRandomEvents.size(); i++) {
				m_pRandomEvents.at(i)->OnClick(m_pMouseX, m_pMouseY, m_availableGameEvents[i]);
				SetIcons(i, m_availableGameEvents[i].reward);
			}
		}
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
		m_pPlayerInfo = derivedEvent.GetPlayerInfo();
		SetPlanetName(m_pPlayerInfo->closestPlanet->GetName());
		break;
	}
	default:
		break;
	}
}

void PlanetInteractionUI::CleanUp() {
	for (unsigned int i = 0; i < m_pRandomEvents.size(); i++) {
		m_pRandomEvents.at(i)->CleanUp();
	}
}