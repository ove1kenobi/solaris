#include "../pch.h"
#include "GaussianBlur.h"

GaussianBlur::GaussianBlur() noexcept
	: m_pBlurredUAV{ nullptr },
	  m_pBlurredSRV{ nullptr },
	  m_ClearColor{ 0.0f, 0.0f, 0.0f, 1.0f }
{
}

const bool GaussianBlur::Initialize(const Microsoft::WRL::ComPtr<ID3D11Device>& pDevice, const unsigned int& textureWidth, const unsigned int& textureHeight)
{
	//We need 2 textures for the 2 blur passes, and their respective views:
	Microsoft::WRL::ComPtr<ID3D11Texture2D> pTexture2D = nullptr;
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
	HR(pDevice->CreateTexture2D(&textureDescriptor, nullptr, &pTexture2D), "CreateTexture2D");
	Microsoft::WRL::ComPtr<ID3D11Texture2D> pSecondTexture2D = nullptr;
	HR(pDevice->CreateTexture2D(&textureDescriptor, nullptr, &pSecondTexture2D), "CreateTexture2D");

	D3D11_UNORDERED_ACCESS_VIEW_DESC unorderedAccessViewDesc = {};
	unorderedAccessViewDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	unorderedAccessViewDesc.ViewDimension = D3D11_UAV_DIMENSION::D3D11_UAV_DIMENSION_TEXTURE2D;
	unorderedAccessViewDesc.Texture2D.MipSlice = 0u;
	HR(pDevice->CreateUnorderedAccessView(pTexture2D.Get(), &unorderedAccessViewDesc, m_pBlurredUAV.GetAddressOf()), "CreateUnorderedAccessView");
	HR(pDevice->CreateUnorderedAccessView(pSecondTexture2D.Get(), &unorderedAccessViewDesc, m_pSecondBlurredUAV.GetAddressOf()), "CreateUnorderedAccessView");

	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc = {};
	shaderResourceViewDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MipLevels = 1u;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0u;
	HR(pDevice->CreateShaderResourceView(pTexture2D.Get(), &shaderResourceViewDesc, &m_pBlurredSRV), "CreateShaderResourceView");
	HR(pDevice->CreateShaderResourceView(pSecondTexture2D.Get(), &shaderResourceViewDesc, &m_pSecondBlurredSRV), "CreateShaderResourceView");

	return true;
}

void GaussianBlur::PreparePass(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext)
{
	// We start by binding the compute shader pipeline:
	BindIDEvent event(BindID::ID_GaussianBlur); 
	EventBuss::Get().Delegate(event);

	//Now we bind the Unordered access view (The input texture is bound in the previous process clean up phase):
	pDeviceContext->ClearUnorderedAccessViewFloat(m_pBlurredUAV.Get(), m_ClearColor);
	pDeviceContext->CSSetUnorderedAccessViews(0u, 1u, m_pBlurredUAV.GetAddressOf(), nullptr);
}

void GaussianBlur::DoPass(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext)
{
	pDeviceContext->Dispatch(118u, 66u, 1u);
	ID3D11UnorderedAccessView* nullUAV[1] = { nullptr };
	ID3D11ShaderResourceView* nullSRV[1] = { nullptr };
	bool regular = false;
	//We do 3 passes, to increase the blurred effect, which spreads the bloom out.
	//Because of non-HDR, this has a marginal effect, but it is there.
	//We alternate between two textures, the one we wrote to is the one we read from etc:
	for (unsigned int i{ 0u }; i < 2; i++)
	{
		if (regular)
		{
			pDeviceContext->CSSetShaderResources(0u, 1u, nullSRV);
			pDeviceContext->ClearUnorderedAccessViewFloat(m_pBlurredUAV.Get(), m_ClearColor);
			pDeviceContext->CSSetUnorderedAccessViews(0u, 1u, nullUAV, nullptr);
			pDeviceContext->CSSetUnorderedAccessViews(0u, 1u, m_pBlurredUAV.GetAddressOf(), nullptr);
			pDeviceContext->CSSetShaderResources(0u, 1u, m_pSecondBlurredSRV.GetAddressOf());
			pDeviceContext->Dispatch(118u, 66u, 1u);
		}
		else
		{
			pDeviceContext->CSSetShaderResources(0u, 1u, nullSRV);
			pDeviceContext->ClearUnorderedAccessViewFloat(m_pSecondBlurredUAV.Get(), m_ClearColor);
			pDeviceContext->CSSetUnorderedAccessViews(0u, 1u, nullUAV, nullptr);
			pDeviceContext->CSSetUnorderedAccessViews(0u, 1u, m_pSecondBlurredUAV.GetAddressOf(), nullptr);
			pDeviceContext->CSSetShaderResources(0u, 1u, m_pBlurredSRV.GetAddressOf());
			pDeviceContext->Dispatch(118u, 66u, 1u);
		}
		regular = !regular;
	}
}	

void GaussianBlur::CleanUpPass(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext)
{
	//We unbind the pipeline:
	UnbindPipelineEvent ubEvent;
	EventBuss::Get().Delegate(ubEvent);

	//Also unbind the unordered access view:
	ID3D11UnorderedAccessView* nullUAV[1] = { nullptr };
	pDeviceContext->CSSetUnorderedAccessViews(0u, 1u, nullUAV, nullptr);

	//Set the texture as a shader resource view:
	pDeviceContext->PSSetShaderResources(0u, 1u, m_pBlurredSRV.GetAddressOf());
}