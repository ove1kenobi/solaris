#include "pch.h"
#include "Asteroid.h"

Asteroid::Asteroid() noexcept
{

}

Asteroid::~Asteroid()
{
}

bool Asteroid::init(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 velocity)
{
	m_center = pos;
	m_velocity = velocity;
	m_mass = 5000.0f;
	m_model = ModelFactory::Get().GetModel(std::string("models/cube.obj"));
	ModelFactory::Get().CreateMatrixBuffer(m_AmatrixBuffer);
	return true;
}

bool Asteroid::update(DirectX::XMMATRIX VMatrix, DirectX::XMMATRIX PMatrix, const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& deviceContext)
{
	//Updated the same way as a cosmicbody, with S * R * T. Rotation is around the ships up vector.
	DirectX::XMVECTOR up = DirectX::XMLoadFloat3(&m_upVector);
	//100 times smaller. TODO: make variable?
	DirectX::XMMATRIX scale = DirectX::XMMatrixIdentity();
	DirectX::XMMATRIX rot = DirectX::XMMatrixIdentity();
	DirectX::XMMATRIX trans = DirectX::XMMatrixTranslation(m_center.x, m_center.y, m_center.z);
	DirectX::XMMATRIX final = scale * rot * trans;
	DirectX::XMStoreFloat4x4(&m_wMatrix, final);

	//Update the matrixBuffer.

	D3D11_MAPPED_SUBRESOURCE mappedSubresource;
	ModelFactory::MatrixBuffer* data;

	DirectX::XMMATRIX WMatrix = DirectX::XMMatrixTranspose(DirectX::XMLoadFloat4x4(&m_wMatrix));
	VMatrix = DirectX::XMMatrixTranspose(VMatrix);
	PMatrix = DirectX::XMMatrixTranspose(PMatrix);

	deviceContext->Map(
		m_AmatrixBuffer.Get(),
		0,
		D3D11_MAP_WRITE_DISCARD,
		0,
		&mappedSubresource
	);

	data = (ModelFactory::MatrixBuffer*)mappedSubresource.pData;

	data->WMatrix = WMatrix;
	data->VMatrix = VMatrix;
	data->PMatrix = PMatrix;

	deviceContext->Unmap(m_AmatrixBuffer.Get(), 0);
	return true;
}

void Asteroid::bindUniques(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& deviceContext)
{
	deviceContext->IASetVertexBuffers(0u,
		1u,
		m_model->getVertexBuffer().GetAddressOf(),
		&m_model->getStride(),
		&m_model->getOffset());
	deviceContext->IASetIndexBuffer(m_model->getIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);
	deviceContext->VSSetConstantBuffers(0, 1, m_AmatrixBuffer.GetAddressOf());
}

const bool Asteroid::IntersectRayObject(const DirectX::FXMVECTOR& origin, const DirectX::FXMVECTOR& direction, float& distance) noexcept
{
	DirectX::BoundingBox b = *(m_model->GetBoundingBox());
	b.Center = m_center;
	return b.Intersects(origin, direction, distance);
}

void Asteroid::CalculateGravity(GameObject* other)
{
	if (this == other) return;
	// Calculates the force of gravity between GameObjects a and b

	// ab = vector from a to b
	DirectX::XMFLOAT3 ab = other->GetCenter() - m_center;

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
}

void Asteroid::AddForce(DirectX::XMFLOAT3 f)
{
	// Adds a force to the forces influencing the GameObject this frame
	m_forces.push_back(f);
}

void Asteroid::UpdatePhysics()
{
	// Sum forces working on GameObject and apply
	DirectX::XMFLOAT3 sumForces = {};
	for (size_t i = 0; i < m_forces.size(); ++i)
	{
		sumForces = sumForces + m_forces[i];
	}

	m_forces.clear();

	m_velocity = m_velocity + sumForces;

	m_center.x += static_cast<float>(m_velocity.x * m_timer.DeltaTime());
	m_center.y += static_cast<float>(m_velocity.y * m_timer.DeltaTime());
	m_center.z += static_cast<float>(m_velocity.z * m_timer.DeltaTime());
}