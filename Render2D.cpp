#include "pch.h"
#include "Render2D.h"

Render2D::Render2D() noexcept 
{
	EventBuss::Get().AddListener(this, EventType::DelegateDXEvent);
}

const bool Render2D::Initialize() noexcept {
	//Create all UI modules here
	return true;
}

void Render2D::RenderUI() {
	//ModuleUI->BeginFrame();

	//ModuleUI->RenderHelpGrid(10);
	//ModuleUI->RenderUI();

	//ModuleUI->EndFrame();
}

void Render2D::OnEvent(IEvent& event) noexcept {
	switch (event.GetEventType()) {
	/*case: EventType::inPlanetInteraction:	//Tells us it's time to switch state
		setState(PlanetInteaction);
	*/
	default:
		break;
	}

	//Other possible events/states
	//case: inMenu	
	//case: inGame
	//case: inSettings
	//case: inControllerDisplay
	//case: inUpgrades
	//case: inPause
	//case: inMap
}