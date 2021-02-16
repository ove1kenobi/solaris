#pragma once
#include "IEvent.h"
//#include "../GameObject.h"
#include "../Resources/BindIDs.h"

class GameObject;

class SendRenderObjectsEvent : public IEvent
{
private:
	std::vector<GameObject*>* m_gameObjects;
	size_t m_numPlanets;
public:
	SendRenderObjectsEvent(std::vector<GameObject*> *gameObjects, const size_t& numPlanets) noexcept
	{
		m_gameObjects = gameObjects;
		m_numPlanets = numPlanets;
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
	std::vector<GameObject*> *getGameObjectVector() {
		return this->m_gameObjects;
	}
	const size_t& GetNumPlanets()
	{
		return m_numPlanets;
	}
};

class AskForRenderObjectsEvent : public IEvent
{
private:

public:
	AskForRenderObjectsEvent() noexcept = default;
	virtual ~AskForRenderObjectsEvent() noexcept = default;

	[[nodiscard]] const EventType GetEventType() const noexcept override
	{
		return EventType::AskForRenderObjectsEvent;
	}
	[[nodiscard]] const std::string GetDebugName() const noexcept override
	{
		return "AskForRenderObjectsEvent";
	}
};

class UnbindPipelineEvent : public IEvent
{
private:

public:
	UnbindPipelineEvent() noexcept = default;
	virtual ~UnbindPipelineEvent() noexcept = default;

	[[nodiscard]] const EventType GetEventType() const noexcept override
	{
		return EventType::UnbindPipelineEvent;
	}
	[[nodiscard]] const std::string GetDebugName() const noexcept override
	{
		return "UnbindPipelineEvent";
	}
};

class ToggleWireFrameEvent : public IEvent
{
private:

public:
	ToggleWireFrameEvent() noexcept = default;
	virtual ~ToggleWireFrameEvent() noexcept = default;

	[[nodiscard]] const EventType GetEventType() const noexcept override
	{
		return EventType::ToggleWireFrameEvent;
	}
	[[nodiscard]] const std::string GetDebugName() const noexcept override
	{
		return "ToggleWireFrameEvent";
	}
};

class BindIDEvent : public IEvent
{
private:
	BindID m_BindID;
public:
	BindIDEvent(const BindID& bindID)
	{
		m_BindID = bindID;
	}
	virtual ~BindIDEvent() noexcept = default;

	[[nodiscard]] const EventType GetEventType() const noexcept override
	{
		return EventType::BindIDEvent;
	}
	[[nodiscard]] const std::string GetDebugName() const noexcept override
	{
		return "BindIDEvent";
	}
	[[nodiscard]] const BindID& GetBindID() const noexcept
	{
		return m_BindID;
	}
};

class ToggleDepthStencilStateEvent : public IEvent
{
private:

public:
	ToggleDepthStencilStateEvent() noexcept = default;
	virtual ~ToggleDepthStencilStateEvent() noexcept = default;

	[[nodiscard]] const EventType GetEventType() const noexcept override
	{
		return EventType::ToggleDepthStencilStateEvent;
	}
	[[nodiscard]] const std::string GetDebugName() const noexcept override
	{
		return "ToggleDepthStencilStateEvent";
	}
};