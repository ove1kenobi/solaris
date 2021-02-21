#include "pch.h"
#include "GameObject.h"

GameObject::GameObject() noexcept 
	:	m_velocity{ 0.0f, 0.0f, 0.0f },
		m_center{ 0.0f, 0.0f, 0.0f },
		m_forwardVector{ 0.0f, 0.0f, 1.0f },
		m_rightVector{ 1.0f, 0.0f, 0.0f },
		m_upVector{ 0.0f, 1.0f, 0.0f },
		m_wMatrix{
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
		},
		m_mass{ 0.0f },
		m_pitch{ 0.0f },
		m_roll{ 0.0f },
		m_yaw{ 0.0f },
		m_model{ nullptr },
		m_sumForces{ 0.0f, 0.0f, 0.0f }
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

const DirectX::XMFLOAT3& GameObject::GetSumForces() const noexcept
{
	return m_sumForces;
}

const DirectX::XMFLOAT3& GameObject::GetVelocity() const noexcept
{
	return m_velocity;
}

DirectX::XMFLOAT3 GameObject::getTransVector() {
	DirectX::XMFLOAT3 transVector = {};
	transVector.x = this->m_wMatrix._41;
	transVector.y = this->m_wMatrix._42;
	transVector.z = this->m_wMatrix._43;

	return transVector;
}

float GameObject::GetMass()
{
	return m_mass;
}

void GameObject::CalculateGravity(GameObject* other)
{
	if (other->GetMass() == 0 || m_mass == 0 || this == other) return;
	// Calculates the force of gravity between GameObjects a and b

	// ab = vector from a to b
	DirectX::XMFLOAT3 ab = other->GetCenter() - m_center;

	// r = |ab| -> (distance between a and b)
	double r = length(ab);

	// Newton's general theory of gravity
	float f = static_cast<float>(6.674e-11 * static_cast<double>(m_mass) * other->GetMass() / (r * r));

	// Normalize ab
	double inverse_r = 1.0f / r;
	ab.x = static_cast<float>(ab.x * inverse_r);
	ab.y = static_cast<float>(ab.y * inverse_r);
	ab.z = static_cast<float>(ab.z * inverse_r);

	// Force working on GameObject a
	ab = ab * f;
	//ab.x *= f;
	//ab.y *= f;
	//ab.z *= f;
	AddForce(ab);					// Force on object a
	other->AddForce(ab * (-1.0));	// Equal and opposite force on object b
}

void GameObject::AddForce(DirectX::XMFLOAT3 f)
{
	// Adds a force to the sum of forces influencing the GameObject this frame
	m_sumForces = m_sumForces + f;
}

void GameObject::UpdatePhysics()
{
	m_velocity = m_velocity + m_sumForces;
	m_sumForces = { 0.0f, 0.0f , 0.0f };
	m_center.x += static_cast<float>(m_velocity.x * m_timer.DeltaTime());
	m_center.y += static_cast<float>(m_velocity.y * m_timer.DeltaTime());
	m_center.z += static_cast<float>(m_velocity.z * m_timer.DeltaTime());
}