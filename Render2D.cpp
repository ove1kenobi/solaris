#include "pch.h"
#include "Render2D.h"
//------ONLY FOR TESTING-----
#include "RenderWindow.h"
//-----------

Render2D::Render2D() noexcept {
	//EventBuss::Get().AddListener(this, );
}

const bool Render2D::Initialize() noexcept {
	//What does here is based on what renderUI needs.
	return true;
}

void Render2D::RenderUI(HWND hwnd) {
	/*The simplest way to add Direct2D content to a Direct3D scene,
	is to use the GetBuffer method of an IDXGISwapChain to obtain a DXGI surface, 
	then use the surface with the CreateDxgiSurfaceRenderTarget method to create an ID2D1RenderTarget,
	with which to draw your 2-D content.*/
	//TODO: move factory to the DXCore
	//TODO: move RenderTarget to DXCore
	//TODO: get hwnd from the window class the correct way
	//TODO:

	//Step 1: Create an ID2D1Factory
	ID2D1Factory* pD2DFactory = NULL;
	HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pD2DFactory);


	//Step 2: Create an ID2D1HwndRenderTarget (WHAT WE NEED TO CHANGE)
	// Obtain the size of the drawing area.
	RECT rc;
	GetClientRect(hwnd, &rc);

	/*// Get a surface in the swap chain
    hr = m_pSwapChain->GetBuffer(
        0,
        IID_PPV_ARGS(&pBackBuffer)
        );*/

	/*hr = m_pD2DFactory->CreateDxgiSurfaceRenderTarget(
        pBackBuffer,
        &props,
        &m_pBackBufferRT
        );*/

	// Create a Direct2D render target          
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


	//Step 3: Create a Brush
	ID2D1SolidColorBrush* pBlackBrush = NULL;
	if (SUCCEEDED(hr))
	{

		pRT->CreateSolidColorBrush(
			D2D1::ColorF(D2D1::ColorF::Aqua),
			&pBlackBrush
		);
	}
	/*A brush can only be used with the render target that created it and with other render targets in the same resource domain. 
	In general, you should create brushes once and retain them for the life*/

	//Step 4: Draw the Rectangle
	pRT->BeginDraw();

	pRT->DrawRectangle(
		D2D1::RectF(
			rc.left + 100.0f,
			rc.top + 100.0f,
			rc.right - 100.0f,
			rc.bottom - 100.0f),
		pBlackBrush);

	hr = pRT->EndDraw();

	//Step 5: Release Resources
	pRT->Release();
	pBlackBrush->Release();
	pD2DFactory->Release();
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