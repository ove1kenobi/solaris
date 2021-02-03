#include "EventBuss.h"

EventBuss EventBuss::m_Instance;

EventBuss& EventBuss::Get() noexcept
{
	return m_Instance;
}

void EventBuss::AddListener(IEventListener* eventListener, const EventType& eventType) noexcept
{
	std::unordered_map<EventType, std::vector<IEventListener*>>::iterator it = m_Map.find(eventType);
	if (it == m_Map.end())
	{
		//This EventType is not registered in the map at all, so we add it with its value:
		std::vector<IEventListener*> eventListeners;
		eventListeners.push_back(eventListener);
		m_Map.insert(std::make_pair(eventType, eventListeners));
	}
	else
	{
		//The EventType exists, so we retrieve the value (the vector) and add a listener:
		std::vector<IEventListener*>* eventListeners = static_cast<std::vector<IEventListener*>*>(&it->second);
		eventListeners->push_back(eventListener);
	}
}

void EventBuss::AddListener(IEventListener* eventListener, const EventType& eventType, const EventType& eventType2) noexcept
{
	std::unordered_map<EventType, std::vector<IEventListener*>>::iterator it = m_Map.find(eventType);
	if (it == m_Map.end())
	{
		//This EventType is not registered in the map at all, so we add it with its value:
		std::vector<IEventListener*> eventListeners;
		eventListeners.push_back(eventListener);
		m_Map.insert(std::make_pair(eventType, eventListeners));
	}
	else
	{
		//The EventType exists, so we retrieve the value (the vector) and add a listener:
		std::vector<IEventListener*>* eventListeners = static_cast<std::vector<IEventListener*>*>(&it->second);
		eventListeners->push_back(eventListener);
	}
	it = m_Map.find(eventType2);
	if (it == m_Map.end())
	{
		//This EventType is not registered in the map at all, so we add it with its value:
		std::vector<IEventListener*> eventListeners;
		eventListeners.push_back(eventListener);
		m_Map.insert(std::make_pair(eventType2, eventListeners));
	}
	else
	{
		//The EventType exists, so we retrieve the value (the vector) and add a listener:
		std::vector<IEventListener*>* eventListeners = static_cast<std::vector<IEventListener*>*>(&it->second);
		eventListeners->push_back(eventListener);
	}
}

void EventBuss::AddListener(IEventListener* eventListener, const EventType& eventType, const EventType& eventType2, const EventType& eventType3) noexcept
{
	std::unordered_map<EventType, std::vector<IEventListener*>>::iterator it = m_Map.find(eventType);
	if (it == m_Map.end())
	{
		//This EventType is not registered in the map at all, so we add it with its value:
		std::vector<IEventListener*> eventListeners;
		eventListeners.push_back(eventListener);
		m_Map.insert(std::make_pair(eventType, eventListeners));
	}
	else
	{
		//The EventType exists, so we retrieve the value (the vector) and add a listener:
		std::vector<IEventListener*>* eventListeners = static_cast<std::vector<IEventListener*>*>(&it->second);
		eventListeners->push_back(eventListener);
	}
	it = m_Map.find(eventType2);
	if (it == m_Map.end())
	{
		//This EventType is not registered in the map at all, so we add it with its value:
		std::vector<IEventListener*> eventListeners;
		eventListeners.push_back(eventListener);
		m_Map.insert(std::make_pair(eventType2, eventListeners));
	}
	else
	{
		//The EventType exists, so we retrieve the value (the vector) and add a listener:
		std::vector<IEventListener*>* eventListeners = static_cast<std::vector<IEventListener*>*>(&it->second);
		eventListeners->push_back(eventListener);
	}
	it = m_Map.find(eventType3);
	if (it == m_Map.end())
	{
		//This EventType is not registered in the map at all, so we add it with its value:
		std::vector<IEventListener*> eventListeners;
		eventListeners.push_back(eventListener);
		m_Map.insert(std::make_pair(eventType3, eventListeners));
	}
	else
	{
		//The EventType exists, so we retrieve the value (the vector) and add a listener:
		std::vector<IEventListener*>* eventListeners = static_cast<std::vector<IEventListener*>*>(&it->second);
		eventListeners->push_back(eventListener);
	}
}

void EventBuss::AddListener(IEventListener* eventListener, const std::vector<EventType>& eventTypes) noexcept
{
	for (unsigned int i { 0u }; i < eventTypes.size(); i++)
	{
		std::unordered_map<EventType, std::vector<IEventListener*>>::iterator it = m_Map.find(eventTypes[i]);
		if (it == m_Map.end())
		{
			//This EventType is not registered in the map at all, so we add it with its value:
			std::vector<IEventListener*> eventListeners;
			eventListeners.push_back(eventListener);
			m_Map.insert(std::make_pair(eventTypes[i], eventListeners));
		}
		else
		{
			//The EventType exists, so we retrieve the value (the vector) and add a listener:
			std::vector<IEventListener*>* eventListeners = static_cast<std::vector<IEventListener*>*>(&it->second);
			eventListeners->push_back(eventListener);
		}
	}
}

void EventBuss::RemoveListener(IEventListener* eventListener, const EventType& eventType) noexcept
{
	std::unordered_map<EventType, std::vector<IEventListener*>>::iterator it = m_Map.find(eventType);
	if (it != m_Map.end())
	{
		//We retrieve the vector holding all listeners for this event and try to find the actual listener:
		std::vector<IEventListener*>* eventListeners = static_cast<std::vector<IEventListener*>*>(&it->second);
		for (unsigned int i{ 0u }; i < eventListeners->size(); i++)
		{
			if (eventListeners->at(i) == eventListener)
			{
					eventListeners->erase(eventListeners->begin() + i);
			}
		}
	}
}

void EventBuss::Delegate(IEvent& event)
{
	/*NOTE: IF DONE CORRECTLY, THIS SHOULD NEVER THROW AN EXCEPTION!*/
	std::vector<IEventListener*>* eventListeners = nullptr;
	try
	{
		eventListeners = &(m_Map.at(event.GetEventType()));
	}
	catch (const std::out_of_range& outOfRange)
	{
		// No EventListeners are listening for this EventType:
		OutputDebugStringA(outOfRange.what());
		return;
	}
	//There are Eventlistener(s) listening for this event. Delegate it to them:
	for (unsigned int i{ 0u }; i < eventListeners->size(); i++)
	{
		if (!event.IsHandled())
			(*eventListeners)[i]->OnEvent(event);
	}
}
