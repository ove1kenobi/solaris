#include "pch.h"
#include "SpaceShip.h"

SpaceShip::SpaceShip()
{
	this->m_model = ModelFactory::Get().GetModel(std::string("models/SciFi_Fighter_AK5.obj"));
	this->m_wMatrix = {
		0.03f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.03f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.03f, 0.0f,
		0.0f, 0.0f, 100.0f, 1.0f
	};
	this->m_center = { 0.0f, 1000.0f, -10000.0f };
	this->m_mass = 10000.0f;
	m_yaw = (float)M_PI;
	m_pitchTilt = 0.0f;
	m_rollTilt = 0.0f;
	m_topSpeed = 3000.0f;
}

bool SpaceShip::update(DirectX::XMMATRIX VMatrix, DirectX::XMMATRIX PMatrix, const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& deviceContext)
{
	this->UpdatePhysics();

#if defined(DEBUG) | defined(_DEBUG)
	ImGui::Begin("Spaceship");
	ImGui::Text("Center  : (%f, %f, %f)", m_center.x, m_center.y, m_center.z);
	ImGui::Text("Velocity: (%f, %f, %f)", m_velocity.x, m_velocity.y, m_velocity.z);
	ImGui::Text("Speed	: %f", length(m_velocity));
	//ImGui::DragFloat("Mass", &m_mass, 500.0f);
	ImGui::End();
#endif

	//Updated the same way as a cosmicbody, with S * R * T. Rotation is around the ships up vector.
	DirectX::XMVECTOR up = DirectX::XMLoadFloat3(&this->m_upVector);
	//100 times smaller. TODO: make variable?
	DirectX::XMMATRIX scale = DirectX::XMMatrixScaling(0.01f, 0.01f, 0.01f);
	DirectX::XMMATRIX rotX = DirectX::XMMatrixRotationX(m_pitch + m_pitchTilt);
	DirectX::XMMATRIX rotY = DirectX::XMMatrixRotationY(m_yaw);
	DirectX::XMVECTOR forward{ m_forwardVector.x, m_forwardVector.y, m_forwardVector.z };
	DirectX::XMMATRIX roll = DirectX::XMMatrixRotationAxis(forward, m_roll + m_rollTilt);
	DirectX::XMMATRIX rot = rotX * rotY * roll;

	DirectX::XMMATRIX trans = DirectX::XMMatrixTranslation(this->m_center.x, this->m_center.y, this->m_center.z);
	DirectX::XMMATRIX final = scale * rot * trans;
	DirectX::XMStoreFloat4x4(&this->m_wMatrix, final);

	//Update the matrixBuffer.
	D3D11_MAPPED_SUBRESOURCE mappedSubresource;
	DirectX::XMMATRIX WMatrix = DirectX::XMMatrixTranspose(DirectX::XMLoadFloat4x4(&this->m_wMatrix));
	VMatrix = DirectX::XMMatrixTranspose(VMatrix);
	PMatrix = DirectX::XMMatrixTranspose(PMatrix);

	deviceContext->Map(this->m_model->getMatrixBuffer().Get(),
			           0,
		               D3D11_MAP_WRITE_DISCARD,
		               0,
		               &mappedSubresource);

	ModelFactory::MatrixBuffer* data = (ModelFactory::MatrixBuffer*)mappedSubresource.pData;

	data->WMatrix = WMatrix;
	data->VMatrix = VMatrix;
	data->PMatrix = PMatrix;

	deviceContext->Unmap(this->m_model->getMatrixBuffer().Get(), 0);
	return true;
}

void SpaceShip::AddRotation(float yaw, float pitch)
{
	float alpha = 0.1f;

	m_yaw += yaw;
	if (m_yaw >= 2 * (float)M_PI) m_yaw -= 2 * (float)M_PI;
	else if (m_yaw <= -2 * (float)M_PI) m_yaw += 2 * (float)M_PI;

	m_pitch += pitch;
	if (m_pitch > (float)M_PI_2 - alpha) m_pitch = (float)M_PI_2 - alpha;
	else if (m_pitch < -(float)M_PI_2 + alpha) m_pitch = -(float)M_PI_2 + alpha;
}

void SpaceShip::SetTilt(float pitchLerp, float rollLerp)
{
	m_pitchTilt = pitchLerp * (float)M_PI / 8.0f;
	m_rollTilt = rollLerp * (float)M_PI_4;
}

void SpaceShip::SetForwardVector(DirectX::XMFLOAT3 forwardVector)
{
	m_forwardVector = forwardVector;
}

DirectX::XMFLOAT3 SpaceShip::GetVelocity()
{
	return m_velocity;
}

const bool SpaceShip::IntersectRayObject(const DirectX::FXMVECTOR& origin, const DirectX::FXMVECTOR& direction, float& distance) noexcept
{
	return false;
}

DirectX::XMFLOAT3 SpaceShip::getCenter() {
	return this->m_center;
}

void SpaceShip::bindUniques(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& deviceContext)
{
	deviceContext->IASetVertexBuffers(0u,
									  1u,
									  this->m_model->getVertexBuffer().GetAddressOf(),
									  &this->m_model->getStride(),
									  &this->m_model->getOffset());
	
	deviceContext->IASetIndexBuffer(this->m_model->getIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);
	deviceContext->VSSetConstantBuffers(0, 1, this->m_model->getMatrixBuffer().GetAddressOf());
}

void SpaceShip::CalculateGravity(GameObject* other)
{
	if (this == other) return;
	// Calculates the force of gravity between GameObjects a and b

	// ab = vector from a to b
	DirectX::XMFLOAT3 ab = other->GetCenter() - m_center;

	// r = |ab| -> (distance between a and b)
	double r = length(ab);
	if (r == 0.0f) r = 0.001f;

	// Newton't general theory of gravity
	float f = static_cast<float>(6.674e-11 * static_cast<double>(this->m_mass) * other->GetMass() / (r * r));

	// Normalize ab
	double inverse_r = 1.0f / r;
	ab.x = static_cast<float>(ab.x * inverse_r);
	ab.y = static_cast<float>(ab.y * inverse_r);
	ab.z = static_cast<float>(ab.z * inverse_r);

	// Force working on GameObject a
	ab.x *= f;
	ab.y *= f;
	ab.z *= f;
	m_forces.push_back(ab);
}

void SpaceShip::AddForce(DirectX::XMFLOAT3 f)
{
	// Adds a force to the forces influencing the GameObject this frame
	m_forces.push_back(f);
}

void SpaceShip::Deceleration(float breakingStrenght)
{
	float speed = length(m_velocity);

	if (speed != 0.0f) {
		DirectX::XMFLOAT3 breakingForce{ -m_velocity.x, -m_velocity.y, -m_velocity.z };
		breakingForce = normalize(breakingForce);
		breakingForce.x *= breakingStrenght;
		breakingForce.y *= breakingStrenght;
		breakingForce.z *= breakingStrenght;
		m_velocity.x += breakingForce.x;
		m_velocity.y += breakingForce.y;
		m_velocity.z += breakingForce.z;
		
		if (dot(m_velocity, breakingForce) > 0.0f) {
			m_velocity.x = 0.0f;
			m_velocity.y = 0.0f;
			m_velocity.z = 0.0f;
		}
	}
}

void SpaceShip::UpdatePhysics()
{
	// Sum forces working on GameObject and apply
	DirectX::XMFLOAT3 sumForces = {};
	for (size_t i = 0; i < m_forces.size(); ++i)
	{
		sumForces = sumForces + m_forces[i];
	}

	m_forces.clear();

	if (m_mass != 0.0f) {
		m_velocity.x += sumForces.x / m_mass;
		m_velocity.y += sumForces.y / m_mass;
		m_velocity.z += sumForces.z / m_mass;
	}

	float speed = length(m_velocity);
	
	if (speed > m_topSpeed) {
		m_velocity = normalize(m_velocity);
		m_velocity.x = m_topSpeed * m_velocity.x;
		m_velocity.y = m_topSpeed * m_velocity.y;
		m_velocity.z = m_topSpeed * m_velocity.z;
	}

	m_center.x += static_cast<float>(m_velocity.x * m_timer.DeltaTime());
	m_center.y += static_cast<float>(m_velocity.y * m_timer.DeltaTime());
	m_center.z += static_cast<float>(m_velocity.z * m_timer.DeltaTime());
}
