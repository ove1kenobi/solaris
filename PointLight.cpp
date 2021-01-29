#include "PointLight.h"

PointLight::PointLight(DirectX::XMVECTOR position, DirectX::XMFLOAT3 attenuation, float range, DirectX::XMVECTOR ambient, DirectX::XMVECTOR diffuse)
	: Light(ambient, diffuse) {
	this->m_Position = position;
	this->m_Attenuation = attenuation;
	this->m_Range = range;
}

PointLight::~PointLight() {

}

DirectX::XMVECTOR PointLight::GetPosition() {
	return this->m_Position;
}

DirectX::XMFLOAT3 PointLight::GetAttenuation() {
	return this->m_Attenuation;
}

//changes position of the point light
void PointLight::SetPosition(DirectX::XMVECTOR position) {
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
