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

class DelegateResolutionEvent : public IEvent 
{
private:
	unsigned int m_ClientWindowWidth;
	unsigned int m_ClientWindowHeight;
public:
	DelegateResolutionEvent(const unsigned int& clientWindowWidth, const unsigned& clientWindowHeight) noexcept
	{
		m_ClientWindowWidth = clientWindowWidth;
		m_ClientWindowHeight = clientWindowHeight;
	}
	[[nodiscard]] const EventType GetEventType() const noexcept override
	{
		return EventType::DelegateResolutionEvent;
	}
	[[nodiscard]] const std::string GetDebugName() const noexcept override
	{
		return "DelegateResolutionEvent";
	}
	[[nodiscard]] const unsigned int& GetClientWindowWidth() const noexcept
	{
		return m_ClientWindowWidth;
	}
	[[nodiscard]] const unsigned int& GetClientWindowHeight() const noexcept
	{
		return m_ClientWindowHeight;
	}
};