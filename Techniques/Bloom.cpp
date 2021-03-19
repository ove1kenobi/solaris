#include "../pch.h"
#include "Bloom.h"

Bloom::Bloom() noexcept
	: m_pRenderTargetViewBright{ nullptr },
	  m_pShaderResourceViewBright{ nullptr},
	  m_ClearColor{ 0.0f, 0.0f, 0.0f, 1.0f }
{
}

const bool Bloom::Initialize(const Microsoft::WRL::ComPtr<ID3D11Device>& pDevice)
{
	D3D11_TEXTURE2D_DESC textureDescriptor = {};
	textureDescriptor.Width = 1920;
	textureDescriptor.Height = 1080;
	textureDescriptor.MipLevels = 1u;
	textureDescriptor.ArraySize = 1u;
	textureDescriptor.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDescriptor.SampleDesc.Count = 1u;
	textureDescriptor.SampleDesc.Quality = 0u;
	textureDescriptor.Usage = D3D11_USAGE_DEFAULT;
	textureDescriptor.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_RENDER_TARGET | D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE;
	textureDescriptor.CPUAccessFlags = 0U;
	textureDescriptor.MiscFlags = 0u;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> pTextureBright = nullptr;
	HR(pDevice->CreateTexture2D(&textureDescriptor, 
								nullptr, 
								&pTextureBright), 
								"CreateTexture2D");
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDescriptor = {};
	renderTargetViewDescriptor.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	renderTargetViewDescriptor.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDescriptor.Texture2D.MipSlice = 0u;
	HR(pDevice->CreateRenderTargetView(pTextureBright.Get(), 
									   &renderTargetViewDescriptor, 
									   &m_pRenderTargetViewBright), 
									   "CreateRenderTargetView");
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc = {};
	shaderResourceViewDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MipLevels = 1u;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0u;
	HR(pDevice->CreateShaderResourceView(pTextureBright.Get(), 
										 &shaderResourceViewDesc, 
										 &m_pShaderResourceViewBright), 
										 "CreateShaderResourceView");

	if (!m_GaussianBlur.Initialize(pDevice, 1920, 1080))
		return false;

	return true;
}

void Bloom::PrepareLumaExtractionPass(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext)
{
	//Prepare the pass that will extract the colors over a certain brightness threshold:
	pDeviceContext->ClearRenderTargetView(m_pRenderTargetViewBright.Get(), m_ClearColor);
	ID3D11DepthStencilView* nullDSV = nullptr;
	pDeviceContext->OMSetRenderTargets(1u, m_pRenderTargetViewBright.GetAddressOf(), nullDSV);

	BindIDEvent bindEvent(BindID::ID_BloomLuma);
	EventBuss::Get().Delegate(bindEvent);

	ToggleDepthStencilStateEvent dsEvent;
	EventBuss::Get().Delegate(dsEvent);
}

void Bloom::DoLumaExtractionPass(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext)
{
	//We render a quad, so we always use index count = 6.
	pDeviceContext->DrawIndexed(6u, 0u, 0u);
}

void Bloom::CleanUpLumaExtractionPass(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext)
{
	//We unbind the pipeline, making sure to follow that up with binding the render texture as a compute shader resource:
	UnbindPipelineEvent ubEvent;
	EventBuss::Get().Delegate(ubEvent);
	ID3D11RenderTargetView* nullRTV[1] = { nullptr };
	ID3D11DepthStencilView* nullDSV = nullptr;
	pDeviceContext->OMSetRenderTargets(1u, nullRTV, nullDSV);
	pDeviceContext->CSSetShaderResources(0u, 1u, m_pShaderResourceViewBright.GetAddressOf());
}

void Bloom::DoBlurPass(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext) noexcept
{
	m_GaussianBlur.PreparePass(pDeviceContext);
	m_GaussianBlur.DoPass(pDeviceContext);
	m_GaussianBlur.CleanUpPass(pDeviceContext);
}

void Bloom::PrepareCombinePass(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext)
{
	//Bind the correct setup of bindables (The blurred image is already bound in previous clean up phase):
	BindIDEvent event(BindID::ID_BloomCombine);
	EventBuss::Get().Delegate(event);
}

void Bloom::DoCombinePass(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext)
{
	/*Post process pass, so we render a quad.
	  This pass combines the colors from the original color texture yielded from the 
	  water pass with the colors from the brightened and blurred colors from the Gaussian filter pass.
	*/
	pDeviceContext->DrawIndexed(6u, 0u, 0u);
}

void Bloom::CleanUpCombinePass(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext)
{
	ToggleDepthStencilStateEvent dsEvent;
	EventBuss::Get().Delegate(dsEvent);
}
