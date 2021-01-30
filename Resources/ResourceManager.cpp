#include "ResourceManager.h"

ResourceManager::ResourceManager() noexcept
	: m_pDevice{ nullptr }, m_pDeviceContext{ nullptr }
{

}

const bool ResourceManager::Initialize(Microsoft::WRL::ComPtr<ID3D11Device> pDevice, 
									   Microsoft::WRL::ComPtr<ID3D11DeviceContext> pDeviceContext) noexcept
{
	m_pDevice = pDevice;
	m_pDeviceContext = pDeviceContext;

	/*DEMO INITIALIZATION. THIS IS ONLY TEMPORARY (Emil F) */
	//if (!m_VertexShaderDemo.Create(m_pDevice, L"VertexShader_Minimalistic.hlsl"))
	//	return false;
	//
	//if (!m_PixelShaderDemo.Create(m_pDevice, L"PixelShader_Minimalistic.hlsl"))
	//	return false;
	//
	//if (!m_InputLayoutDemo.Create(m_pDevice, m_VertexShaderDemo))
	//	return false;
	//
	//if (!m_TopologyDemo.Create(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST))
	//	return false;
	

	return true;
}