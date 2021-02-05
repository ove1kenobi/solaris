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
	pi = static_cast<float>(atan(1) * 4);
	this->m_rotationAngle = pi;
}

bool SpaceShip::update(DirectX::XMMATRIX VMatrix, DirectX::XMMATRIX PMatrix, const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& deviceContext)
{
	DirectX::XMVECTOR up = DirectX::XMLoadFloat3(&this->m_upVector);
	DirectX::XMMATRIX scale = DirectX::XMMatrixScaling(0.01f, 0.01f, 0.01f);
	DirectX::XMMATRIX rot = DirectX::XMMatrixRotationAxis(up, this->m_rotationAngle);
	DirectX::XMMATRIX trans = DirectX::XMMatrixTranslation(this->m_center.x, this->m_center.y, this->m_center.z);
	DirectX::XMMATRIX final = scale * rot * trans;
	DirectX::XMStoreFloat4x4(&this->m_wMatrix, final);
	//DirectX::XMStoreFloat4x4(&this->m_wMatrix, DirectX::XMMatrixTranslation(this->m_center.x, this->m_center.y, this->m_center.z));

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
	// Set the buffers.
	deviceContext->IASetIndexBuffer(this->m_model->getIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);
	deviceContext->VSSetConstantBuffers(0, 1, this->m_model->getMatrixBuffer().GetAddressOf());
}
