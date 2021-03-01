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

bool Orbit::update(DirectX::XMMATRIX VMatrix, DirectX::XMMATRIX PMatrix, const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& deviceContext)
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

	deviceContext->Unmap(m_model->getMatrixBuffer().Get(), 0);
	return true;
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
