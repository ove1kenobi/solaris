#include "pch.h"
#include "Render2D.h"
//-----------

Render2D::Render2D() noexcept {
	//EventBuss::Get().AddListener(this, );
}

const bool Render2D::Initialize() noexcept {
	return true;
}

void Render2D::RenderUI(HWND hwnd) {
	/*The simplest way to add Direct2D content to a Direct3D scene,
	is to use the GetBuffer method of an IDXGISwapChain to obtain a DXGI surface, 
	then use the surface with the CreateDxgiSurfaceRenderTarget method to create an ID2D1RenderTarget.*/

	//Step 1: Create an ID2D1Factory
	ID2D1Factory* pD2DFactory = NULL;
	HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pD2DFactory);

	RECT rc;
	GetClientRect(hwnd, &rc);

	//Step 2: Create an ID2D1HwndRenderTarget
	/*version 1
	ID2D1HwndRenderTarget* pRT = NULL;
	hr = pD2DFactory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(
			hwnd,
			D2D1::SizeU(
				rc.right - rc.left,
				rc.bottom - rc.top)
		),
		&pRT
	);
	*/

	//------Version 2------------
    hr = m_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));

	FLOAT dpiX;
    FLOAT dpiY;
	pD2DFactory->GetDesktopDpi(&dpiX, &dpiY);

	D2D1_RENDER_TARGET_PROPERTIES props =
		D2D1::RenderTargetProperties(
			D2D1_RENDER_TARGET_TYPE_DEFAULT,
			D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED),
			dpiX,
			dpiY
		);

	hr = pD2DFactory->CreateDxgiSurfaceRenderTarget(
        pBackBuffer,
        &props,
        &m_pBackBufferRT
        );

	//--------------------------------


	//Step 3: Create a Brush
	ID2D1SolidColorBrush* pBlackBrush = NULL;
	if (SUCCEEDED(hr))
	{

		m_pBackBufferRT->CreateSolidColorBrush(
			D2D1::ColorF(D2D1::ColorF::Aqua),
			&pBlackBrush
		);
	}
	/*A brush can only be used with the render target that created it and with other render targets in the same resource domain. 
	In general, you should create brushes once and retain them for the life*/

	//Step 4: Draw the Rectangle
	m_pBackBufferRT->BeginDraw();

	m_pBackBufferRT->DrawRectangle(
		D2D1::RectF(
			rc.left + 100.0f,
			rc.top + 100.0f,
			rc.right - 100.0f,
			rc.bottom - 100.0f),
		pBlackBrush);

	hr = m_pBackBufferRT->EndDraw();

	//Step 5: Release Resources
	//pRT->Release();
	//pBlackBrush->Release();
	//pD2DFactory->Release();
}

//If an event has been picked up, then we delegate it here
void Render2D::OnEvent(IEvent& event) noexcept {
	//switch (event.GetEventType()) {
		//case: inPlanetInteraction	//Player pressed "interact"
		//case: inMenu	
		//case: inGame
		//case: inSettings
		//case: inControllerDisplay
		//case: inUpgrades
		//case: inPause
		//case: inMap
	//default:
		//break;
	//}
}

//Vad behövs för att få tag på hwnd?
//saved in class
//Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_pDeviceContext;

//used by function to update
//DelegateDXEvent& derivedEvent = static_cast<DelegateDXEvent&>(event);
//m_pDeviceContext = derivedEvent.GetDeviceContext();