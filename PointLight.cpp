#include "PointLight.h"

PointLight::PointLight(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 attenuation, float range, DirectX::XMFLOAT4 ambient, DirectX::XMFLOAT4 diffuse)
	: Light(ambient, diffuse) {
	this->position = position;
	this->attenuation = attenuation;
	this->range = range;
}

PointLight::~PointLight() {

}

DirectX::XMFLOAT3 PointLight::GetPosition() {
	return this->position;
}

DirectX::XMFLOAT3 PointLight::GetAttenuation() {
	return this->attenuation;
}

//changes position of the point light
void PointLight::SetPosition(DirectX::XMFLOAT3 position) {
	this->position = position;
}

//Changes the three fall off factors for the point light
void PointLight::SetAttenuation(DirectX::XMFLOAT3 attenuation) {
	this->attenuation = attenuation;
}

//Changes the range of the point light
void PointLight::SetRange(float range) {
	this->range = range;
}
