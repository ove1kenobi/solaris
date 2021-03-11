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