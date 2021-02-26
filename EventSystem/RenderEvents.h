#pragma once
#include "IEvent.h"
#include "../Resources/BindIDs.h"
//class GameObject;
class PointLight;
class Scene;

class SendRenderObjectsEvent : public IEvent
{
private:
	struct CulledData* m_CulledData;
public:
	SendRenderObjectsEvent(CulledData* culledData) noexcept
	{
		m_CulledData = culledData;
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
	CulledData* GetCulledData() const noexcept
	{
		return m_CulledData;
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

class RequestSunLightEvent : public IEvent
{
private:
public:
	RequestSunLightEvent() noexcept = default;
	virtual ~RequestSunLightEvent() noexcept = default;

	[[nodiscard]] const EventType GetEventType() const noexcept override
	{
		return EventType::RequestSunLightEvent;
	}
	[[nodiscard]] const std::string GetDebugName() const noexcept override
	{
		return "RequestSunLightEvent";
	}
};

class DelegateSunLightEvent : public IEvent
{
private:
	PointLight* m_pSunLight;
public:
	DelegateSunLightEvent(PointLight* pSunLight) noexcept
	{
		m_pSunLight = pSunLight;
	}
	virtual ~DelegateSunLightEvent() noexcept = default;

	[[nodiscard]] const EventType GetEventType() const noexcept override
	{
		return EventType::DelegateSunLightEvent;
	}
	[[nodiscard]] const std::string GetDebugName() const noexcept override
	{
		return "DelegateSunLightEvent";
	}
	[[nodiscard]] PointLight* GetSunLight() const noexcept
	{
		return m_pSunLight;
	}
};