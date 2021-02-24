#pragma once
#include "IEvent.h"
#include "../Resources/BindIDs.h"
class GameObject;
class PointLight;

class SendRenderObjectsEvent : public IEvent
{
private:
	std::vector<GameObject*>* m_gameObjects;
public:
	SendRenderObjectsEvent(std::vector<GameObject*> *gameObjects) noexcept
	{
		this->m_gameObjects = gameObjects;
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

class ResetDefaultViewportEvent : public IEvent
{
private:
public:
	ResetDefaultViewportEvent() noexcept = default;
	virtual ~ResetDefaultViewportEvent() noexcept = default;

	[[nodiscard]] const EventType GetEventType() const noexcept override
	{
		return EventType::ResetDefaultViewportEvent;
	}
	[[nodiscard]] const std::string GetDebugName() const noexcept override
	{
		return "ResetDefaultViewportEvent";
	}
};

class CreateShadowMapViewportEvent : public IEvent
{
private:
	float m_Width;
	float m_Height;
public:
	CreateShadowMapViewportEvent(const float& width, const float& height) noexcept
		: m_Width{ width }, m_Height{ height }
	{
	}
	virtual ~CreateShadowMapViewportEvent() noexcept = default;

	[[nodiscard]] const EventType GetEventType() const noexcept override
	{
		return EventType::CreateShadowMapViewportEvent;
	}
	[[nodiscard]] const std::string GetDebugName() const noexcept override
	{
		return "CreateShadowMapViewportEvent";
	}
	[[nodiscard]] const float& GetWidth() const noexcept
	{
		return m_Width;
	}
	[[nodiscard]] const float& GetHeight() const noexcept
	{
		return m_Height;
	}
};

class SetShadowMapViewportEvent : public IEvent
{
private:
public:
	SetShadowMapViewportEvent() noexcept = default;
	virtual ~SetShadowMapViewportEvent() noexcept = default;

	[[nodiscard]] const EventType GetEventType() const noexcept override
	{
		return EventType::SetShadowMapViewportEvent;
	}
	[[nodiscard]] const std::string GetDebugName() const noexcept override
	{
		return "SetShadowMapViewportEvent";
	}
};

class SetShadowBlendStateEvent : public IEvent
{
private:
public:
	SetShadowBlendStateEvent() noexcept = default;
	virtual ~SetShadowBlendStateEvent() noexcept = default;

	[[nodiscard]] const EventType GetEventType() const noexcept override
	{
		return EventType::SetShadowBlendStateEvent;
	}
	[[nodiscard]] const std::string GetDebugName() const noexcept override
	{
		return "SetShadowBlendStateEvent";
	}
};

class ResetDefaultBlendStateEvent : public IEvent
{
private:
public:
	ResetDefaultBlendStateEvent() noexcept = default;
	virtual ~ResetDefaultBlendStateEvent() noexcept = default;

	[[nodiscard]] const EventType GetEventType() const noexcept override
	{
		return EventType::ResetDefaultBlendStateEvent;
	}
	[[nodiscard]] const std::string GetDebugName() const noexcept override
	{
		return "ResetDefaultBlendStateEvent";
	}
};