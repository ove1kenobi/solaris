#include "PointLight.h"

PointLight::PointLight(DirectX::XMVECTOR position) {
	this->position = position;
	this->ambient = ambient;
	this->diffuse = diffuse;
	/*
	light.Position = XMVectorSet(-20.0f, 15.0f, -20.0f, 0.0f);
	light.Ambient = XMVectorSet(0.3f, 0.3f, 0.3f, 1.0f);
	light.Diffuse = XMVectorSet(0.8f, 0.8f, 0.8f, 1.0f);

	XMVECTOR lookAtPos = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR direction = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	light.LightView = XMMatrixLookAtLH(light.Position, lookAtPos, direction);

	light.LightProjection = XMMatrixOrthographicLH((float)WIDTH, (float)HEIGHT, 0.1f, 100.0f);
	*/
}

PointLight::~PointLight()
{
}

bool PointLight::CreateViewMatrix()
{
	return false;
}

bool PointLight::CreateProjectionMatrix()
{
	return false;
}
