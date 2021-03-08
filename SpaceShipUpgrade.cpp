#include "pch.h"
#include "SpaceShipUpgrade.h"

SpaceShipUpgrade::SpaceShipUpgrade()
	: m_Tag{ "Upgrade" }, m_TestForCulling{ false }
{
	m_center = { 1000.0f, 1000.0f, 1000.0f };
}

SpaceShipUpgrade::SpaceShipUpgrade(std::string modelFile)
{
	m_model = ModelFactory::Get().GetModel(modelFile);
}

GameObject* SpaceShipUpgrade::update(DirectX::XMFLOAT4X4 VMatrix, DirectX::XMFLOAT4X4 PMatrix, const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& deviceContext)
{
	return nullptr;
}

void SpaceShipUpgrade::bindUniques(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& deviceContext)
{
	for (auto tex : m_model->GetTextures())
	{
		if (tex) tex->Bind(deviceContext);
	}
	deviceContext->IASetVertexBuffers(0u,
		1u,
		this->m_model->getVertexBuffer().GetAddressOf(),
		&this->m_model->getStride(),
		&this->m_model->getOffset());

	deviceContext->IASetIndexBuffer(this->m_model->getIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);
	//deviceContext->VSSetConstantBuffers(0, 1, this->m_model->getMatrixBuffer().GetAddressOf());
}

void SpaceShipUpgrade::BindShadowUniques(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext)
{
	pDeviceContext->IASetVertexBuffers(0u,
		1u,
		this->m_model->getVertexBuffer().GetAddressOf(),
		&this->m_model->getStride(),
		&this->m_model->getOffset());
	pDeviceContext->IASetIndexBuffer(this->m_model->getIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);
}

const bool SpaceShipUpgrade::IntersectRayObject(const DirectX::XMFLOAT3* origin, const DirectX::XMFLOAT3* direction, float& distance) noexcept
{
	return false;
}
const std::string& SpaceShipUpgrade::GetTag() const noexcept
{
	return m_Tag;
}

const bool& SpaceShipUpgrade::ShallBeTestedForCulling() const noexcept
{
	return m_TestForCulling;
}
