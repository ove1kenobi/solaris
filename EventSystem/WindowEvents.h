#pragma once
#include "IEvent.h"
class WindowCloseEvent : public IEvent
{
private:
	
public:
	WindowCloseEvent() noexcept = default;
	virtual ~WindowCloseEvent() noexcept = default;
	[[nodiscard]] const EventType GetEventType() const noexcept override
	{
		return EventType::WindowCloseEvent;
	}
	[[nodiscard]] const std::string GetDebugName() const noexcept override
	{
		return "WindowCloseEvent";
	}
};

class ToggleImGuiDemoWindowEvent : public IEvent
{
private:
public:
	ToggleImGuiDemoWindowEvent() noexcept = default;
	virtual ~ToggleImGuiDemoWindowEvent() noexcept = default;
	[[nodiscard]] const EventType GetEventType() const noexcept override
	{
		return EventType::ToggleImGuiDemoWindowEvent;
	}
	[[nodiscard]] const std::string GetDebugName() const noexcept override
	{
		return "ToggleImGuiDemoWindowEvent";
	}
};