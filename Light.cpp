#include "pch.h"
#include "Light.h"

Light::Light() {
	this->m_Ambient = { 1.0f ,1.0f ,1.0f , 0.0f };
	this->m_Diffuse = { 1.0f ,1.0f ,1.0f , 0.0f };
}

Light::~Light() {

}

bool Light::Init(DirectX::XMVECTOR ambient, DirectX::XMVECTOR diffuse) {
	this->m_Ambient = ambient;
	this->m_Diffuse = diffuse;
	return true;
}

DirectX::XMVECTOR Light::GetAmbient() {
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
