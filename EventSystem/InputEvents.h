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

class MouseMoveAbsoluteEvent : public IEvent
{
private:
	int m_xCoord, m_yCoord;
public:
	MouseMoveAbsoluteEvent(int xCoord, int yCoord)
	{
		m_xCoord = xCoord;
		m_yCoord = yCoord;
	}
	virtual ~MouseMoveAbsoluteEvent() noexcept = default;

	[[nodiscard]] const EventType GetEventType() const noexcept
	{
		return EventType::MouseMoveAbsoluteEvent;
	}
	[[nodiscard]] const std::string GetDebugName() const noexcept
	{
		return "MouseMoveAbsoluteEvent";
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

class MouseMoveRelativeEvent : public IEvent
{
private:
	int m_xDiff, m_yDiff;
public:
	MouseMoveRelativeEvent(int xDiff, int yDiff)
	{
		m_xDiff = xDiff;
		m_yDiff = yDiff;
	}
	virtual ~MouseMoveRelativeEvent() noexcept = default;

	[[nodiscard]] const EventType GetEventType() const noexcept
	{
		return EventType::MouseMoveRelativeEvent;
	}
	[[nodiscard]] const std::string GetDebugName() const noexcept
	{
		return "MouseMoveRelativeEvent";
	}
	[[nodiscard]] const int GetXDiff() const noexcept
	{
		return m_xDiff;
	}
	[[nodiscard]] const int GetYDiff() const noexcept
	{
		return m_yDiff;
	}
};

class MouseScrollEvent : public IEvent
{
private:
	int m_wheelScroll;
public:
	MouseScrollEvent(int wheelScroll) 
	{
		m_wheelScroll = wheelScroll;
	}
	virtual ~MouseScrollEvent() noexcept = default;

	[[nodiscard]] const EventType GetEventType() const noexcept
	{
		return EventType::MouseScrollEvent;
	}
	[[nodiscard]] const std::string GetDebugName() const noexcept
	{
		return "MouseScrollEvent";
	}
	[[nodiscard]] const int GetWheelScroll() const noexcept
	{
		return m_wheelScroll;
	}
};