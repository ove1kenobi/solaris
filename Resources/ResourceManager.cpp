#include "..\pch.h"
#include "ResourceManager.h"

ResourceManager::ResourceManager() noexcept
	: m_pDevice{ nullptr }, m_pDeviceContext{ nullptr }, m_ClientWindowWidth{ 1920u }, m_ClientWindowHeight{ 1080u }
{
	EventBuss::Get().AddListener(this, EventType::UnbindPipelineEvent, EventType::BindIDEvent, EventType::DelegateDXEvent);
	EventBuss::Get().AddListener(this, EventType::DelegateResolutionEvent);
}

ResourceManager::~ResourceManager() {
	EventBuss::Get().RemoveListener(this, EventType::UnbindPipelineEvent);
	EventBuss::Get().RemoveListener(this, EventType::BindIDEvent);
	EventBuss::Get().RemoveListener(this, EventType::DelegateDXEvent);
	EventBuss::Get().RemoveListener(this, EventType::DelegateResolutionEvent);
}

const bool ResourceManager::Initialize() noexcept
{
	CreateCubeData();
	CreateQuadData();
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
	if (!m_VertexShaderOrbit.Create(m_pDevice, L"VertexShader_Orbit.hlsl"))
		return false;
	if (!m_VertexShaderPostProcessing.Create(m_pDevice, L"VertexShader_PostProcessing.hlsl"))
		return false;
	if (!m_VertexShaderWaterSpheres.Create(m_pDevice, L"VertexShader_WaterSphere.hlsl"))
		return false;
	if (!m_VertexShaderShadow.Create(m_pDevice, L"VertexShader_Shadow.hlsl"))
		return false;
	if (!m_VertexShaderTextured.Create(m_pDevice, L"VertexShader_Textured.hlsl"))
		return false;
	//Pixel Shaders:
	if (!m_PixelShaderMinimal.Create(m_pDevice, L"PixelShader_Minimalistic.hlsl"))
		return false;
	if (!m_PixelShaderSkybox.Create(m_pDevice, L"PixelShader_Skybox.hlsl"))
		return false;
	if (!m_PixelShaderOrbit.Create(m_pDevice, L"PixelShader_Orbit.hlsl"))
		return false;
	if (!m_PixelShaderSun.Create(m_pDevice, L"PixelShader_Sun.hlsl"))
		return false;
	if (!m_PixelShaderPostProcessing.Create(m_pDevice, L"PixelShader_PostProcessing.hlsl"))
		return false;
	if (!m_PixelShaderWaterSpheres.Create(m_pDevice, L"PixelShader_WaterSphere.hlsl"))
		return false;
	if (!m_PixelShaderShadow.Create(m_pDevice, L"PixelShader_Shadow.hlsl"))
		return false;
	if (!m_PixelShaderTextured.Create(m_pDevice, L"PixelShader_Textured.hlsl"))
		return false;
	if (!m_PixelShaderBloomLuma.Create(m_pDevice, L"PixelShader_BLoomLuma.hlsl"))
		return false;
	if (!m_PixelShaderBloomCombine.Create(m_pDevice, L"PixelShader_BloomCombine.hlsl"))
		return false;
	//Geometry Shaders:

	//Hull Shaders:

	//Domain Shaders:

	//Compute Shaders:
	if (!m_ComputeShaderPlanet.Create(m_pDevice, L"ComputeShader_Planet.hlsl"))
		return false;
	if (!m_ComputeShaderGaussianBlur.Create(m_pDevice, L"ComputeShader_Blur.hlsl"))
		return false;
	//InputLayouts:
	if (!m_InputLayoutMinimal.Create(m_pDevice, m_VertexShaderMinimal, LAYOUT_MINIMAL))
		return false;
	if (!m_InputLayoutPositionOnly.Create(m_pDevice, m_VertexShaderSkybox, LAYOUT_POSITION))
		return false;
	if (!m_InputLayoutPostProcessing.Create(m_pDevice, m_VertexShaderPostProcessing, LAYOUT_POSTPROCESSING))
		return false;
	if (!m_InputLayoutWaterSpheres.Create(m_pDevice, m_VertexShaderWaterSpheres, LAYOUT_WATERSPHERES))
		return false;
	if (!m_InputLayoutPlayerModel.Create(m_pDevice, m_VertexShaderTextured, LAYOUT_PLAYER))
		return false;
	//Primitive topologies:
	if (!m_TopologyTriList.Create(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST))
		return false;
	if (!m_TopologyPatchList.Create(D3D11_PRIMITIVE_TOPOLOGY_11_CONTROL_POINT_PATCHLIST))
		return false;
	if (!m_TopologyLineStrip.Create(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP))
		return false;
	//Samplers:
	if (!m_SamplerSkybox.Create(m_pDevice, BindFlag::S_PS, TechFlag::SKYBOX, 0u))
		return false;
	if (!m_SamplerPostProcessing.Create(m_pDevice, BindFlag::S_PS, TechFlag::POSTPROCESSING, 0u))
		return false;
	if (!m_SamplerShadow.Create(m_pDevice, BindFlag::S_PS, TechFlag::SHADOW, 0u))
		return false;
	//Textures:
	if (!m_GBuffer.Create(m_pDevice, m_ClientWindowWidth, m_ClientWindowHeight))
		return false;
	if (!m_CubeTextureSkybox.Create(m_pDevice, L"skymap.dds", 0u))
		return false;
	//Vertex Buffers:
		//Skybox
	if (!m_VertexBufferCube.Create(m_pDevice, D3D11_USAGE::D3D11_USAGE_IMMUTABLE, 0u, 
								   static_cast<UINT>(m_CubeVertices.size() * 12), sizeof(Vertex_Position), m_CubeVertices.data(), 0u))
		return false;
	if (!m_IndexBufferCube.Create(m_pDevice, D3D11_USAGE::D3D11_USAGE_IMMUTABLE, 0u,
								  static_cast<UINT>(m_CubeIndices.size() * sizeof(unsigned int)), sizeof(unsigned int), m_CubeIndices.data()))
		return false;
		//Quad
	if (!m_VertexBufferQuad.Create(m_pDevice, D3D11_USAGE::D3D11_USAGE_IMMUTABLE, 0u,
								  static_cast<UINT>(m_QuadVertices.size() * 20), sizeof(Vertex_PosTex), m_QuadVertices.data(), 0u))
		return false;
	if (!m_IndexBufferQuad.Create(m_pDevice, D3D11_USAGE::D3D11_USAGE_IMMUTABLE, 0u,
								  static_cast<UINT>(m_QuadIndices.size() * sizeof(unsigned int)), sizeof(unsigned int), m_QuadIndices.data()))
		return false;
	//Arrange:
	//Minimal:
	m_BindablesMinimalistic.insert(m_BindablesMinimalistic.end(), { &m_VertexShaderMinimal, &m_PixelShaderMinimal, &m_InputLayoutMinimal, &m_TopologyTriList });
	//Skybox:
	m_BindablesSkybox.insert(m_BindablesSkybox.end(), { &m_VertexShaderSkybox, &m_PixelShaderSkybox, &m_InputLayoutPositionOnly,
														&m_TopologyTriList, &m_CubeTextureSkybox, &m_SamplerSkybox,
														&m_VertexBufferCube, &m_IndexBufferCube});
	// Orbits
	m_BindablesOrbit.insert(m_BindablesOrbit.end(), { &m_VertexShaderOrbit, &m_PixelShaderOrbit, &m_InputLayoutPositionOnly, &m_TopologyLineStrip });
	// Sun
	m_BindablesSun.insert(m_BindablesSun.end(), { &m_VertexShaderTextured, &m_PixelShaderSun,
												&m_InputLayoutPlayerModel, &m_TopologyTriList, &m_SamplerSkybox });

	//RenderQuad First Pass:
	m_BindablesRenderQuad.insert(m_BindablesRenderQuad.end(), { &m_VertexShaderMinimal, &m_PixelShaderMinimal, &m_InputLayoutMinimal, &m_TopologyTriList, &m_GBuffer });

	//Water Post processing:
	m_BindablesWater.insert(m_BindablesWater.end(), { &m_VertexShaderPostProcessing, &m_PixelShaderPostProcessing, &m_InputLayoutPostProcessing, 
							&m_TopologyTriList, &m_GBuffer, &m_VertexBufferQuad, &m_IndexBufferQuad, &m_SamplerShadow });
	//Water spheres
	m_BindablesWaterSpheres.insert(m_BindablesWaterSpheres.end(), { &m_VertexShaderWaterSpheres, &m_PixelShaderWaterSpheres, &m_InputLayoutWaterSpheres, &m_TopologyTriList });
	//Shadow mapping:
	m_BindablesShadow.insert(m_BindablesShadow.end(), { &m_VertexShaderShadow, &m_PixelShaderShadow, &m_InputLayoutPositionOnly, &m_TopologyTriList });
	//Bloom Luma:
	m_BindablesBloomLuna.insert(m_BindablesBloomLuna.end(), { &m_VertexShaderPostProcessing, &m_PixelShaderBloomLuma, &m_InputLayoutPostProcessing,
															  &m_TopologyTriList, &m_VertexBufferQuad, &m_IndexBufferQuad });
	//Gaussian Blur:
	m_BindablesGaussianBlur.insert(m_BindablesGaussianBlur.end(), { &m_ComputeShaderGaussianBlur});
	//Bloom Combine:
	m_BindablesBloomCombine.insert(m_BindablesBloomCombine.end(), { &m_VertexShaderPostProcessing, &m_PixelShaderBloomCombine, &m_InputLayoutPostProcessing,
															  &m_TopologyTriList, &m_VertexBufferQuad, &m_IndexBufferQuad });
	//Textured models
	m_BindablesTextured.insert(m_BindablesTextured.end(), { &m_VertexShaderTextured, &m_PixelShaderTextured, 
															&m_InputLayoutPlayerModel, &m_TopologyTriList, &m_SamplerSkybox });
	return true;
}

void ResourceManager::UnbindPipeline()
{
	ID3D11ShaderResourceView*	nullSRV[6] = { nullptr };
	ID3D11SamplerState*			nullSampler[6] = { nullptr };
	ID3D11Buffer*				nullBuffer[6] = { nullptr };

	m_pDeviceContext->VSSetShader(nullptr, nullptr, 0u);
	m_pDeviceContext->VSSetShaderResources(0u, 6u, nullSRV);
	m_pDeviceContext->VSSetSamplers(0u, 6u, nullSampler);
	m_pDeviceContext->VSSetConstantBuffers(0u, 6u, nullBuffer);

	m_pDeviceContext->PSSetShader(nullptr, nullptr, 0u);
	m_pDeviceContext->PSSetShaderResources(0u, 6u, nullSRV);
	m_pDeviceContext->PSSetSamplers(0u, 6u, nullSampler);
	m_pDeviceContext->PSSetConstantBuffers(0u, 6u, nullBuffer);

	m_pDeviceContext->DSSetShader(nullptr, nullptr, 0u);
	m_pDeviceContext->DSSetShaderResources(0u, 6u, nullSRV);
	m_pDeviceContext->DSSetSamplers(0u, 6u, nullSampler);
	m_pDeviceContext->DSSetConstantBuffers(0u, 6u, nullBuffer);

	m_pDeviceContext->HSSetShader(nullptr, nullptr, 0u);
	m_pDeviceContext->HSSetShaderResources(0u, 6u, nullSRV);
	m_pDeviceContext->HSSetSamplers(0u, 6u, nullSampler);
	m_pDeviceContext->HSSetConstantBuffers(0u, 6u, nullBuffer);

	m_pDeviceContext->GSSetShader(nullptr, nullptr, 0u);
	m_pDeviceContext->GSSetShaderResources(0u, 6u, nullSRV);
	m_pDeviceContext->GSSetSamplers(0u, 6u, nullSampler);
	m_pDeviceContext->GSSetConstantBuffers(0u, 6u, nullBuffer);

	m_pDeviceContext->CSSetShader(nullptr, nullptr, 0u);
	m_pDeviceContext->CSSetShaderResources(0u, 6u, nullSRV);
	m_pDeviceContext->CSSetConstantBuffers(0u, 0u, nullBuffer);
}

void ResourceManager::BindToPipeline(IEvent& event)
{
	BindIDEvent& derivedEvent = static_cast<BindIDEvent&>(event);
	switch (derivedEvent.GetBindID())
	{
	case BindID::ID_Minimal:
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
	case BindID::ID_RenderQuad:
	{
		for (auto bindables : m_BindablesRenderQuad)
		{
			if (!bindables->IsBound())
			{
				bindables->Bind(m_pDeviceContext);
			}
		}
		break;
	}
	case BindID::ID_Player:
	{
		for (auto bindables : m_BindablesPlayer)
		{
			if (!bindables->IsBound())
			{
				bindables->Bind(m_pDeviceContext);
			}
		}
		break;
	case BindID::ID_PlanetHeight:
		if (!m_ComputeShaderPlanet.IsBound())
		{
			m_ComputeShaderPlanet.Bind(m_pDeviceContext);
		}
		break;
	}
	case BindID::ID_Skybox:
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
	case BindID::ID_Orbit:
	{
		for (auto bindables : m_BindablesOrbit)
		{
			if (!bindables->IsBound())
			{
				bindables->Bind(m_pDeviceContext);
			}
		}
		break;
	}
	case BindID::ID_SUN:
	{
		for (auto bindables : m_BindablesSun)
		{
			if (!bindables->IsBound())
			{
				bindables->Bind(m_pDeviceContext);
			}
		}
		break;
	}
	case BindID::ID_Shadow:
	{
		for (auto bindables : m_BindablesShadow)
		{
			if (!bindables->IsBound())
			{
				bindables->Bind(m_pDeviceContext);
			}
		}
		break;
	}
	case BindID::ID_WaterSphere:
	{
		for (auto bindables : m_BindablesWaterSpheres)
		{
			if (!bindables->IsBound())
			{
				bindables->Bind(m_pDeviceContext);
			}
		}
		break;
	}
	case BindID::ID_Water:
	{
		for (auto bindables : m_BindablesWater)
		{
			if (!bindables->IsBound())
			{
				bindables->Bind(m_pDeviceContext);
			}
		}
		break;
	}
	case BindID::ID_BloomLuma:
	{
		for (auto bindables : m_BindablesBloomLuna)
		{
			if (!bindables->IsBound())
			{
				bindables->Bind(m_pDeviceContext);
			}
		}
		break;
	}
	case BindID::ID_GaussianBlur:
	{
		for (auto bindables : m_BindablesGaussianBlur)
		{
			if (!bindables->IsBound())
			{
				bindables->Bind(m_pDeviceContext);
			}
		}
		break;
	}
	case BindID::ID_BloomCombine:
	{
		for (auto bindables : m_BindablesBloomCombine)
		{
			if (!bindables->IsBound())
			{
				bindables->Bind(m_pDeviceContext);
			}
		}
		break;
	}
	case BindID::ID_Textured:
	{
		for (auto bindables : m_BindablesTextured)
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

void ResourceManager::UpdateResolution(IEvent& event) noexcept
{
	DelegateResolutionEvent& derivedEvent = static_cast<DelegateResolutionEvent&>(event);
	m_ClientWindowWidth = derivedEvent.GetClientWindowWidth();
	m_ClientWindowHeight = derivedEvent.GetClientWindowHeight();
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
	case EventType::DelegateResolutionEvent:
		UpdateResolution(event);
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

void ResourceManager::CreateQuadData() noexcept
{
	m_QuadVertices.insert(m_QuadVertices.end(), {	{	DirectX::XMFLOAT3(-1.0f, -1.0f, 0.0f),	// Bottom left
														DirectX::XMFLOAT2(0.0f, 0.0f)},
													{	DirectX::XMFLOAT3(1.0f, 1.0f, 0.0f),	// Top right
														DirectX::XMFLOAT2(1.0f, 1.0f)},
													{	DirectX::XMFLOAT3(-1.0f, 1.0f, 0.0f),	// Top left
														DirectX::XMFLOAT2(0.0f, 1.0f)},
													{	DirectX::XMFLOAT3(1.0f, -1.0f, 0.0f),	// Bottom right
														DirectX::XMFLOAT2(1.0f, 0.0f)}
												});
	m_QuadIndices.insert(m_QuadIndices.end(), { 0, 2, 3,  3, 2, 1 });
}