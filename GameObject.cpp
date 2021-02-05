#include "GameObject.h"

GameObject::GameObject() noexcept 
	:	m_velocity{ 0, 0, 0 },
		m_center{ 0, 0, 0 },
		m_forwardVector{ 0, 0, 1 },
		m_rightVector{ 1, 0, 0 },
		m_upVector{ 0, 1, 0 },
		m_wMatrix{
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
		},
		m_mass{ 0 },
		m_pitch{ 0 },
		m_roll{ 0 },
		m_yaw{ 0 },
		m_model{ nullptr }
{

}

GameObject::~GameObject() {
	if (this->m_model->NotLoaded()) {
		delete this->m_model;
	}
}

void GameObject::getWMatrix(DirectX::XMMATRIX& wMatrix) {
	wMatrix = DirectX::XMLoadFloat4x4(&this->m_wMatrix);
}

UINT GameObject::getVertexBufferSize() {
	return this->m_model->getVertexBufferSize();
}

UINT GameObject::getIndexBufferSize() {
	return this->m_model->getIndexBufferSize();
}

const DirectX::XMFLOAT3& GameObject::GetCenter() const noexcept
{
	return m_center;
}

DirectX::XMFLOAT3 GameObject::getTransVector() {
	DirectX::XMFLOAT3 transVector;
	transVector.x = this->m_wMatrix._41;
	transVector.y = this->m_wMatrix._42;
	transVector.z = this->m_wMatrix._43;

	return transVector;
}
