#include "pch.h"
#include "Orbit.h"

Orbit::Orbit() noexcept
	:	m_Tag{ "Orbit"},
		m_TestForCulling{ false }
{

}

Orbit::~Orbit()
{
}

bool Orbit::init(float major_semi_axis, float minor_semi_axis)
{
	m_model = ModelFactory::Get().GenerateOrbit(major_semi_axis, minor_semi_axis);
	return true;
}

GameObject* Orbit::update(DirectX::XMFLOAT4X4 VMatrix, DirectX::XMFLOAT4X4 PMatrix, const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& deviceContext)
{
	m_sumForces = { 0.0f, 0.0f, 0.0f };

	//Updated the same way as a cosmicbody, with S * R * T. Rotation is around the ships up vector.
	DirectX::XMVECTOR up = DirectX::XMLoadFloat3(&m_upVector);
	//100 times smaller. TODO: make variable?
	DirectX::XMMATRIX scale = DirectX::XMMatrixIdentity();
	DirectX::XMMATRIX rot = DirectX::XMMatrixIdentity();
	DirectX::XMMATRIX trans = DirectX::XMMatrixTranslation(m_center.x, m_center.y, m_center.z);
	DirectX::XMMATRIX result = scale * rot * trans;
	DirectX::XMStoreFloat4x4(&m_wMatrix, result);

	//Update the matrixBuffer.

	D3D11_MAPPED_SUBRESOURCE mappedSubresource;

	DirectX::XMFLOAT4X4 wvpMatrix;
	DirectX::XMMATRIX vMatrix = DirectX::XMLoadFloat4x4(&VMatrix);
	DirectX::XMMATRIX pMatrix = DirectX::XMLoadFloat4x4(&PMatrix);

	DirectX::XMStoreFloat4x4(&wvpMatrix, DirectX::XMMatrixTranspose(result * vMatrix * pMatrix));

	deviceContext->Map(
		this->m_model->getMatrixBuffer().Get(),
		0,
		D3D11_MAP_WRITE_DISCARD,
		0,
		&mappedSubresource
	);

	memcpy(mappedSubresource.pData, &m_wMatrix, sizeof(DirectX::XMFLOAT4X4));
	memcpy(((char*)mappedSubresource.pData) + sizeof(DirectX::XMFLOAT4X4), &wvpMatrix, sizeof(DirectX::XMFLOAT4X4));

	deviceContext->Unmap(m_model->getMatrixBuffer().Get(), 0);
	return nullptr;
}

void Orbit::bindUniques(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& deviceContext)
{
	deviceContext->IASetVertexBuffers(0u,
		1u,
		m_model->getVertexBuffer().GetAddressOf(),
		&m_model->getStride(),
		&m_model->getOffset());
	deviceContext->IASetIndexBuffer(m_model->getIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);
	deviceContext->VSSetConstantBuffers(0, 1, m_model->getMatrixBuffer().GetAddressOf());
}

void Orbit::BindShadowUniques(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext)
{
	//Do nothing as of now...
}

const bool Orbit::IntersectRayObject(const DirectX::FXMVECTOR& origin, const DirectX::FXMVECTOR& direction, float& distance) noexcept
{
	return false;
}

const std::string& Orbit::GetTag() const noexcept
{
	return m_Tag;
}

const bool& Orbit::ShallBeTestedForCulling() const noexcept
{
	return m_TestForCulling;
}
