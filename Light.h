#pragma once
#include <DirectXMath.h>

class Light {
private:
	DirectX::XMFLOAT4 ambient;
	DirectX::XMFLOAT4 diffuse;
public:
	Light(DirectX::XMFLOAT4 ambient, DirectX::XMFLOAT4 diffuse);
	~Light();
	//Only for debugging and dynamic adjustment of values if needed
	void SetAmbient(DirectX::XMFLOAT4 ambient);
	void SetDiffuse(DirectX::XMFLOAT4 diffuse);
};