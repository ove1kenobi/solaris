#pragma once
#include "IEvent.h"

enum class MouseEventType {
	MouseMoved = 0,
	MouseButtenPress,
	MouseButtenRelease
};

class MouseEvent : public IEvent
{
private:
	MouseEventType m_type;
	int m_xCoord, m_yCoord;
	unsigned int m_virtualKeyCode;

public:
	MouseEvent(MouseEventType type, unsigned int virtualKeyCode, int xCoord, int yCoord)
	{
		m_xCoord = xCoord;
		m_yCoord = yCoord;
	}
	virtual ~MouseEvent() noexcept = default;

	[[nodiscard]] const EventType GetEventType() const noexcept
	{
		return EventType::MouseEvent;
	}

	[[nodiscard]] const std::string GetDebugName() const noexcept
	{
		return "MouseEvent";
	}

	[[nodiscard]] const int GetXCoord() const noexcept
	{
		return m_xCoord;
	}

	[[nodiscard]] const int GetYCoord() const noexcept
	{
		return m_yCoord;
	}

	[[nodiscard]] const unsigned int GetVirtualKeyCode() const noexcept
	{
		return m_virtualKeyCode;
	}

	[[nodiscard]] const MouseEventType GetMouseEventType() const noexcept
	{
		return m_type;
	}
};
