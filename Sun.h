#pragma once
#include "CosmicBody.h"
#include "PointLight.h"
#include "EventSystem/IEventListener.h"
#include "EventSystem/EventPublisher.h"
#include "EventSystem\UtilityEvents.h"

//Sun is of the type CosmicBody, as it needs the same properties
class Sun : public CosmicBody, public IEventListener, public EventPublisher 
{
private:
	//But it also contains a light source
	PointLight m_PointLight;
	std::string m_Tag;
	bool m_TestForCulling;
private:
	void OnEvent(IEvent& event) noexcept override;
public:
	Sun() noexcept;
	virtual ~Sun() noexcept = default;
	//Uses the CosmicBody init() and light init() to set everything up related to the sun
	[[nodiscard]] const bool Initialize() noexcept;
	[[nodiscard]] const PointLight& GetPointLight() const;
	[[nodiscard]] const bool IntersectRayObject(const DirectX::FXMVECTOR& origin, const DirectX::FXMVECTOR& direction, float& distance) noexcept override;
	[[nodiscard]] const std::string& GetTag() const noexcept override;
	[[nodiscard]] const bool& ShallBeTestedForCulling() const noexcept override;
};
