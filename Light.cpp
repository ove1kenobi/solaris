#include "Light.h"

Light::Light(DirectX::XMFLOAT4 ambient, DirectX::XMFLOAT4 diffuse) {
	this->ambient = ambient;
	this->diffuse = diffuse;
}

Light::~Light() {

}

void Light::SetAmbient(DirectX::XMFLOAT4 ambient) {
	this->ambient = ambient;
}

void Light::SetDiffuse(DirectX::XMFLOAT4 diffuse) {
	this->diffuse = diffuse;
}
