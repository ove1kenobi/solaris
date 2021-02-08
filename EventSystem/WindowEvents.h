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

class ToggleImGuiEvent : public IEvent
{
private:
public:
	ToggleImGuiEvent() noexcept = default;
	virtual ~ToggleImGuiEvent() noexcept = default;
	[[nodiscard]] const EventType GetEventType() const noexcept override
	{
		return EventType::ToggleImGuiEvent;
	}
	[[nodiscard]] const std::string GetDebugName() const noexcept override
	{
		return "ToggleImGuiEvent";
	}
};