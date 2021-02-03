#pragma once
#include "IEvent.h"
#include "KeyState.h"

class MouseButtenEvent : public IEvent
{
private:
	KeyState m_keyState;
	int m_xCoord, m_yCoord;
	int m_virtualKeyCode;

public:
	MouseButtenEvent(KeyState keyState, int virtualKeyCode, int xCoord, int yCoord)
	{
		m_keyState = keyState;
		m_virtualKeyCode = virtualKeyCode;
		m_xCoord = xCoord;
		m_yCoord = yCoord;
	}
	virtual ~MouseButtenEvent() noexcept = default;

	[[nodiscard]] const EventType GetEventType() const noexcept
	{
		return EventType::MouseButtenEvent;
	}

	[[nodiscard]] const std::string GetDebugName() const noexcept
	{
		return "MouseButtenEvent";
	}

	[[nodiscard]] const int GetXCoord() const noexcept
	{
		return m_xCoord;
	}

	[[nodiscard]] const int GetYCoord() const noexcept
	{
		return m_yCoord;
	}

	[[nodiscard]] const KeyState GetKeyState() const noexcept
	{
		return m_keyState;
	}

	[[nodiscard]] const int GetVirtualKeyCode() const noexcept
	{
		return m_virtualKeyCode;
	}

};