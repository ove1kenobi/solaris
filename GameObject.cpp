#include "pch.h"
#include "GameObject.h"

const double GameObject::G = 6.674e-11;

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
		m_1byMass{ 0 },
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

void GameObject::CalculateGravity(GameObject* other)
{
	// Calculates the force of gravity between this and other GameObject
	DirectX::XMVECTOR a = DirectX::XMLoadFloat3(&this->m_center);
	DirectX::XMVECTOR b = DirectX::XMLoadFloat3(&other->m_center);
	DirectX::XMVECTOR ab = DirectX::XMVectorSubtract(b, a);
	DirectX::XMVECTOR dab = DirectX::XMVector3Normalize(ab);
	double r = DirectX::XMVectorGetX(DirectX::XMVector3Length(ab));
	double f = G * this->m_mass * other->m_mass / (r * r);
	m_forces.push_back(DirectX::XMVectorScale(dab, f));
	other->AddForce(DirectX::XMVectorScale(dab, -f));
}

void GameObject::AddForce(DirectX::XMVECTOR f)
{
	// Adds a force to the forces influencing the GameObject this frame
	m_forces.push_back(f);
}

void GameObject::UpdatePhysics()
{
	DirectX::XMVECTOR sumForces = DirectX::XMVectorZero();
	for (size_t i = 0; i < m_forces.size(); ++i)
	{
		sumForces = DirectX::XMVectorAdd(sumForces, m_forces[i]);
	}
	m_forces.empty();
	sumForces = DirectX::XMVectorScale(sumForces, m_timer.DeltaTime() * this->m_1byMass);
	m_velocity.x += DirectX::XMVectorGetX(sumForces);
	m_velocity.y += DirectX::XMVectorGetY(sumForces);
	m_velocity.z += DirectX::XMVectorGetZ(sumForces);

	//m_center.x += m_velocity.x;
	//m_center.y += m_velocity.y;
	//m_center.z += m_velocity.z;
}