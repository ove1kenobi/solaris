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
/*FOR TESTING PURPOSES ONLY*/
class TestEvent : public IEvent
{
private:

public:
	TestEvent() noexcept = default;
	virtual ~TestEvent() noexcept = default;
	[[nodiscard]] const EventType GetEventType() const noexcept override
	{
		return EventType::TestEvent;
	}
	[[nodiscard]] const std::string GetDebugName() const noexcept override
	{
		return "TestEvent";
	}
};

class WindowResizeEvent : public IEvent
{
private:
	unsigned int m_Width;
	unsigned int m_Height;
public:
	WindowResizeEvent(const unsigned int& width, const unsigned int& height) noexcept
	{
		m_Width = width;
		m_Height = height;
	}
	virtual ~WindowResizeEvent() noexcept = default;
	[[nodiscard]] const EventType GetEventType() const noexcept override
	{
		return EventType::WindowResizeEvent;
	}
	[[nodiscard]] const std::string GetDebugName() const noexcept override
	{
		return "WindowResizeEvent";
	}
	[[nodiscard]] const unsigned int& GetWidth() const noexcept
	{
		return m_Width;
	}
	[[nodiscard]] const unsigned int& GetHeight() const noexcept
	{
		return m_Height;
	}
};