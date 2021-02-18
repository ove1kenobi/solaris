#include "pch.h"
#include "PointLight.h"

PointLight::PointLight() noexcept
	:  m_WorldPosition{ DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f) }
{
}

const bool PointLight::Initialize(const DirectX::XMFLOAT3& diffuseLightColor, const float& diffuseLightIntensity, const DirectX::XMFLOAT3& worldPosition) noexcept
{
	m_DiffuseLightColor = diffuseLightColor;
	m_DiffuseLightIntensity = diffuseLightIntensity;
	m_WorldPosition = worldPosition;
	return true;
}

const DirectX::XMFLOAT3& PointLight::GetWorldPosition() const noexcept
{
	return m_WorldPosition;
}

void PointLight::SetWorldPosition(const DirectX::XMFLOAT3& position) noexcept
{
	m_WorldPosition = position;
}