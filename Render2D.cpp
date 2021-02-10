#include "pch.h"
#include "Render2D.h"

Render2D::Render2D() noexcept {

}

const bool Render2D::Initialize() noexcept {
	//What does here is based on what renderUI needs.
	return true;
}

void Render2D::RenderUI() {
	//Step 1: Create an ID2D1Factory
	ID2D1Factory* pD2DFactory = NULL;
	HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pD2DFactory);

	//Step 2: Create an ID2D1HwndRenderTarget
	//Step 3: Create a Brush
	//Step 4: Draw the Rectangle
	//Step 5: Release Resources
}

//If an event has been picked up, then we delegate it here
void Render2D::OnEvent(IEvent& event) noexcept {
	switch (event.GetEventType()) {
		//case: inPlanetInteraction	//Player pressed "interact"
		//case: inMenu	
		//case: inGame
		//case: inSettings
		//case: inControllerDisplay
		//case: inUpgrades
		//case: inPause
		//case: inMap
	default:
		break;
	}
}
