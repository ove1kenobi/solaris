#pragma once
#include "Light.h"
class PointLight : public Light 
{
private:
	DirectX::XMFLOAT3 m_WorldPosition;
public:
	PointLight() noexcept;
	virtual ~PointLight() noexcept = default;
	const bool Initialize(const DirectX::XMFLOAT3& diffuseLightColor, const float& diffuseLightIntensity, const DirectX::XMFLOAT3& worldPosition) noexcept;
	[[nodiscard]] const DirectX::XMFLOAT3& GetWorldPosition() const noexcept;
	//Only for debugging and dynamic adjustment of values if needed
	void SetWorldPosition(const DirectX::XMFLOAT3& position) noexcept;
};
