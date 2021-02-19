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
	this->m_center = { 0.0f, 2000.0f, 0.0f };
	this->m_mass = 10000.0f;
	pi = static_cast<float>(atan(1) * 4);
	this->m_rotationAngle = pi;
}

bool SpaceShip::update(DirectX::XMMATRIX VMatrix, DirectX::XMMATRIX PMatrix, const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& deviceContext)
{
	this->UpdatePhysics();

#if defined(DEBUG) | defined(_DEBUG)
	ImGui::Begin("Spaceship");
	ImGui::Text("Center  : (%f, %f, %f)", m_center.x, m_center.y, m_center.z);
	ImGui::Text("Velocity: (%f, %f, %f)", m_velocity.x, m_velocity.y, m_velocity.z);
	ImGui::DragFloat("Mass", &m_mass, 500.0f);
	ImGui::End();
#endif

	//Updated the same way as a cosmicbody, with S * R * T. Rotation is around the ships up vector.
	DirectX::XMVECTOR up = DirectX::XMLoadFloat3(&this->m_upVector);
	//100 times smaller. TODO: make variable?
	DirectX::XMMATRIX scale = DirectX::XMMatrixScaling(0.01f, 0.01f, 0.01f);
	DirectX::XMMATRIX rot = DirectX::XMMatrixRotationAxis(up, this->m_rotationAngle);
	DirectX::XMMATRIX trans = DirectX::XMMatrixTranslation(this->m_center.x, this->m_center.y, this->m_center.z);
	DirectX::XMMATRIX final = scale * rot * trans;
	DirectX::XMStoreFloat4x4(&this->m_wMatrix, final);

	//Update the matrixBuffer.

	D3D11_MAPPED_SUBRESOURCE mappedSubresource;
	ModelFactory::MatrixBuffer* data;

	DirectX::XMMATRIX WMatrix = DirectX::XMMatrixTranspose(DirectX::XMLoadFloat4x4(&this->m_wMatrix));
	VMatrix = DirectX::XMMatrixTranspose(VMatrix);
	PMatrix = DirectX::XMMatrixTranspose(PMatrix);

	deviceContext->Map(
		this->m_model->getMatrixBuffer().Get(),
		0,
		D3D11_MAP_WRITE_DISCARD,
		0,
		&mappedSubresource
	);

	data = (ModelFactory::MatrixBuffer*)mappedSubresource.pData;

	data->WMatrix = WMatrix;
	data->VMatrix = VMatrix;
	data->PMatrix = PMatrix;

	deviceContext->Unmap(this->m_model->getMatrixBuffer().Get(), 0);
	return true;
}

void SpaceShip::move(DirectX::XMFLOAT4 deltaPos) {
	this->m_center.x += deltaPos.x;
	this->m_center.y += deltaPos.y;
	this->m_center.z += deltaPos.z;
}

void SpaceShip::rotate(float step) {
	this->m_rotationAngle -= step;
	
	if (this->m_rotationAngle <= -2 * pi) {
		this->m_rotationAngle += 2 * pi;
	}
	else if (this->m_rotationAngle >= 2*pi){
		this->m_rotationAngle -= 2 * pi;
	}
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

void SpaceShip::BindShadowUniques(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext)
{
	pDeviceContext->IASetVertexBuffers(0u,
									  1u,
									  this->m_model->getVertexBuffer().GetAddressOf(),
									  &this->m_model->getStride(),
									  &this->m_model->getOffset());
	pDeviceContext->IASetIndexBuffer(this->m_model->getIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);
}

void SpaceShip::CalculateGravity(GameObject* other)
{
	if (this == other) return;
	// Calculates the force of gravity between GameObjects a and b

	// ab = vector from a to b
	DirectX::XMFLOAT3 ab = other->GetCenter();
	ab.x -= this->m_center.x;
	ab.y -= this->m_center.y;
	ab.z -= this->m_center.z;

	// r = |ab| -> (distance between a and b)
	double r = sqrtf(ab.x * ab.x + ab.y * ab.y + ab.z * ab.z);

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

	// The equal and opposite force on GameObject b
	//ab.x *= -1.0f;
	//ab.y *= -1.0f;
	//ab.z *= -1.0f;
	//other->AddForce(ab);
}

void SpaceShip::AddForce(DirectX::XMFLOAT3 f)
{
	// Adds a force to the forces influencing the GameObject this frame
	m_forces.push_back(f);
}

void SpaceShip::UpdatePhysics()
{
	// Sum forces working on GameObject and apply
	DirectX::XMFLOAT3 sumForces = {};
	for (size_t i = 0; i < m_forces.size(); ++i)
	{
		sumForces.x += m_forces[i].x;
		sumForces.y += m_forces[i].y;
		sumForces.z += m_forces[i].z;
	}

	m_forces.clear();

	m_velocity.x += sumForces.x;
	m_velocity.y += sumForces.y;
	m_velocity.z += sumForces.z;

	m_center.x += static_cast<float>(m_velocity.x * m_timer.DeltaTime());
	m_center.y += static_cast<float>(m_velocity.y * m_timer.DeltaTime());
	m_center.z += static_cast<float>(m_velocity.z * m_timer.DeltaTime());
}
