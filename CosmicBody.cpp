#include "CosmicBody.h"

CosmicBody::CosmicBody() noexcept
	:	m_radius{ 0.0f }, m_yAxis{ 0.0f, 1.0f, 0.0f, 0.0f }
{
	
}

bool CosmicBody::init(float x, float y, float z, float r, float xRot, float zRot) {
	this->m_radius = r;
	this->m_center.x = x;
	this->m_center.y = y;
	this->m_center.z = z;
	this->m_pitch = xRot;
	this->m_roll = zRot;

	DirectX::XMMATRIX rotX = DirectX::XMMatrixRotationX(this->m_pitch);
	DirectX::XMMATRIX rotZ = DirectX::XMMatrixRotationZ(this->m_roll);
	DirectX::XMVECTOR resultY = DirectX::XMLoadFloat4(&this->m_yAxis);
	resultY = DirectX::XMVector4Transform(resultY, rotX);
	resultY = DirectX::XMVector4Transform(resultY, rotZ);
	DirectX::XMStoreFloat4(&this->m_yAxis, resultY);

	DirectX::XMStoreFloat4x4(&this->m_wMatrix, DirectX::XMMatrixIdentity());
	DirectX::XMMATRIX trans = DirectX::XMMatrixTranslation(x, y, z);
	DirectX::XMMATRIX final = trans;

	DirectX::XMStoreFloat4x4(&this->m_wMatrix, final);
	

	this->m_model = ModelFactory::Get().GenerateSphere(x, y, z, r);
	
	//createSphere();

	return true;
}

bool CosmicBody::update(DirectX::XMMATRIX VMatrix, DirectX::XMMATRIX PMatrix, const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& deviceContext) {
	DirectX::XMFLOAT4 yAxis;
	
	yAxis.x = this->m_wMatrix._12;
	yAxis.y = this->m_wMatrix._22;
	yAxis.z = this->m_wMatrix._32;
	
	//DirectX::XMMATRIX wMatrix = DirectX::XMLoadFloat4x4(&this->m_wMatrix);
	/*
	DirectX::XMFLOAT4X4 zero;
	DirectX::XMStoreFloat4x4(&zero, DirectX::XMMatrixIdentity());
	
	zero._41 = 0;
	zero._42 = 0;
	zero._43 = 0;
	zero._44 = 0;
	DirectX::XMMATRIX zeroMatrix = DirectX::XMLoadFloat4x4(&zero);
	wMatrix = DirectX::XMMatrixMultiply(zeroMatrix, wMatrix);
	*/
	
	static float angle = 0.0f;
	DirectX::XMMATRIX scaleMatrix = DirectX::XMMatrixIdentity();
	DirectX::XMMATRIX rotMatrix = DirectX::XMMatrixRotationAxis(DirectX::XMLoadFloat4(&yAxis), (1000 / this->m_radius) * angle);
	
	DirectX::XMMATRIX transMatrix = DirectX::XMMatrixIdentity();
	DirectX::XMFLOAT4X4 transMatrixFloats;
	DirectX::XMStoreFloat4x4(&transMatrixFloats, transMatrix);
	transMatrixFloats._41 = getTransVector().x;
	transMatrixFloats._42 = getTransVector().y;
	transMatrixFloats._43 = getTransVector().z;
	transMatrix = DirectX::XMLoadFloat4x4(&transMatrixFloats);

	//wMatrix = DirectX::XMMatrixTranslation(this->m_wMatrix._41, this->m_wMatrix._42, this->m_wMatrix._43);
	DirectX::XMMATRIX result = scaleMatrix * rotMatrix * transMatrix;
	DirectX::XMStoreFloat4x4(&this->m_wMatrix, result);
	angle += 0.001 * this->m_timer.DeltaTime();

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

	// Set the buffer.
	deviceContext->IASetIndexBuffer(this->m_model->getIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);

	deviceContext->VSSetConstantBuffers(0, 1, this->m_model->getMatrixBuffer().GetAddressOf());
}