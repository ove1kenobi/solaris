#pragma once
#include "Light.h"

class PointLight : public Light {
private:
	DirectX::XMFLOAT3 m_Position;
	DirectX::XMFLOAT3 m_Attenuation;
	float m_Range;
public:
	PointLight();
	~PointLight();
	//bool Init();
	bool Init(DirectX::XMVECTOR ambient, DirectX::XMVECTOR diffuse, DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 attenuation, float range);
	//For incorporation with the shaders
	DirectX::XMFLOAT3 GetPosition();
	DirectX::XMFLOAT3 GetAttenuation();
	//Only for debugging and dynamic adjustment of values if needed
	void SetPosition(DirectX::XMFLOAT3 position);
	void SetAttenuation(DirectX::XMFLOAT3 attenuation);
	void SetRange(float range);
};
