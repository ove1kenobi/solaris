#pragma once
#include "IEvent.h"
#include "../GameObject.h"
#include <vector>

class SendRenderObjectsEvent : public IEvent
{
private:
	std::vector<GameObject*> &m_gameObjects;

public:
	SendRenderObjectsEvent(std::vector<GameObject*> &gameObjects) noexcept
	{
		this->m_gameObjects = *gameObjects;
	};
	virtual ~SendRenderObjectsEvent() noexcept = default;

	[[nodiscard]] const EventType GetEventType() const noexcept
	{
		return EventType::SendRenderObjectsEvent;
	}
	[[nodiscard]] const std::string GetDebugName() const noexcept
	{
		return "SendRenderObjectsEvent";
	}

	std::vector<GameObject*> &getGameObjectVector() {
		return this->m_gameObjects;
	}
};

class AskForRenderObjectsEvent : public IEvent
{
private:

public:
	AskForRenderObjectsEvent() noexcept = default;
	virtual ~AskForRenderObjectsEvent() noexcept = default;

	[[nodiscard]] const EventType GetEventType() const noexcept
	{
		return EventType::AskForRenderObjectsEvent;
	}
	[[nodiscard]] const std::string GetDebugName() const noexcept
	{
		return "AskForRenderObjectsEvent";
	}
};