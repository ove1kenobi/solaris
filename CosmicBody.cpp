#include "pch.h"
#include "CosmicBody.h"

CosmicBody::CosmicBody() noexcept
	: m_radius{ 0.0f }, m_yAxis{ 0.0f, 1.0f, 0.0f, 0.0f }, m_rotationDir{ 0 },
	m_tetheredTo{ nullptr }, m_major_semi_axis{ 0 }, m_minor_semi_axis{ 0 }, m_orbital_speed{ 0 },
	m_orbit{ nullptr }, m_waterSphere{ nullptr }
{
	
}

CosmicBody::~CosmicBody()
{
}

bool CosmicBody::init(float x, float y, float z, float r, float xRot, float zRot, int rotDir, GameObject* tetherTo, Orbit* orbit, WaterSphere* waterSphere) {
	m_waterSphere = waterSphere;
	if(m_waterSphere)
		m_waterSphere->Initialize(x, y, z, r);

	//Set initial values. All randomized.
	this->m_radius = r;
	this->m_mass = r * 4.68e11f;
	this->m_center.x = x;
	this->m_center.y = y;
	this->m_center.z = z;
	this->m_pitch = xRot;
	this->m_roll = zRot;
	this->m_rotationDir = rotDir;
	this->m_tetheredTo = tetherTo;
	if(tetherTo)
	{
		this->m_major_semi_axis = length(m_center - tetherTo->GetCenter());
		this->m_minor_semi_axis = this->m_major_semi_axis * 0.8f;
		this->m_orbital_speed = (static_cast<float>(6.674e-11 * tetherTo->GetMass() / (m_major_semi_axis * m_major_semi_axis)));
	}
	if (orbit)
	{
		m_orbit = orbit;
		m_orbit->init(m_major_semi_axis, m_minor_semi_axis);
	}
	//If the random value was 0, set it to -1.
	if (this->m_rotationDir == 0) {
		this->m_rotationDir--;
	}

	//Set up the planets randomized y-axis.
	DirectX::XMMATRIX rotX = DirectX::XMMatrixRotationX(this->m_pitch);
	DirectX::XMMATRIX rotZ = DirectX::XMMatrixRotationZ(this->m_roll);
	DirectX::XMVECTOR resultY = DirectX::XMLoadFloat4(&this->m_yAxis);
	resultY = DirectX::XMVector4Transform(resultY, rotX);
	resultY = DirectX::XMVector4Transform(resultY, rotZ);
	DirectX::XMStoreFloat4(&this->m_yAxis, resultY);
	
	//Initialize the wMatrix.
	DirectX::XMStoreFloat4x4(&this->m_wMatrix, DirectX::XMMatrixIdentity());
	DirectX::XMMATRIX trans = DirectX::XMMatrixTranslation(x, y, z);
	DirectX::XMMATRIX final = rotX * rotZ * trans;

	DirectX::XMStoreFloat4x4(&this->m_wMatrix, final);
	
	m_HasBoundingVolume = true;

	return true;
}

GameObject* CosmicBody::update(DirectX::XMFLOAT4X4 VMatrix, DirectX::XMFLOAT4X4 PMatrix, const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& deviceContext)
{
	m_sumForces = { 0.0f, 0.0f, 0.0f };
	// Update orbit
	if (m_tetheredTo)
	{
		DirectX::XMFLOAT3 tc = m_tetheredTo->GetCenter();
		m_center.x = static_cast<float>(tc.x + m_major_semi_axis * cos(m_time.SinceStart() * m_orbital_speed));
		m_center.z = static_cast<float>(tc.z + m_minor_semi_axis * sin(m_time.SinceStart() * m_orbital_speed));
	}
	static float angle = 0.0f;

	//Construct rotation matrices
	DirectX::XMMATRIX scaleMatrix = DirectX::XMMatrixScaling(m_scale, m_scale, m_scale);

	//Angle change is positive or negative depending on a randomized value PER PLANET.
	DirectX::XMMATRIX rotMatrix = DirectX::XMMatrixRotationAxis(DirectX::XMLoadFloat4(&this->m_yAxis), angle * this->m_rotationDir);
	
	//Construct the translation matrix.
	DirectX::XMMATRIX transMatrix = DirectX::XMMatrixTranslation(m_center.x, m_center.y, m_center.z);

	//Update the wMatrix and the angle.
	DirectX::XMMATRIX result = scaleMatrix * rotMatrix  * transMatrix;
	DirectX::XMStoreFloat4x4(&this->m_wMatrix, DirectX::XMMatrixTranspose(result));
	//Angle change depends on planets radius (smaller planet = faster spin)
	angle += 0.001f * static_cast<float>(this->m_timer.DeltaTime()) * (250 / this->m_radius);

	//Update the matrixBuffer.
	D3D11_MAPPED_SUBRESOURCE mappedSubresource;

	DirectX::XMFLOAT4X4 wvpMatrix;
	DirectX::XMMATRIX vMatrix = DirectX::XMLoadFloat4x4(&VMatrix);
	DirectX::XMMATRIX pMatrix = DirectX::XMLoadFloat4x4(&PMatrix);

	DirectX::XMStoreFloat4x4(&wvpMatrix, DirectX::XMMatrixTranspose(result * vMatrix * pMatrix));

	deviceContext->Map(this->m_model->getMatrixBuffer().Get(),
					   0,
					   D3D11_MAP_WRITE_DISCARD,
					   0,
					   &mappedSubresource);

	memcpy(mappedSubresource.pData, &m_wMatrix, sizeof(DirectX::XMFLOAT4X4));
	memcpy(((char*)mappedSubresource.pData) + sizeof(DirectX::XMFLOAT4X4), &wvpMatrix, sizeof(DirectX::XMFLOAT4X4));

	deviceContext->Unmap(this->m_model->getMatrixBuffer().Get(), 0);
	
	//Bounding sphere:
	m_model->GetBoundingSphere()->Center = m_center;

	//Update the water sphere.
	if(m_waterSphere)
		m_waterSphere->updateSphere(VMatrix, PMatrix, deviceContext, m_center.x, m_center.y, m_center.z);
	return nullptr;
}

void CosmicBody::bindUniques(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& deviceContext) {

	deviceContext->IASetVertexBuffers(0u,
									  1u,
									  this->m_model->getVertexBuffer().GetAddressOf(),
									  &this->m_model->getStride(),
									  &this->m_model->getOffset());
	deviceContext->IASetIndexBuffer(this->m_model->getIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);
	deviceContext->VSSetConstantBuffers(0u, 1u, this->m_model->getMatrixBuffer().GetAddressOf());
}

GameObject* CosmicBody::GetOrbit()
{
	return static_cast<GameObject*>(m_orbit);
}

void CosmicBody::BindShadowUniques(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext)
{
	pDeviceContext->IASetVertexBuffers(0u,
									  1u,
									  this->m_model->getVertexBuffer().GetAddressOf(),
									  &this->m_model->getStride(),
									  &this->m_model->getOffset());
	pDeviceContext->IASetIndexBuffer(this->m_model->getIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);
}

float CosmicBody::GetRadius() {
	return m_radius;
}

int CosmicBody::GetRotDir() {
	return m_rotationDir;
}

const float& CosmicBody::GetOrbitalSpeed() const noexcept
{
	return m_orbital_speed;
}
