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
	bool m_TestForCulling;
private:
	void OnEvent(IEvent& event) noexcept override;
public:
	Sun() noexcept;
	virtual ~Sun();
	//Uses the CosmicBody init() and light init() to set everything up related to the sun
	[[nodiscard]] const bool Initialize() noexcept;
	[[nodiscard]] const PointLight& GetPointLight() const;
	[[nodiscard]] const bool IntersectRayObject(const DirectX::XMFLOAT3* origin, const DirectX::XMFLOAT3* direction, float& distance) noexcept override;
	[[nodiscard]] const bool& ShallBeTestedForCulling() const noexcept override;
	void bindUniques(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& deviceContext) override;
};
