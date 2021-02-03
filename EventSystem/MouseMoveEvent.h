#pragma once
#include "IEvent.h"

class MouseMoveEvent : public IEvent
{
private:
	int m_xCoord;
	int m_yCoord;

public:
	MouseMoveEvent(int xCoord, int yCoord)
	{
		m_xCoord = xCoord;
		m_yCoord = yCoord;
	}
	virtual ~MouseMoveEvent() noexcept = default;

	[[nodiscard]] const EventType GetEventType() const noexcept
	{
		return EventType::MouseMoveEvent;
	}

	[[nodiscard]] const std::string GetDebugName() const noexcept
	{
		return "MouseMoveEvent";
	}

	[[nodiscard]] const int GetXCoord() const noexcept
	{
		return m_xCoord;
	}

	[[nodiscard]] const int GetYCoord() const noexcept
	{
		return m_yCoord;
	}

};
