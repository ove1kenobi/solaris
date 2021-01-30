#pragma once
#include "Light.h"

class PointLight : public Light {
private:
	DirectX::XMVECTOR m_Position;
	DirectX::XMFLOAT3 m_Attenuation;
	float m_Range;
public:
	PointLight(DirectX::XMVECTOR position, DirectX::XMFLOAT3 attenuation, float range, DirectX::XMVECTOR ambient, DirectX::XMVECTOR diffuse);
	~PointLight();
	//For incorporation with the shaders
	DirectX::XMVECTOR GetPosition();
	DirectX::XMFLOAT3 GetAttenuation();
	//Only for debugging and dynamic adjustment of values if needed
	void SetPosition(DirectX::XMVECTOR position);
	void SetAttenuation(DirectX::XMFLOAT3 attenuation);
	void SetRange(float range);
};
