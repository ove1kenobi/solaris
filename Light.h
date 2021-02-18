#pragma once

struct BlinnPhongLightCB
{
	DirectX::XMFLOAT3 ambientColor;
	float padding1;
	DirectX::XMFLOAT3 diffuseColor;
	float padding2;
	DirectX::XMFLOAT3 lightWorldPosition;
	float diffuseLightIntensity;
	DirectX::XMFLOAT3 padding;
	float ambientLightIntensity;
	DirectX::XMFLOAT2 padding4;
	float specularPower;
	float specularIntensity;
};

class Light 
{
protected:
	DirectX::XMFLOAT3 m_DiffuseLightColor;
	float m_DiffuseLightIntensity;
public:
	Light() noexcept;
	virtual ~Light() noexcept = default;
	[[nodiscard]] const DirectX::XMFLOAT3& GetDiffuseColor() const noexcept;
	[[nodiscard]] const float& GetDiffuseLightIntensity() const noexcept;
	//Only for debugging and dynamic adjustment of values if needed
	void SetDiffuseColor(const DirectX::XMFLOAT3& diffuseColor) noexcept;
};