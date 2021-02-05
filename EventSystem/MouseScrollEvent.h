#pragma once
#include "IEvent.h"

class MouseScrollEvent : public IEvent
{
private:

public:
	MouseScrollEvent() noexcept = default;
	virtual ~MouseScrollEvent() noexcept = default;

	[[nodiscard]] const EventType GetEventType() const noexcept
	{
		return EventType::MouseScrollEvent;
	}

	[[nodiscard]] const std::string GetDebugName() const noexcept
	{
		return "MouseScrollEvent";
	}
};
