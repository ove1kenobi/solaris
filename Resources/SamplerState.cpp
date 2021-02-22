#include "..\pch.h"
#include "SamplerState.h"

SamplerState::SamplerState() noexcept
	: m_pSamplerState{ nullptr }, m_BindFlag{ BindFlag::S_None }, m_Slot{ 0u }
{

}

void SamplerState::Bind(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext)
{
#if defined(DEBUG) | defined(_DEBUG)
	assert(pDeviceContext && m_pSamplerState);
#endif
	switch (m_BindFlag)
	{
	case BindFlag::S_PS:
		pDeviceContext->PSSetSamplers(m_Slot, 1u, m_pSamplerState.GetAddressOf());
			break;
	case BindFlag::S_DS:
		pDeviceContext->DSSetSamplers(m_Slot, 1u, m_pSamplerState.GetAddressOf());
		break;
	}
}

void SamplerState::Unbind(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext)
{
#if defined(DEBUG) | defined(_DEBUG)
	assert(pDeviceContext && m_pSamplerState);
#endif
	ID3D11SamplerState* nullSampler[1] = {nullptr};
	switch (m_BindFlag)
	{
	case BindFlag::S_PS:
		pDeviceContext->PSSetSamplers(m_Slot, 1u, nullSampler);
		break;
	case BindFlag::S_DS:
		pDeviceContext->DSSetSamplers(m_Slot, 1u, nullSampler);
		break;
	}
}

const bool SamplerState::Create(const Microsoft::WRL::ComPtr<ID3D11Device>& pDevice, const BindFlag& bindFlag, const TechFlag& techFlag, const unsigned int& slot)
{
	if (techFlag == TechFlag::SKYBOX)
	{
		m_Slot = slot;
		m_BindFlag = bindFlag;
		D3D11_SAMPLER_DESC samplerDescriptor = {};
		samplerDescriptor.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDescriptor.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDescriptor.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDescriptor.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDescriptor.ComparisonFunc = D3D11_COMPARISON_NEVER;
		samplerDescriptor.MinLOD = 0.0f;
		samplerDescriptor.MaxLOD = D3D11_FLOAT32_MAX;
		HR(pDevice->CreateSamplerState(&samplerDescriptor, &m_pSamplerState), "CreateSamplerState");
		return true;
	}
	else if (techFlag == TechFlag::SHADOW)
	{
		m_Slot = slot;
		m_BindFlag = bindFlag;
		D3D11_SAMPLER_DESC samplerDescriptor = {};
		samplerDescriptor.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		samplerDescriptor.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDescriptor.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDescriptor.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDescriptor.ComparisonFunc = D3D11_COMPARISON_LESS_EQUAL;
		samplerDescriptor.BorderColor[0] = 0.0f;
		samplerDescriptor.BorderColor[1] = 0.0f;
		samplerDescriptor.BorderColor[2] = 0.0f;
		samplerDescriptor.BorderColor[3] = 0.0f;
		HR(pDevice->CreateSamplerState(&samplerDescriptor, &m_pSamplerState), "CreateSamplerState");
		return true;
	}
	return false;
}