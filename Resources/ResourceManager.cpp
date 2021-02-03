#include "ResourceManager.h"

ResourceManager::ResourceManager() noexcept
	: m_pDevice{ nullptr }, m_pDeviceContext{ nullptr }
{

}

const bool ResourceManager::Initialize(
	Microsoft::WRL::ComPtr<ID3D11Device> pDevice, 
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pDeviceContext
	) noexcept
{
	m_pDevice = pDevice;
	m_pDeviceContext = pDeviceContext;
	EventBuss::Get().AddListener(this, EventType::UnbindPipelineEvent, EventType::BindIDEvent);

	if (!CreateAllBindables())
		return false;

	return true;
}

const bool ResourceManager::CreateAllBindables()
{
	//Vertex Shaders:
	if (!m_VertexShaderMinimal.Create(m_pDevice, L"VertexShader_Minimalistic.hlsl"))
		return false;
	//Pixel Shaders:
	if (!m_PixelShaderMinimal.Create(m_pDevice, L"PixelShader_Minimalistic.hlsl"))
		return false;
	//Geometry Shaders:

	//Hull Shaders:

	//Domain Shaders:

	//InputLayouts:
	if (!m_InputLayoutMinimal.Create(m_pDevice, m_VertexShaderMinimal, LAYOUT_MINIMAL))
		return false;
	//if (!m_InputLayoutCosmicBody.Create(m_pDevice, m_VertexShaderCosmic, LAYOUT_COSMIC))
	//	return false;
	//if (!m_InputLayoutPlayerModel.Create(m_pDevice, m_VertexShaderPlayer, LAYOUT_PLAYER))
	//	return false;

	//Primitive topologies:
	if (!m_TopologyTriList.Create(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST))
		return false;
	//Samplers:

	//Arrange:
	//Minimal:
	m_BindablesMinimalistic.insert(m_BindablesMinimalistic.end(), { &m_VertexShaderMinimal, &m_PixelShaderMinimal, &m_InputLayoutMinimal, &m_TopologyTriList });
	return true;
}

void ResourceManager::UnbindPipeline()
{
	ID3D11ShaderResourceView* nullSRV[3] = { nullptr };

	m_pDeviceContext->VSSetShader(nullptr, nullptr, 0u);
	m_pDeviceContext->VSSetConstantBuffers(0u, 0u, nullptr);
	m_pDeviceContext->VSSetShaderResources(0u, 3u, nullSRV);

	m_pDeviceContext->PSSetShader(nullptr, nullptr, 0u);
	m_pDeviceContext->PSSetSamplers(0u, 0u, nullptr);
	m_pDeviceContext->PSSetShaderResources(0u, 3u, nullSRV);

	m_pDeviceContext->DSSetShader(nullptr, nullptr, 0u);
	m_pDeviceContext->DSSetShaderResources(0u, 3u, nullSRV);
	m_pDeviceContext->DSSetConstantBuffers(0u, 0u, nullptr);

	m_pDeviceContext->HSSetShader(nullptr, nullptr, 0u);
	m_pDeviceContext->HSSetShaderResources(0u, 3u, nullSRV);
	m_pDeviceContext->HSSetConstantBuffers(0u, 0u, nullptr);

	m_pDeviceContext->GSSetShader(nullptr, nullptr, 0u);
	m_pDeviceContext->GSSetShaderResources(0u, 3u, nullSRV);
	m_pDeviceContext->HSSetConstantBuffers(0u, 0u, nullptr);
}

void ResourceManager::BindToPipeline(IEvent& event)
{
	BindIDEvent& derivedEvent = static_cast<BindIDEvent&>(event);
	switch (derivedEvent.GetBindID())
	{
	case BindID::ID_Minimal :
		for (auto bindables : m_BindablesMinimalistic)
		{
			if (!bindables->IsBound())
			{
				bindables->Bind(m_pDeviceContext);
			}
		}
		break;
	case BindID::ID_Cosmic :
		for (auto bindables : m_BindablesCosmic)
		{
			if (!bindables->IsBound())
			{
				bindables->Bind(m_pDeviceContext);
			}
		}
		break;
	case BindID::ID_Player :
		for (auto bindables : m_BindablesPlayer)
		{
			if (!bindables->IsBound())
			{
				bindables->Bind(m_pDeviceContext);
			}
		}
		break;
	}
}

void ResourceManager::OnEvent(IEvent& event) noexcept
{
	switch (event.GetEventType())
	{
	case EventType::UnbindPipelineEvent :
		UnbindPipeline();
			break;

	case EventType::BindIDEvent :
		BindToPipeline(event);
		break;
	}
}
