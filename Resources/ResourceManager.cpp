#include "..\pch.h"
#include "ResourceManager.h"

ResourceManager::ResourceManager() noexcept
	: m_pDevice{ nullptr }, m_pDeviceContext{ nullptr }
{
	EventBuss::Get().AddListener(this, EventType::UnbindPipelineEvent, EventType::BindIDEvent, EventType::DelegateDXEvent);
}

const bool ResourceManager::Initialize() noexcept
{
	CreateCubeData();
	if (!CreateAllBindables())
		return false;

	return true;
}

const bool ResourceManager::CreateAllBindables()
{
	//Vertex Shaders:
	if (!m_VertexShaderMinimal.Create(m_pDevice, L"VertexShader_Minimalistic.hlsl"))
		return false;
	if (!m_VertexShaderSkybox.Create(m_pDevice, L"VertexShader_Skybox.hlsl"))
		return false;
	//Pixel Shaders:
	if (!m_PixelShaderMinimal.Create(m_pDevice, L"PixelShader_Minimalistic.hlsl"))
		return false;
	if (!m_PixelShaderSkybox.Create(m_pDevice, L"PixelShader_Skybox.hlsl"))
		return false;
	//Geometry Shaders:

	//Hull Shaders:

	//Domain Shaders:

	//InputLayouts:
	if (!m_InputLayoutMinimal.Create(m_pDevice, m_VertexShaderMinimal, LAYOUT_MINIMAL))
		return false;
	if (!m_InputLayoutSkybox.Create(m_pDevice, m_VertexShaderSkybox, LAYOUT_SKYBOX))
		return false;
	//Primitive topologies:
	if (!m_TopologyTriList.Create(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST))
		return false;
	if (!m_TopologyPatchList.Create(D3D11_PRIMITIVE_TOPOLOGY_11_CONTROL_POINT_PATCHLIST))
		return false;
	//Samplers:
	if (!m_SamplerSkybox.Create(m_pDevice, BindFlag::S_PS, TechFlag::SKYBOX, 0u))
		return false;
	//Textures:
	if (!m_CubeTextureSkybox.Create(m_pDevice, L"skymap.dds", 0u))
		return false;
	//Vertex Buffers:
	if (!m_VertexBufferCube.Create(m_pDevice, D3D11_USAGE::D3D11_USAGE_IMMUTABLE, 0u, 
								   static_cast<UINT>(m_CubeVertices.size() * 12), sizeof(Vertex_Position), m_CubeVertices.data(), 0u))
		return false;
	if (!m_IndexBufferCube.Create(m_pDevice, D3D11_USAGE::D3D11_USAGE_IMMUTABLE, 0u,
								  static_cast<UINT>(m_CubeIndices.size() * sizeof(unsigned int)), sizeof(unsigned int), m_CubeIndices.data()))
		return false;
	//Arrange:
	//Minimal:
	m_BindablesMinimalistic.insert(m_BindablesMinimalistic.end(), { &m_VertexShaderMinimal, &m_PixelShaderMinimal, &m_InputLayoutMinimal, &m_TopologyTriList });
	//Skybox:
	m_BindablesSkybox.insert(m_BindablesSkybox.end(), { &m_VertexShaderSkybox, &m_PixelShaderSkybox, &m_InputLayoutSkybox, 
														&m_TopologyTriList, &m_CubeTextureSkybox, &m_SamplerSkybox,
														&m_VertexBufferCube, &m_IndexBufferCube});
	return true;
}

void ResourceManager::UnbindPipeline()
{
	ID3D11ShaderResourceView* nullSRV[3] = { nullptr };
	ID3D11SamplerState* nullSampler[3] = { nullptr };
	ID3D11Buffer* nullBuffer[3] = { nullptr };

	m_pDeviceContext->VSSetShader(nullptr, nullptr, 0u);
	m_pDeviceContext->VSSetConstantBuffers(0u, 0u, nullptr);
	m_pDeviceContext->VSSetShaderResources(0u, 3u, nullSRV);
	m_pDeviceContext->VSSetSamplers(0u, 3u, nullSampler);
	m_pDeviceContext->VSSetConstantBuffers(0u, 3u, nullBuffer);

	m_pDeviceContext->PSSetShader(nullptr, nullptr, 0u);
	m_pDeviceContext->PSSetSamplers(0u, 0u, nullptr);
	m_pDeviceContext->PSSetShaderResources(0u, 3u, nullSRV);
	m_pDeviceContext->PSSetSamplers(0u, 3u, nullSampler);
	m_pDeviceContext->PSSetConstantBuffers(0u, 3u, nullBuffer);

	m_pDeviceContext->DSSetShader(nullptr, nullptr, 0u);
	m_pDeviceContext->DSSetShaderResources(0u, 3u, nullSRV);
	m_pDeviceContext->DSSetConstantBuffers(0u, 0u, nullptr);
	m_pDeviceContext->DSSetSamplers(0u, 3u, nullSampler);
	m_pDeviceContext->DSSetConstantBuffers(0u, 3u, nullBuffer);

	m_pDeviceContext->HSSetShader(nullptr, nullptr, 0u);
	m_pDeviceContext->HSSetShaderResources(0u, 3u, nullSRV);
	m_pDeviceContext->HSSetConstantBuffers(0u, 0u, nullptr);
	m_pDeviceContext->HSSetSamplers(0u, 3u, nullSampler);
	m_pDeviceContext->HSSetConstantBuffers(0u, 3u, nullBuffer);

	m_pDeviceContext->GSSetShader(nullptr, nullptr, 0u);
	m_pDeviceContext->GSSetShaderResources(0u, 3u, nullSRV);
	m_pDeviceContext->GSSetConstantBuffers(0u, 0u, nullptr);
	m_pDeviceContext->GSSetSamplers(0u, 3u, nullSampler);
	m_pDeviceContext->GSSetConstantBuffers(0u, 3u, nullBuffer);
}

void ResourceManager::BindToPipeline(IEvent& event)
{
	BindIDEvent& derivedEvent = static_cast<BindIDEvent&>(event);
	switch (derivedEvent.GetBindID())
	{
	case BindID::ID_Minimal :
	{
		for (auto bindables : m_BindablesMinimalistic)
		{
			if (!bindables->IsBound())
			{
				bindables->Bind(m_pDeviceContext);
			}
		}
		break;
	}
	case BindID::ID_Cosmic :
	{
		for (auto bindables : m_BindablesCosmic)
		{
			if (!bindables->IsBound())
			{
				bindables->Bind(m_pDeviceContext);
			}
		}
		break;
	}
	case BindID::ID_Player :
	{
		for (auto bindables : m_BindablesPlayer)
		{
			if (!bindables->IsBound())
			{
				bindables->Bind(m_pDeviceContext);
			}
		}
		break;
	}
	case BindID::ID_SKYBOX:
	{
		for (auto bindables : m_BindablesSkybox)
		{
			if (!bindables->IsBound())
			{
				bindables->Bind(m_pDeviceContext);
			}
		}
		break;
	}
	}
}

void ResourceManager::UpdateDXHandlers(IEvent& event) noexcept
{
	DelegateDXEvent& derivedEvent = static_cast<DelegateDXEvent&>(event);
	m_pDevice = derivedEvent.GetDevice();
	m_pDeviceContext = derivedEvent.GetDeviceContext();
#if defined(DEBUG) | defined(_DEBUG)
	assert(m_pDevice && m_pDeviceContext);
#endif
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
		
	case EventType::DelegateDXEvent :
		UpdateDXHandlers(event);
		break;
	}
}

void ResourceManager::CreateCubeData() noexcept
{
	m_CubeVertices.insert(m_CubeVertices.end(), { DirectX::XMFLOAT3(-1.0f, -1.0f, -1.0f),
												  DirectX::XMFLOAT3(1.0f, -1.0f, -1.0f),
												  DirectX::XMFLOAT3(-1.0f, 1.0f, -1.0f),
												  DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f),
												  DirectX::XMFLOAT3(-1.0f, -1.0f, 1.0f),
												  DirectX::XMFLOAT3(1.0f, -1.0f, 1.0f),
												  DirectX::XMFLOAT3(-1.0f, 1.0f, 1.0f),
												  DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f) });
	m_CubeIndices.insert(m_CubeIndices.end(), { 0,2,1, 2,3,1,
												1,3,5, 3,7,5,
												2,6,3, 3,6,7,
												4,5,7, 4,7,6,
												0,4,2, 2,4,6,
												0,1,4, 1,5,4 });
}
