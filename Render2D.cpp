#include "pch.h"
#include "Render2D.h"
//-----------

//Make Render2D into a listener for events
Render2D::Render2D() noexcept {
	EventBuss::Get().AddListener(this, EventType::DelegateDXEvent);
}

const bool Render2D::Initialize() noexcept {
	return true;
}

void Render2D::UpdateDXHandlers(IEvent& event) noexcept {
	DelegateDXEvent& derivedEvent = static_cast<DelegateDXEvent&>(event);
	this->m_pBackBuffer = derivedEvent.GetBackBuffer();
	this->m_pSwapChain = derivedEvent.GetSwapChain();

#if defined(DEBUG) | defined(_DEBUG)
	assert(this->m_pBackBuffer);
#endif
}

void Render2D::RenderUI(HWND hwnd) {
	//Create factory TODO: needs to be moved to DXCore
	ID2D1Factory* pD2DFactory = NULL;
	HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pD2DFactory);
	if (FAILED(hr))	//S_OK
	{
		printf("Error!\n");
	}

	D2D1_RENDER_TARGET_PROPERTIES props =
		D2D1::RenderTargetProperties(
			D2D1_RENDER_TARGET_TYPE_DEFAULT,
			D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED)
		);

	// Get swap chain surface
	Microsoft::WRL::ComPtr<IDXGISurface> surface;
	//hr = this->m_pSwapChain->GetBuffer(0, __uuidof(IDXGISurface), static_cast<void**>(&surface));
	hr = m_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&surface));
	if (FAILED(hr))	//S_OK
	{
		printf("Error!\n");
	}

	//one or more arguments here are invalid
	Microsoft::WRL::ComPtr<ID2D1RenderTarget> g_renderTarget2D = NULL;
	hr = pD2DFactory->CreateDxgiSurfaceRenderTarget(
		surface.Get(),
		&props,
		&g_renderTarget2D
	);
	if (FAILED(hr))	//E_INVALIDARG One or more arguments are invalid
	{
		printf("Error!\n");
	}

	if (g_renderTarget2D) {
		//Create brush that will be used for rendering
		ID2D1SolidColorBrush* pBrush = NULL;
		hr = g_renderTarget2D->CreateSolidColorBrush(
			D2D1::ColorF(D2D1::ColorF::Aqua),
			&pBrush
		);
		if (FAILED(hr))
		{
			printf("Error!\n");
		}

		//Get screen size for screen position, used for drawing TODO: might not be needed depending on UI implementation
		RECT rc;
		GetClientRect(hwnd, &rc);

		//Draw UI using brush
		g_renderTarget2D->BeginDraw();

		g_renderTarget2D->DrawRectangle(
			D2D1::RectF(
				rc.left + 100.0f,
				rc.top + 100.0f,
				rc.right - 100.0f,
				rc.bottom - 100.0f),
			pBrush);

		hr = g_renderTarget2D->EndDraw();
		if (FAILED(hr))
		{
			printf("Error!\n");
		}
	}

	//TODO: do NOT forget to release pointers
	//pRT->Release();
	//pBlackBrush->Release();
	//pD2DFactory->Release();
}

//If an event has been picked up, then we delegate it here
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