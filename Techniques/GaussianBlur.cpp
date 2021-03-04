#include "../pch.h"
#include "GaussianBlur.h"

GaussianBlur::GaussianBlur() noexcept
	: m_pHorizontalBlurUAV{ nullptr },
	  m_pFinalBlurredUAV{ nullptr },
	  m_pHorizontalBlurSRV{ nullptr },
	  m_pFinalBlurredSRV{ nullptr },
	  m_ClearColor{ 0.0f, 0.0f, 0.0f, 1.0f }
{
}

const bool GaussianBlur::Initialize(const Microsoft::WRL::ComPtr<ID3D11Device>& pDevice, const unsigned int& textureWidth, const unsigned int& textureHeight)
{
	//We need 2 textures for the 2 blur passes, and their respecitve views:
	Microsoft::WRL::ComPtr<ID3D11Texture2D> pFirstPassTexture = nullptr;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> pSecondPassTexture = nullptr;
	D3D11_TEXTURE2D_DESC textureDescriptor = {};
	textureDescriptor.Width = textureWidth;
	textureDescriptor.Height = textureHeight;
	textureDescriptor.MipLevels = 1u;
	textureDescriptor.ArraySize = 1u;
	textureDescriptor.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDescriptor.SampleDesc.Count = 1u;
	textureDescriptor.SampleDesc.Quality = 0u;
	textureDescriptor.Usage = D3D11_USAGE_DEFAULT;
	textureDescriptor.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
	textureDescriptor.CPUAccessFlags = 0U;
	textureDescriptor.MiscFlags = 0u;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> pTextureBright = nullptr;
	HR(pDevice->CreateTexture2D(&textureDescriptor, nullptr, &pFirstPassTexture), "CreateTexture2D");
	HR(pDevice->CreateTexture2D(&textureDescriptor, nullptr, &pSecondPassTexture), "CreateTexture2D");

	D3D11_UNORDERED_ACCESS_VIEW_DESC unorderedAccessViewDesc = {};
	unorderedAccessViewDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	unorderedAccessViewDesc.ViewDimension = D3D11_UAV_DIMENSION::D3D11_UAV_DIMENSION_TEXTURE2D;
	unorderedAccessViewDesc.Texture2D.MipSlice = 0u;
	HR(pDevice->CreateUnorderedAccessView(pFirstPassTexture.Get(), &unorderedAccessViewDesc, m_pHorizontalBlurUAV.GetAddressOf()), "CreateUnorderedAccessView");
	HR(pDevice->CreateUnorderedAccessView(pSecondPassTexture.Get(), &unorderedAccessViewDesc, m_pFinalBlurredUAV.GetAddressOf()), "CreateUnorderedAccessView");

	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc = {};
	shaderResourceViewDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MipLevels = 1u;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0u;
	HR(pDevice->CreateShaderResourceView(pFirstPassTexture.Get(), &shaderResourceViewDesc, &m_pHorizontalBlurSRV), "CreateShaderResourceView");
	HR(pDevice->CreateShaderResourceView(pSecondPassTexture.Get(), &shaderResourceViewDesc, &m_pFinalBlurredSRV), "CreateShaderResourceView");

	return true;
}

void GaussianBlur::PrepareHorizontalBlurPass(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext)
{
	// We start by binding the compute shader pipeline:
	BindIDEvent event(BindID::ID_GaussianBlurHorizontal); 
	EventBuss::Get().Delegate(event);

	//Now we bind the Unordered access view (The input texture is bound in the previous process clean up phase):
	pDeviceContext->ClearUnorderedAccessViewFloat(m_pHorizontalBlurUAV.Get(), m_ClearColor);
	pDeviceContext->CSSetUnorderedAccessViews(0u, 1u, m_pHorizontalBlurUAV.GetAddressOf(), nullptr);
}

void GaussianBlur::DoHorizontalPass(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext)
{
	pDeviceContext->Dispatch(2u, 1024u, 1u);
}

void GaussianBlur::CleanUpHorizontalPass(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext)
{
	
}

void GaussianBlur::PrepareVerticalBlurPass(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext)
{
	// We start by binding the compute shader pipeline:
	BindIDEvent event(BindID::ID_GaussianBlurVertical);
	EventBuss::Get().Delegate(event);

	//Now we bind the Unordered access view (The input texture is bound in the previous process clean up phase):
	pDeviceContext->ClearUnorderedAccessViewFloat(m_pFinalBlurredUAV.Get(), m_ClearColor);
	pDeviceContext->CSSetUnorderedAccessViews(0u, 1u, m_pFinalBlurredUAV.GetAddressOf(), nullptr);
}

void GaussianBlur::DoVerticalBlurPass(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext)
{
	pDeviceContext->Dispatch(0u, 1u, 0u);
}

void GaussianBlur::CleanUpVerticalBlurPass(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext)
{
}
