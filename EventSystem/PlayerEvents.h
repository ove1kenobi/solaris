#pragma once
#include "IEvent.h"
class ToggleTetheredEvent : public IEvent
{
private:

public:
	ToggleTetheredEvent() noexcept = default;
	virtual ~ToggleTetheredEvent() noexcept = default;

	[[nodiscard]] const EventType GetEventType() const noexcept override
	{
		return EventType::ToggleTetheredEvent;
	}
	[[nodiscard]] const std::string GetDebugName() const noexcept override
	{
		return "ToggleTetheredEvent";
	}
};

class GameOverEvent : public IEvent
{
private:
	bool m_playerWon;
public:
	GameOverEvent(bool playerWon) noexcept 
	{
		m_playerWon = playerWon;
	}
	virtual ~GameOverEvent() noexcept = default;

	[[nodiscard]] const EventType GetEventType() const noexcept override
	{
		return EventType::GameOverEvent;
	}
	[[nodiscard]] const std::string GetDebugName() const noexcept override
	{
		return "GameOverEvent";
	}
	[[nodiscard]] const bool GetPlayerWon() const noexcept
	{
		return m_playerWon;
	}
};