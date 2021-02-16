#include "pch.h"
#include "Render2D.h"

Render2D::Render2D() noexcept 
{
	EventBuss::Get().AddListener(this, EventType::DelegateDXEvent);
}

const bool Render2D::Initialize() noexcept {
	if (!m_TestUI.Initialize()) {
		return false;
	}
	return true;
}

void Render2D::RenderUI() {
	m_TestUI.BeginFrame();

	m_TestUI.RenderHelpGrid(10);
	m_TestUI.RenderUI();

	m_TestUI.EndFrame();
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