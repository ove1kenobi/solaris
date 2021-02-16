#include "pch.h"
#include "Light.h"

Light::Light() noexcept
	: m_DiffuseLightColor{ DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f) },
	  m_DiffuseLightIntensity{ 1.0f }
{
}


const DirectX::XMFLOAT3& Light::GetDiffuseColor() const noexcept
{
	return m_DiffuseLightColor;
}

const float& Light::GetDiffuseLightIntensity() const noexcept
{
	return m_DiffuseLightIntensity;
}

void Light::SetDiffuseColor(const DirectX::XMFLOAT3& diffuseColor) noexcept 
{
	m_DiffuseLightColor = diffuseColor;
}
