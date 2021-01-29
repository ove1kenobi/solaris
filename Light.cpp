#include "Light.h"

Light::Light(DirectX::XMVECTOR ambient, DirectX::XMVECTOR diffuse) {
	this->m_Ambient = ambient;
	this->m_Diffuse = diffuse;
}

Light::~Light() {

}

DirectX::XMVECTOR Light::GetAmbient()
{
	return this->m_Ambient;
}

DirectX::XMVECTOR Light::GetDiffuse() {
	return this->m_Diffuse;
}

void Light::SetAmbient(DirectX::XMVECTOR ambient) {
	this->m_Ambient = ambient;
}

void Light::SetDiffuse(DirectX::XMVECTOR diffuse) {
	this->m_Diffuse = diffuse;
}
