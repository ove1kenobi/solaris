#include "pch.h"
#include "PointLight.h"

PointLight::PointLight() {
	//TODO: look how position is handled in cosmic body
	this->m_Position = {0,0,0};
	this->m_Range = 100.0f;
	this->m_Attenuation = {25.0f, 50.0f, 75.0f};
}

PointLight::~PointLight() {

}

bool PointLight::Init(DirectX::XMVECTOR ambient, DirectX::XMVECTOR diffuse, DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 attenuation, float range) {
	this->m_Ambient = ambient;
	this->m_Diffuse = diffuse;
	this->m_Position = position;
	this->m_Attenuation = attenuation;
	this->m_Range = range;
	return true;
}

DirectX::XMFLOAT3 PointLight::GetPosition() {
	return this->m_Position;
}

DirectX::XMFLOAT3 PointLight::GetAttenuation() {
	return this->m_Attenuation;
}

//changes position of the point light
void PointLight::SetPosition(DirectX::XMFLOAT3 position) {
	this->m_Position = position;
}

//Changes the three fall off factors for the point light
void PointLight::SetAttenuation(DirectX::XMFLOAT3 attenuation) {
	this->m_Attenuation = attenuation;
}

//Changes the range of the point light
void PointLight::SetRange(float range) {
	this->m_Range = range;
}
