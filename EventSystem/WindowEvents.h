#pragma once
#include "IEvent.h"
class WindowCloseEvent : public IEvent
{
private:
	
public:
	WindowCloseEvent() noexcept = default;
	virtual ~WindowCloseEvent() noexcept = default;
	[[nodiscard]] const EventType GetEventType() const noexcept
	{
		return EventType::WindowCloseEvent;
	}
	[[nodiscard]] const std::string GetDebugName() const noexcept
	{
		return "WindowCloseEvent";
	}
};

class TestEvent : public IEvent
{
private:

public:
	TestEvent() noexcept = default;
	virtual ~TestEvent() noexcept = default;
	[[nodiscard]] const EventType GetEventType() const noexcept
	{
		return EventType::TestEvent;
	}
	[[nodiscard]] const std::string GetDebugName() const noexcept
	{
		return "TestEvent";
	}
};