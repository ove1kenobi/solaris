#include "pch.h"
#include "ModelTexture.h"

void ModelTexture::LoadTexture(const Microsoft::WRL::ComPtr<ID3D11Device>& pDevice, const LPCWSTR& file, UINT type) noexcept
{
	Microsoft::WRL::ComPtr<ID3D11Texture2D> pTexture2D = nullptr;

	DirectX::CreateWICTextureFromFile(pDevice.Get(), file, 
					(ID3D11Resource**)(pTexture2D.ReleaseAndGetAddressOf()), &m_shaderResourceView);
#if defined(DEBUG) | defined(_DEBUG)
	assert(m_shaderResourceView);
#endif
}

void ModelTexture::Bind(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext)
{
#if defined(DEBUG) | defined(_DEBUG)
	assert(pDeviceContext && m_shaderResourceView);
#endif
	pDeviceContext->PSSetShaderResources(m_Slot, 1u, m_shaderResourceView.GetAddressOf());
}

void ModelTexture::Unbind(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext)
{
#if defined(DEBUG) | defined(_DEBUG)
	assert(pDeviceContext);
#endif
	ID3D11ShaderResourceView* nullSRV[1] = { nullptr };
	pDeviceContext->PSSetShaderResources(m_Slot, 1u, nullSRV);
}