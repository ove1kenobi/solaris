#pragma once
#include "CosmicBody.h"
#include "PointLight.h"
#include "EventSystem/IEventListener.h"
#include "EventSystem/EventPublisher.h"

//Sun is of the type CosmicBody, as it needs the same properties
class Sun : public CosmicBody, public IEventListener, public EventPublisher 
{
private:
	//But it also contains a light source
	PointLight m_PointLight;
private:
	void OnEvent(IEvent& event) noexcept override;
public:
	Sun() noexcept;
	virtual ~Sun() noexcept = default;
	//Uses the CosmicBody init() and light init() to set everything up related to the sun
	[[nodiscard]] const bool Initialize() noexcept;
	[[nodiscard]] const PointLight& GetPointLight() const;
};
