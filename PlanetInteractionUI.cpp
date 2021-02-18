#include "pch.h"
#include "PlanetInteractionUI.h"

PlanetInteractionUI::PlanetInteractionUI() noexcept {
	EventBuss::Get().AddListener(this, EventType::KeyboardEvent);			
	EventBuss::Get().AddListener(this, EventType::MouseButtonEvent);
	EventBuss::Get().AddListener(this, EventType::DelegateMouseCoordsEvent);

	m_pRenderHelpGrids = false;
	m_pRenderRandomEvents = false;

	m_pMainRectangle = D2D1::RectF();
	m_pPlanetNameTextBox = D2D1::RectF();
	m_pPlanetFlavourTextBox = D2D1::RectF();
	m_pEventOneTextBox = D2D1::RectF();
	m_pEventTwoTextBox = D2D1::RectF();
	m_pEventThreeTextBox = D2D1::RectF();

	//Example text: should be removed once event system is in place.
	m_pPlanetNameText = L"TATOOINE";

	m_pPlanetFlavourText = L"Luke Skywalker has returned to his home planet of Tatooine in an attempt "
		L"to rescue his friend Han Solo from the clutches of the vile gangster Jabba the Hutt."
		L" Little does Luke know that the GALACTIC EMPIRE has secretly begun construction on a new "
		L"armored space station even more powerful than the first dreaded Death Star."
		L"When completed, this ultimate weapon will spell certain doom for the small band "
		L"of rebels struggling to restore freedom to the galaxy...";

	m_pEventOneText = L"It is a period of civil war. Rebel spaceships, striking from a hidden base, "
		L"have won their first victory against the evil Galactic Empire.During the battle, "
		L"Rebel spies managed to steal secret plans to the Empire’s ultimate weapon, the DEATH STAR, "
		L"an armored space station with enough power to destroy an entire planet. ";

	m_pEventTwoText = L"It is a dark time for the Rebellion.Although the Death Star has been destroyed, "
		L"Imperial troops have driven the Rebel forces from their hidden baseand pursued them across the galaxy."
		L"Evading the dreaded Imperial Starfleet, a group of freedom fighters led by Luke Skywalker has"
		L" established a new secret base on the remote ice world of Hoth.";

	m_pEventThreeText = L"Turmoil has engulfed the Galactic Republic. "
		L"The taxation of trade routes to outlying star systems is in dispute."
		L"Hoping to resolve the matter with a blockade of deadly battleships, "
		L"the greedy Trade Federation has stopped all shipping to the small planet of Naboo.";
}

bool PlanetInteractionUI::CreateMainScreen() {
	//Create main rectangle
	m_pMainRectangle = D2D1::RectF(
		100.0f,
		50.0f,
		static_cast<float>(m_pWindowWidth) - 100.0f,
		static_cast<float>(m_pWindowHeight) - 200.0f
	);

	ID2D1GradientStopCollection* pGradientStops = NULL;

	D2D1_GRADIENT_STOP gradientStops[3];
	gradientStops[0].color = D2D1::ColorF(D2D1::ColorF::DarkSlateBlue, 0.25f);
	gradientStops[0].position = 0.0f;
	gradientStops[1].color = D2D1::ColorF(D2D1::ColorF::Aqua, 0.25f);
	gradientStops[1].position = 0.5f;
	gradientStops[2].color = D2D1::ColorF(D2D1::ColorF::DarkSlateBlue, 0.25f);
	gradientStops[2].position = 1.0f;

	// Create the ID2D1GradientStopCollection from a previously
// declared array of D2D1_GRADIENT_STOP structs.
	HRESULT hr = m_pRenderTarget2D->CreateGradientStopCollection(
		gradientStops,
		3,
		D2D1_GAMMA_2_2,
		D2D1_EXTEND_MODE_CLAMP,
		&pGradientStops
	);

	// The line that determines the direction of the gradient starts at
	// the upper-left corner of the square and ends at the lower-right corner.

	if (SUCCEEDED(hr))
	{
		hr = m_pRenderTarget2D->CreateLinearGradientBrush(
			D2D1::LinearGradientBrushProperties(
				D2D1::Point2F(m_pMainRectangle.left, static_cast<float>(m_pWindowHeight)),
				D2D1::Point2F(m_pMainRectangle.right, static_cast<float>(m_pWindowHeight))),
			pGradientStops,
			&m_pLinearGradientBrush
		);
	}

	//Create bottom left corner
	hr = m_pFactory2D->CreatePathGeometry(&m_pBottomLeft);
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
	hr = m_pFactory2D->CreatePathGeometry(&m_pBottomRight);
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
	hr = m_pFactory2D->CreatePathGeometry(&m_pTop);
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

bool PlanetInteractionUI::CreateTextElements() {
	HRESULT hr = m_pTextFactory->CreateTextFormat(
		L"Arial",					// Font family name.
		NULL,                       // Font collection (NULL sets it to use the system font collection).
		DWRITE_FONT_WEIGHT_REGULAR,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		48.0f,
		L"en-us",
		&m_pTitleTextFormat
	);
	if (FAILED(hr)) {
		printf("Error!\n");
		return false;
	}

	hr = m_pTitleTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	if (FAILED(hr)) {
		printf("Error!\n");
		return false;
	}

	m_pPlanetNameTextBox = D2D1::RectF(
		m_pMainRectangle.left + 300.0f,
		m_pMainRectangle.top - 30.0f,
		m_pMainRectangle.right - 300.0f,
		m_pMainRectangle.top + 30.0f
	);

	hr = m_pTextFactory->CreateTextFormat(
		L"Arial",					// Font family name.
		NULL,                       // Font collection (NULL sets it to use the system font collection).
		DWRITE_FONT_WEIGHT_REGULAR,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		16.0f,
		L"en-us",
		&m_pBodyTextFormat
	);
	if (FAILED(hr)) {
		printf("Error!\n");
		return false;
	}

	/*	hr = m_pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	if (FAILED(hr)) {
		printf("Error!\n");
		return false;
	}

	hr = m_pTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	if (FAILED(hr)) {
		printf("Error!\n");
		return false;
	}*/

	float screenOffset = 40.0f;
	float padding = 10.0f;
	float blockSize = ((m_pMainRectangle.bottom - screenOffset) - (m_pMainRectangle.top + screenOffset)) / 3.0f;

	m_pPlanetFlavourTextBox = D2D1::RectF(
		(static_cast<float>(m_pWindowWidth) / 2.0f) + screenOffset,
		m_pMainRectangle.top + screenOffset,
		m_pMainRectangle.right - screenOffset,
		m_pMainRectangle.bottom - screenOffset
	);

	m_pEventOneTextBox = D2D1::RectF(
		m_pMainRectangle.left + screenOffset,
		m_pMainRectangle.top + screenOffset,
		static_cast<float>(m_pWindowWidth) / 2.0f,
		m_pMainRectangle.top + screenOffset + blockSize - padding
	);

	m_pEventThreeTextBox = D2D1::RectF(
		m_pMainRectangle.left + screenOffset,
		m_pMainRectangle.bottom - screenOffset - blockSize + padding,
		static_cast<float>(m_pWindowWidth) / 2.0f,
		m_pMainRectangle.bottom - screenOffset
	);

	m_pEventTwoTextBox = D2D1::RectF(
		m_pMainRectangle.left + screenOffset,
		m_pEventOneTextBox.bottom + padding,
		static_cast<float>(m_pWindowWidth) / 2.0f,
		m_pEventThreeTextBox.top - padding
	);

	return true;
}

bool PlanetInteractionUI::CreateDetails()
{
	return true;
}

bool PlanetInteractionUI::UpdateModules() {
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

void PlanetInteractionUI::RenderRandomEvents() {
	//Event one
	m_pRenderTarget2D.Get()->DrawTextW(
		m_pEventOneText.c_str(),
		(UINT32)m_pEventOneText.length(),
		m_pBodyTextFormat.Get(),
		m_pEventOneTextBox,
		m_pBrush.Get()
	);

	//Event two
	m_pRenderTarget2D.Get()->DrawTextW(
		m_pEventTwoText.c_str(),
		(UINT32)m_pEventTwoText.length(),
		m_pBodyTextFormat.Get(),
		m_pEventTwoTextBox,
		m_pBrush.Get()
	);

	//Event three
	m_pRenderTarget2D.Get()->DrawTextW(
		m_pEventThreeText.c_str(),
		(UINT32)m_pEventThreeText.length(),
		m_pBodyTextFormat.Get(),
		m_pEventThreeTextBox,
		m_pBrush.Get()
	);
}

void PlanetInteractionUI::RenderHelpLines() {
	//Temp function only for debuging
	RenderHelpGrid(10);
	m_pRenderTarget2D->DrawRectangle(m_pPlanetNameTextBox, m_pBrush.Get());
	m_pRenderTarget2D->DrawRectangle(m_pPlanetFlavourTextBox, m_pBrush.Get());
	m_pRenderTarget2D->DrawRectangle(m_pEventOneTextBox, m_pBrush.Get());
	m_pRenderTarget2D->DrawRectangle(m_pEventTwoTextBox, m_pBrush.Get());
	m_pRenderTarget2D->DrawRectangle(m_pEventThreeTextBox, m_pBrush.Get());
}

void PlanetInteractionUI::RenderUI() {
	//Main screen
	this->UpdateBrush(D2D1::ColorF::Aqua, 0.05f);
	m_pRenderTarget2D->FillRectangle(m_pMainRectangle, m_pBrush.Get());

	unsigned int gridSize = 25;

	this->UpdateBrush(D2D1::ColorF::Aqua, 0.25f);
	for (unsigned int x = m_pMainRectangle.left; x < m_pMainRectangle.right; x += gridSize)
	{
		m_pRenderTarget2D->DrawLine(
			D2D1::Point2F(static_cast<FLOAT>(x), m_pMainRectangle.top),
			D2D1::Point2F(static_cast<FLOAT>(x), m_pMainRectangle.bottom),
			m_pBrush.Get(),
			0.5f
		);
	}

	for (unsigned int y = m_pMainRectangle.top; y < m_pMainRectangle.bottom; y += gridSize)
	{
		m_pRenderTarget2D->DrawLine(
			D2D1::Point2F(m_pMainRectangle.left, static_cast<FLOAT>(y)),
			D2D1::Point2F(m_pMainRectangle.right, static_cast<FLOAT>(y)),
			m_pBrush.Get(),
			0.5f
		);
	}

	m_pRenderTarget2D->FillRectangle(&m_pMainRectangle, m_pLinearGradientBrush.Get());

	UpdateBrush(D2D1::ColorF::White, 1.0f);
	m_pRenderTarget2D->DrawRectangle(m_pMainRectangle, m_pBrush.Get());

	//Corners of main screen
	this->UpdateBrush(D2D1::ColorF::Teal, 1.0f);
	m_pRenderTarget2D->FillGeometry(m_pBottomLeft.Get(), m_pBrush.Get());
	m_pRenderTarget2D->FillGeometry(m_pBottomRight.Get(), m_pBrush.Get());
	m_pRenderTarget2D->FillGeometry(m_pTop.Get(), m_pBrush.Get());


	//Planet text UI
	UpdateBrush(D2D1::ColorF::White, 1.0f);
	m_pRenderTarget2D.Get()->DrawTextW(
		m_pPlanetNameText.c_str(),
		(UINT32)m_pPlanetNameText.length(),
		m_pTitleTextFormat.Get(),
		m_pPlanetNameTextBox,
		m_pBrush.Get()
	);

	m_pRenderTarget2D.Get()->DrawTextW(
		m_pPlanetFlavourText.c_str(),
		(UINT32)m_pPlanetFlavourText.length(),
		m_pBodyTextFormat.Get(),
		m_pPlanetFlavourTextBox,
		m_pBrush.Get()
	);

	//Random event text UI
	if (m_pRenderRandomEvents) {
		this->RenderRandomEvents();
	}

	if (m_pRenderHelpGrids) {
		this->RenderHelpLines();
	}
}

void PlanetInteractionUI::SetPlanetName(std::wstring text) {
	m_pPlanetNameText = text;
}

void PlanetInteractionUI::SetPlanetFlavourText(std::wstring text) {
	m_pPlanetFlavourText = text;
}

void PlanetInteractionUI::SetEventOne(std::wstring text) {
	m_pEventOneText = text;
}

void PlanetInteractionUI::SetEventTwo(std::wstring text) {
	m_pEventTwoText = text;
}

void PlanetInteractionUI::SetEventThree(std::wstring text) {
	m_pEventThreeText = text;
}

bool PlanetInteractionUI::Initialize() {
	if (!ModuleUI::Initialize()) {
		return false;
	}
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

void PlanetInteractionUI::OnEvent(IEvent& event) noexcept {
	switch (event.GetEventType()) {
	case EventType::DelegateDXEvent:
	{
		UpdateDXHandlers(event);
		break;
	}
	case EventType::KeyboardEvent:
	{
		//TODO: make it possible to nagivate the choices with the keys too
		KeyState state = static_cast<KeyboardEvent*>(&event)->GetKeyState();
		int virKey = static_cast<KeyboardEvent*>(&event)->GetVirtualKeyCode();

		if (state == KeyState::KeyPress) {
			if (virKey == 'R') {
				if (m_pRenderRandomEvents) {
					m_pRenderRandomEvents = false;
				}
				else {
					m_pRenderRandomEvents = true;
				}
			}
			if (virKey == 'H') {
				if (m_pRenderHelpGrids) {
					m_pRenderHelpGrids = false;
				}
				else {
					m_pRenderHelpGrids = true;
				}
			}
		}
		break;
	}
	//For hovering over UI elements
	case EventType::DelegateMouseCoordsEvent:
	{
		int mouseX = static_cast<DelegateMouseCoordsEvent*>(&event)->GetXCoord();
		int mouseY = static_cast<DelegateMouseCoordsEvent*>(&event)->GetYCoord();
		this->SetPlanetName(std::to_wstring(mouseX));
		if (mouseX > m_pMainRectangle.left && mouseX < m_pMainRectangle.right &&
			mouseY > m_pMainRectangle.top && mouseY < m_pMainRectangle.bottom) {
			m_pRenderHelpGrids = true;
		}
		else {
			m_pRenderHelpGrids = false;
		}
		break;
	}
	//For clicking on UI elements
	case EventType::MouseButtonEvent:
	{
		int mouseX = static_cast<MouseButtonEvent*>(&event)->GetXCoord();
		int mouseY = static_cast<MouseButtonEvent*>(&event)->GetYCoord();
		KeyState state = static_cast<MouseButtonEvent*>(&event)->GetKeyState();
		int virKey = static_cast<MouseButtonEvent*>(&event)->GetVirtualKeyCode();
		if (virKey == VK_LBUTTON && state == KeyState::KeyPress) {
			this->SetPlanetName(std::to_wstring(mouseX));
			if (mouseX > m_pEventOneTextBox.left && mouseX < m_pEventOneTextBox.right &&
				mouseY > m_pMainRectangle.top && mouseY < m_pMainRectangle.bottom) {
				if (m_pRenderRandomEvents) {
					m_pRenderRandomEvents = false;
				}
				else {
					m_pRenderRandomEvents = true;
				}
			}
		}
		break;
	}
	case EventType::DelegateResolutionEvent:
	{
		m_pWindowWidth = static_cast<DelegateResolutionEvent*>(&event)->GetClientWindowWidth();
		m_pWindowHeight = static_cast<DelegateResolutionEvent*>(&event)->GetClientWindowHeight();
		this->UpdateModules();
		break;
	}
	default:
		break;
	}
}
