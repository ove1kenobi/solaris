#pragma once
#include "IEvent.h"
#include "../GameEvent.h"
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

class GameEventSelectedEvent : public IEvent {
private:
	GameEvent m_gameEvent;
public:
	GameEventSelectedEvent(GameEvent gameEvent) noexcept
	{
		m_gameEvent = gameEvent;
	}
	[[nodiscard]] const EventType GetEventType() const noexcept override
	{
		return EventType::GameEventSelectedEvent;
	}
	[[nodiscard]] const std::string GetDebugName() const noexcept override
	{
		return "GameEventSelectedEvent";
	}
	[[nodiscard]] const GameEvent GetGameEvent() const noexcept
	{
		return m_gameEvent;
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