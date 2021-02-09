#include "pch.h"
#include "GameObject.h"

const float GameObject::G = 6.674e-11;

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
		m_1byMass{ 0.0f },
		m_pitch{ 0.0f },
		m_roll{ 0.0f },
		m_yaw{ 0.0f },
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
	//DirectX::XMVECTOR a = DirectX::XMLoadFloat3(&this->m_center);
	//DirectX::XMVECTOR b = DirectX::XMLoadFloat3(&other->m_center);
	//DirectX::XMVECTOR ab = DirectX::XMVectorSubtract(b, a);
	// ab = vector from a to b
	DirectX::XMFLOAT3 ab = other->m_center;
	ab.x -= this->m_center.x;
	ab.y -= this->m_center.y;
	ab.z -= this->m_center.z;
	//DirectX::XMVECTOR dab = DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&ab));
	// r = distance between a and b
	double r = sqrt(ab.x * ab.x + ab.y * ab.y + ab.z * ab.z);
	double ir = 1.0f / r;
	//double G_x_cube_ir = ir * ir * ir * 6.674e-11;
	//float r = DirectX::XMVectorGetX(DirectX::XMVector3Length(DirectX::XMLoadFloat3(&ab)));
	double f = G * static_cast<double>(this->m_mass) * other->m_mass * ir * ir;
	//float f_t = other->m_mass * ir * 6.674e-11;
	//float f_o = this->m_mass * ir * ir * ir * 6.674e-11;
	DirectX::XMFLOAT3 ab_l = ab;
	ab_l.x *= f;
	ab_l.y *= f;
	ab_l.z *= f;
	m_forces.push_back(ab_l);
	DirectX::XMFLOAT3 ab_ml = ab_l;
	ab_ml.x *= -1.0f;
	ab_ml.y *= -1.0f;
	ab_ml.z *= -1.0f;
	other->AddForce(ab_ml);
//#if defined(DEBUG) | defined(_DEBUG)
//	ImGui::Begin("CalculateGravity");
//	ImGui::Text("ab    : (%.0f, %.0f, %.0f)", ab.x, ab.y, ab.z);
//	ImGui::Text("mass  : %f", this->m_mass);
//	ImGui::Text("r     : %f", r);
//	ImGui::Text("f     : %Lf", f);
//	ImGui::End();
//#endif
}

void GameObject::AddForce(DirectX::XMFLOAT3 f)
{
	// Adds a force to the forces influencing the GameObject this frame
	m_forces.push_back(f);
}

void GameObject::UpdatePhysics()
{
	DirectX::XMFLOAT3 sumForces = {};
	for (size_t i = 0; i < m_forces.size(); ++i)
	{
		sumForces.x += m_forces[i].x;
		sumForces.y += m_forces[i].y;
		sumForces.z += m_forces[i].z;
	}
//#if defined(DEBUG) | defined(_DEBUG)
//	ImGui::Begin("Game Objects");
//	for (unsigned int i{ 0u }; i < m_forces.size(); i++)
//	{
//		ImGui::Text("Game Object #%d", i + 1);
//		ImGui::Text("Center: (%.0f, %.0f, %.0f)", m_forces[i].x, m_forces[i].y, m_forces[i].z);
//	}
//	ImGui::End();
//#endif
	m_forces.clear();
	//float change = m_timer.DeltaTime() * this->m_1byMass;
	//sumForces.x *= m_timer.DeltaTime();
	//sumForces.y *= m_timer.DeltaTime();
	//sumForces.z *= m_timer.DeltaTime();
	m_velocity.x += sumForces.x * m_timer.DeltaTime();
	m_velocity.y += sumForces.y * m_timer.DeltaTime();
	m_velocity.z += sumForces.z * m_timer.DeltaTime();

//#if defined(DEBUG) | defined(_DEBUG)
//	ImGui::Begin("UpdatePhysics");
//	ImGui::Text("Center  : (%f, %f, %f)", m_center.x, m_center.y, m_center.z);
//	ImGui::Text("Velocity: (%f, %f, %f)", m_velocity.x, m_velocity.y, m_velocity.z);
//	ImGui::Text("Sum f   : (%f, %f, %f)", sumForces.x, sumForces.y, sumForces.z);
//	//ImGui::Text("Mass    : %f", m_mass);
//	ImGui::End();
//#endif

	m_center.x += m_velocity.x * m_timer.DeltaTime();
	m_center.y += m_velocity.y * m_timer.DeltaTime();
	m_center.z += m_velocity.z * m_timer.DeltaTime();
	//m_center.x += 1.0f;
	//m_center.y += 1.0f;
	//m_center.z += 1.0f;
}