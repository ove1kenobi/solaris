#pragma once
#include "Light.h"

class PointLight : public Light {
private:
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 attenuation;
	float range;
public:
	PointLight(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 attenuation, float range, DirectX::XMFLOAT4 ambient, DirectX::XMFLOAT4 diffuse);
	~PointLight();
	//Only for debugging and dynamic adjustment of values if needed
	void SetPosition(DirectX::XMFLOAT3 position);
	void SetAttenuation(DirectX::XMFLOAT3 attenuation);
	void SetRange(float range);
};
