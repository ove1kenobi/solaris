#pragma once
#include <unordered_map>
#include <vector>
#include <stdexcept>
#include <stdarg.h>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "IEvent.h"
#include "IEventListener.h"
class EventBuss
{
private:
	std::unordered_map<EventType, std::vector<IEventListener*>> m_Map;
	static EventBuss m_Instance;
private:
	EventBuss() = default;
	virtual ~EventBuss() noexcept = default;
public:
	[[nodiscard]] static EventBuss& Get() noexcept;
	void AddListener(IEventListener* eventListener, const EventType& eventType) noexcept;
	void AddListener(IEventListener* eventListener, const EventType& eventType, const EventType& eventType2) noexcept;
	void AddListener(IEventListener* eventListener, const EventType& eventType, const EventType& eventType2, const EventType& eventType3) noexcept;
	void AddListener(IEventListener* eventListener, const std::vector<EventType>& eventTypes) noexcept;
	void RemoveListener(IEventListener* eventListener, const EventType& eventType) noexcept;
	void Delegate(IEvent& event);
};