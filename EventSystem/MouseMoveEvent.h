#pragma once
#include "IEvent.h"

class MouseMoveEvent : public IEvent
{
private:

public:
	MouseMoveEvent() noexcept = default;
	virtual ~MouseMoveEvent() noexcept = default;

	[[nodiscard]] const EventType GetEventType() const noexcept
	{
		return EventType::MouseMoveEvent;
	}

	[[nodiscard]] const std::string GetDebugName() const noexcept
	{
		return "MouseMoveEvent";
	}
};
