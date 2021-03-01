#include "pch.h"
#include "CosmicBody.h"

CosmicBody::CosmicBody() noexcept
	: m_radius{ 0.0f }, m_yAxis{ 0.0f, 1.0f, 0.0f, 0.0f }, m_rotationDir{ 0 },
	m_tetheredTo{ nullptr }, m_major_semi_axis{ 0 }, m_minor_semi_axis{ 0 }, m_orbital_speed{ 0 },
	m_orbit{ nullptr }
{
	
}

CosmicBody::~CosmicBody()
{
}

bool CosmicBody::init(float x, float y, float z, float r, float xRot, float zRot, int rotDir, GameObject* tetherTo, Orbit* orbit) {
	//Set initial values. All randomized.
	this->m_radius = r;
	this->m_mass = r * 1000000000;
	this->m_center.x = x;
	this->m_center.y = y;
	this->m_center.z = z;
	this->m_pitch = xRot;
	this->m_roll = zRot;
	this->m_rotationDir = rotDir;
	this->m_tetheredTo = tetherTo;
	if(tetherTo && orbit)
	{
		this->m_major_semi_axis = length(m_center - tetherTo->GetCenter());
		this->m_minor_semi_axis = this->m_major_semi_axis * 0.8f;
		this->m_orbital_speed = 6.2831853f * m_major_semi_axis / m_mass * 100000.0f;
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

bool CosmicBody::update(DirectX::XMMATRIX VMatrix, DirectX::XMMATRIX PMatrix, const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& deviceContext)
{
	// Update orbit
	if (m_tetheredTo)
	{
		DirectX::XMFLOAT3 tc = m_tetheredTo->GetCenter();
		m_center.x = tc.x + m_major_semi_axis * static_cast<float>(cos(m_time.SinceStart() * m_orbital_speed));
		m_center.z = tc.z + m_minor_semi_axis * static_cast<float>(sin(m_time.SinceStart() * m_orbital_speed));
	}
	static float angle = 0.0f;

	//Construct rotation matrices
	DirectX::XMMATRIX scaleMatrix = DirectX::XMMatrixIdentity();
	DirectX::XMMATRIX rotX = DirectX::XMMatrixRotationX(this->m_pitch);
	DirectX::XMMATRIX rotZ = DirectX::XMMatrixRotationZ(this->m_roll);
	//Angle change is positive or negative depending on a randomized value PER PLANET.
	DirectX::XMMATRIX rotMatrix = DirectX::XMMatrixRotationAxis(DirectX::XMLoadFloat4(&this->m_yAxis), angle * this->m_rotationDir);
	
	//Construct the translation matrix.
	DirectX::XMMATRIX transMatrix = DirectX::XMMatrixIdentity();

	//TODO: EMIL H, review and remove!
	// Static or dynamic center coordinates
	//DirectX::XMFLOAT4X4 transMatrixFloats;
	//DirectX::XMStoreFloat4x4(&transMatrixFloats, transMatrix);
	//transMatrixFloats._41 = getTransVector().x;
	//transMatrixFloats._42 = getTransVector().y;
	//transMatrixFloats._43 = getTransVector().z;
	//transMatrix = DirectX::XMLoadFloat4x4(&transMatrixFloats);
	transMatrix = DirectX::XMMatrixTranslation(m_center.x, m_center.y, m_center.z);	// dynamic center coordinates

	//Update the wMatrix and the angle.
	DirectX::XMMATRIX result = scaleMatrix * rotX * rotZ * rotMatrix  * transMatrix;
	DirectX::XMStoreFloat4x4(&this->m_wMatrix, result);
	//Angle change depends on planets radius (smaller planet = faster spin)
	angle += 0.001f * static_cast<float>(this->m_timer.DeltaTime()) * (1000 / this->m_radius);

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
	
	//Bounding sphere:
	m_model->GetBoundingSphere()->Center = m_center;

	return true;
}

void CosmicBody::bindUniques(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& deviceContext) {

	deviceContext->IASetVertexBuffers(0u,
									  1u,
									  this->m_model->getVertexBuffer().GetAddressOf(),
									  &this->m_model->getStride(),
									  &this->m_model->getOffset());
	deviceContext->IASetIndexBuffer(this->m_model->getIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);
	deviceContext->VSSetConstantBuffers(0, 1, this->m_model->getMatrixBuffer().GetAddressOf());
}

GameObject* CosmicBody::GetOrbit()
{
	return static_cast<GameObject*>(m_orbit);
}