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

class ToggleControls : public IEvent
{
private:

public:
	ToggleControls() noexcept = default;
	virtual ~ToggleControls() noexcept = default;

	[[nodiscard]] const EventType GetEventType() const noexcept override
	{
		return EventType::ToggleControls;
	}
	[[nodiscard]] const std::string GetDebugName() const noexcept override
	{
		return "ToggleControls";
	}
};

class ToggleCredits : public IEvent
{
private:

public:
	ToggleCredits() noexcept = default;
	virtual ~ToggleCredits() noexcept = default;

	[[nodiscard]] const EventType GetEventType() const noexcept override
	{
		return EventType::ToggleCredits;
	}
	[[nodiscard]] const std::string GetDebugName() const noexcept override
	{
		return "ToggleCredits";
	}
};

class TogglePressToWinGame : public IEvent
{
private:

public:
	TogglePressToWinGame() noexcept = default;
	virtual ~TogglePressToWinGame() noexcept = default;

	[[nodiscard]] const EventType GetEventType() const noexcept override
	{
		return EventType::TogglePressToWinGame;
	}
	[[nodiscard]] const std::string GetDebugName() const noexcept override
	{
		return "TogglePressToWinGame";
	}
};

class ToggleDamageHUD : public IEvent
{
private:
	bool m_coldDamage;
	bool m_heatDamage;
	bool m_radioactiveDamage;
public:
	ToggleDamageHUD(bool coldDamage, bool heatDamage, bool radioactiveDamage) noexcept
	{
		m_coldDamage = coldDamage;
		m_heatDamage = heatDamage;
		m_radioactiveDamage = radioactiveDamage;
	}
	virtual ~ToggleDamageHUD() noexcept = default;

	[[nodiscard]] const EventType GetEventType() const noexcept override
	{
		return EventType::ToggleDamageHUD;
	}
	[[nodiscard]] const std::string GetDebugName() const noexcept override
	{
		return "ToggleDamageHUD";
	}
	[[nodiscard]] const bool GetCold() const noexcept
	{
		return m_coldDamage;
	}
	[[nodiscard]] const bool GetHeat() const noexcept
	{
		return m_heatDamage;
	}
	[[nodiscard]] const bool GetRadiation() const noexcept
	{
		return m_radioactiveDamage;
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

class DelegateUpgradeID : public IEvent
{
private:
	Resources upgradeCost;
	unsigned int upgradeID;
public:
	DelegateUpgradeID(unsigned int ID, Resources cost) noexcept {
		upgradeID = ID;
		upgradeCost = cost;
	};
	virtual ~DelegateUpgradeID() noexcept = default;

	[[nodiscard]] const EventType GetEventType() const noexcept override
	{
		return EventType::DelegateUpgradeID;
	}
	[[nodiscard]] const std::string GetDebugName() const noexcept override
	{
		return "DelegateUpgradeID";
	}
	[[nodiscard]] const int GetID() const noexcept
	{
		return upgradeID;
	}
	[[nodiscard]] const Resources GetCost() const noexcept
	{
		return upgradeCost;
	}
};

class DisplayEndgameText : public IEvent
{
private:
	std::wstring m_endgameText;
public:
	DisplayEndgameText(std::wstring endgameText) noexcept {
		m_endgameText = endgameText;
	};
	virtual ~DisplayEndgameText() noexcept = default;

	[[nodiscard]] const EventType GetEventType() const noexcept override
	{
		return EventType::DisplayEndgameText;
	}
	[[nodiscard]] const std::string GetDebugName() const noexcept override
	{
		return "DisplayEndgameText";
	}
	[[nodiscard]] const std::wstring GetEndgameText() const noexcept
	{
		return m_endgameText;
	}
};