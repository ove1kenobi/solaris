#include "pch.h"
#include "CosmicBody.h"

CosmicBody::CosmicBody() noexcept
	:	m_radius{ 0.0f }, m_yAxis{ 0.0f, 1.0f, 0.0f, 0.0f }
{
	
}

bool CosmicBody::init(float x, float y, float z, float r, float xRot, float zRot, int rotDir) {
	//Set initial values. All ranomized.
	this->m_radius = r;
	this->m_mass = r * 100000;
	this->m_1byMass = 1.0f / m_mass;
	this->m_center.x = x;
	this->m_center.y = y;
	this->m_center.z = z;
	this->m_pitch = xRot;
	this->m_roll = zRot;
	this->m_rotationDir = rotDir;
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
	
	//Generate the sphere.
	this->m_model = ModelFactory::Get().GenerateSphere(x, y, z, r);
	
	return true;
}

bool CosmicBody::update(DirectX::XMMATRIX VMatrix, DirectX::XMMATRIX PMatrix, const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& deviceContext) {
	static float angle = 0.0f;

	//Construct rotation matrices
	DirectX::XMMATRIX scaleMatrix = DirectX::XMMatrixIdentity();
	DirectX::XMMATRIX rotX = DirectX::XMMatrixRotationX(this->m_pitch);
	DirectX::XMMATRIX rotZ = DirectX::XMMatrixRotationZ(this->m_roll);
	//Angle change is positive or negative depending on a randomized value PER PLANET.
	DirectX::XMMATRIX rotMatrix = DirectX::XMMatrixRotationAxis(DirectX::XMLoadFloat4(&this->m_yAxis), angle * this->m_rotationDir);
	
	//Construct the translation matrix.
	DirectX::XMMATRIX transMatrix = DirectX::XMMatrixIdentity();
	//DirectX::XMFLOAT4X4 transMatrixFloats;
	//DirectX::XMStoreFloat4x4(&transMatrixFloats, transMatrix);
	//transMatrixFloats._41 = getTransVector().x;
	//transMatrixFloats._42 = getTransVector().y;
	//transMatrixFloats._43 = getTransVector().z;
	//transMatrix = DirectX::XMLoadFloat4x4(&transMatrixFloats);
	transMatrix = DirectX::XMMatrixTranslation(m_center.x, m_center.y, m_center.z);

	//Update the wMatrix and the angle.
	DirectX::XMMATRIX result = scaleMatrix * rotX * rotZ * rotMatrix  * transMatrix;
	DirectX::XMStoreFloat4x4(&this->m_wMatrix, result);
	//Angle change depends on planets radius (smaller planet = faster spin)
	angle += 0.001f * static_cast<float>(this->m_timer.DeltaTime()) * (1000 / this->m_radius);

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

void CosmicBody::bindUniques(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& deviceContext) {

	deviceContext->IASetVertexBuffers(
		0u,
		1u,
		this->m_model->getVertexBuffer().GetAddressOf(),
		&this->m_model->getStride(),
		&this->m_model->getOffset()
	);

	deviceContext->IASetIndexBuffer(this->m_model->getIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);

	deviceContext->VSSetConstantBuffers(0, 1, this->m_model->getMatrixBuffer().GetAddressOf());
}