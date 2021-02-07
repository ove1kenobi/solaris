#pragma once
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