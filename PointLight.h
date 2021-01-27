#pragma once
#include "Light.h"

//Point Lights: have a position, a fall off point or range, and a color

class PointLight {
private:
	DirectX::XMMATRIX view;
	DirectX::XMMATRIX projection;
	DirectX::XMVECTOR position;
	DirectX::XMVECTOR ambient;
	DirectX::XMVECTOR diffuse;
	D3D11_BUFFER_DESC buffer;
	float range;
protected:
public:
	PointLight(DirectX::XMVECTOR position, DirectX::XMVECTOR ambient, DirectX::XMVECTOR diffuse);
	~PointLight();
	bool CreateViewMatrix();
	bool CreateProjectionMatrix();
};
