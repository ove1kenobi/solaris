#include "..\pch.h"
#include "CubeTexture.h"

CubeTexture::CubeTexture() noexcept
	: m_pShaderResourceView{ nullptr }, m_Slot{ 0u }
{
}

void CubeTexture::Bind(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext)
{
#if defined(DEBUG) | defined(_DEBUG)
	assert(pDeviceContext && m_pShaderResourceView);
#endif
	pDeviceContext->PSSetShaderResources(m_Slot, 1u, m_pShaderResourceView.GetAddressOf());
}

void CubeTexture::Unbind(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext)
{
#if defined(DEBUG) | defined(_DEBUG)
	assert(pDeviceContext);
#endif
	ID3D11ShaderResourceView* nullSRV[1] = { nullptr };
	pDeviceContext->PSSetShaderResources(m_Slot, 1u, nullSRV);
}

const bool CubeTexture::Create(const Microsoft::WRL::ComPtr<ID3D11Device>& pDevice, const LPCWSTR& fileName, const unsigned int& slot)
{
	m_Slot = slot;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> pTexture2D = nullptr;
	HR(DirectX::CreateDDSTextureFromFileEx(pDevice.Get(),
										   fileName,
										   0u,
										   D3D11_USAGE_IMMUTABLE,
										   D3D11_BIND_SHADER_RESOURCE,
										   0u,
										   D3D11_RESOURCE_MISC_TEXTURECUBE,
										   false,
										   (ID3D11Resource**)(pTexture2D.ReleaseAndGetAddressOf()),
										   &m_pShaderResourceView, 
										   nullptr), 
										   "CreateDDSTextureFromFileEx");
	return true;
}
