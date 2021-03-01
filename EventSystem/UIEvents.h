#pragma once
#include "IEvent.h"
#include "../pch.h"

class DelegatePlanetDistanceEvent : public IEvent {
private:
	std::wstring m_PlanetName;
	float m_DistanceToObject;
public:
	DelegatePlanetDistanceEvent(float& distanceToObject, std::wstring planetName) noexcept
	{
		m_DistanceToObject = distanceToObject;
		m_PlanetName = planetName;
	}
	[[nodiscard]] const EventType GetEventType() const noexcept override
	{
		return EventType::DelegateResolutionEvent;
	}
	[[nodiscard]] const std::string GetDebugName() const noexcept override
	{
		return "DelegatePlanetDistanceEvent";
	}
	[[nodiscard]] const std::wstring GetPlanetName() const noexcept
	{
		return m_PlanetName;
	}
	[[nodiscard]] const float GetDistanceToObject() const noexcept
	{
		return m_DistanceToObject;
	}
};


/*
What the UI will need to listen after or send out requests for:
- distance to planet
if player is in the HUD UI and clicks on the screen, then the HUD want's to request the distance to the game object the player clicked on.
The MousePicking shall recive the coordinates the player clicked on to check if there is a planet there or not.
- player health
- player O2
- player fuel
- rendertarget for top down camera
- what amount of resources the player currently have
- what three random events (text, what resources, what recourse amount)
- what event the player selected (send back an ID?)
*/