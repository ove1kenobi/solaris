#pragma once
#include <DirectXMath.h>

class Light {
private:
	DirectX::XMVECTOR m_Ambient;
	DirectX::XMVECTOR m_Diffuse;
public:
	Light(DirectX::XMVECTOR ambient, DirectX::XMVECTOR diffuse);
	~Light();
	//For incorporation with the shaders
	DirectX::XMVECTOR GetAmbient();
	DirectX::XMVECTOR GetDiffuse();
	//Only for debugging and dynamic adjustment of values if needed
	void SetAmbient(DirectX::XMVECTOR ambient);
	void SetDiffuse(DirectX::XMVECTOR diffuse);
};