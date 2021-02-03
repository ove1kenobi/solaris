#pragma once
#include "IEvent.h"
#include "KeyState.h"

class KeyboardEvent : public IEvent
{
private:
	KeyState m_keyState;
	int m_virtualKeyCode;

public:
	KeyboardEvent(KeyState keyState, int virtualKeyCode)
	{
		m_keyState = keyState;
		m_virtualKeyCode = virtualKeyCode;
	}
	virtual ~KeyboardEvent() noexcept = default;

	[[nodiscard]] const EventType GetEventType() const noexcept
	{
		return EventType::KeyboardEvent;
	}

	[[nodiscard]] const std::string GetDebugName() const noexcept
	{
		return "KeyboardEvent";
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
