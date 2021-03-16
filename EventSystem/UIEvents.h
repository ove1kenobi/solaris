#pragma once
#include "IEvent.h"
#include "../pch.h"

class DelegatePlanetDistanceEvent : public IEvent {
private:
	std::wstring m_PlanetName;
	float m_DistanceToObject;
public:
	DelegatePlanetDistanceEvent(float distanceToObject, std::wstring planetName) noexcept
	{
		m_DistanceToObject = distanceToObject;
		m_PlanetName = planetName;
	}
	virtual ~DelegatePlanetDistanceEvent() noexcept = default;
	[[nodiscard]] const EventType GetEventType() const noexcept override
	{
		return EventType::DelegatePlanetDistanceEvent;
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

class ToggleStartGame : public IEvent
{
private:

public:
	ToggleStartGame() noexcept = default;
	virtual ~ToggleStartGame() noexcept = default;

	[[nodiscard]] const EventType GetEventType() const noexcept override
	{
		return EventType::ToggleStartGame;
	}
	[[nodiscard]] const std::string GetDebugName() const noexcept override
	{
		return "ToggleStartGame";
	}
};


/*
What the UI will need to listen after or send out requests for:
- player health
- player O2
- player fuel
- what amount of resources the player currently have
- what three random events (text, what resources, what recourse amount)
- what event the player selected (send back an ID?)
*/