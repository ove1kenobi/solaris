#include "pch.h"
#include "WaterSphere.h"

WaterSphere::WaterSphere() noexcept :
	m_radius{ 0 },
	tag{ "WaterSphere" },
	testForCulling{ false }
{

}

bool WaterSphere::Initialize(float x, float y, float z, float r) {

	this->m_radius = r;
	this->m_center.x = x;
	this->m_center.y = y;
	this->m_center.z = z;

	//Initialize the wMatrix.
	DirectX::XMStoreFloat4x4(&this->m_wMatrix, DirectX::XMMatrixIdentity());
	DirectX::XMMATRIX final = DirectX::XMMatrixTranslation(x, y, z);

	DirectX::XMStoreFloat4x4(&this->m_wMatrix, final);

	//Generate the Water Sphere.
	this->m_model = ModelFactory::Get().GenerateWaterSphere(x, y, z, r);

	return true;
}

void WaterSphere::updateSphere(DirectX::XMFLOAT4X4 VMatrix, DirectX::XMFLOAT4X4 PMatrix, const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& deviceContext, float x, float y, float z) {

	m_center.x = x;
	m_center.y = y;
	m_center.z = z;

	update(VMatrix, PMatrix, deviceContext);
}

GameObject* WaterSphere::update(DirectX::XMFLOAT4X4 VMatrix, DirectX::XMFLOAT4X4 PMatrix, const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& deviceContext) {
	DirectX::XMMATRIX result = DirectX::XMMatrixTranslation(m_center.x, m_center.y, m_center.z);
	DirectX::XMStoreFloat4x4(&this->m_wMatrix, result);

	//Update the matrixBuffer.
	D3D11_MAPPED_SUBRESOURCE mappedSubresource;

	DirectX::XMFLOAT4X4 wvpMatrix;
	DirectX::XMMATRIX vMatrix = DirectX::XMLoadFloat4x4(&VMatrix);
	DirectX::XMMATRIX pMatrix = DirectX::XMLoadFloat4x4(&PMatrix);

	DirectX::XMStoreFloat4x4(&wvpMatrix, DirectX::XMMatrixTranspose(result * vMatrix * pMatrix));

	//Already transposed in Cosmicbody
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

	return nullptr;
}

void WaterSphere::bindUniques(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& deviceContext) {

	deviceContext->IASetVertexBuffers(0u,
		1u,
		this->m_model->getVertexBuffer().GetAddressOf(),
		&this->m_model->getStride(),
		&this->m_model->getOffset());
	deviceContext->IASetIndexBuffer(this->m_model->getIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);

	ID3D11Buffer* nullBuffer[6] = { nullptr };
	deviceContext->VSSetConstantBuffers(0u, 6u, nullBuffer);
	deviceContext->VSSetConstantBuffers(0u, 1u, this->m_model->getMatrixBuffer().GetAddressOf());
}

void WaterSphere::BindShadowUniques(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext)
{
	//We simply return here...
}

[[nodiscard]] const std::string& WaterSphere::GetTag() const noexcept {
	return tag;
}

[[nodiscard]] const bool& WaterSphere::ShallBeTestedForCulling() const noexcept {
	return testForCulling;
}