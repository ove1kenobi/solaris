#pragma once
#include "IBindable.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "ComputeShader.h"
#include "PrimitiveTopology.h"
#include "InputLayout.h"
#include "CubeTexture.h"
#include "SamplerState.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "GBuffer.h"
#include "..\vertex.h"
#include "../EventSystem/RenderEvents.h"
#include "../EventSystem/UtilityEvents.h"
#include "..\EventSystem\WindowEvents.h"
#include "..\EventSystem\EventBuss.h"
#include "..\EventSystem\IEventListener.h"
class ResourceManager : public IEventListener
{
private:
	Microsoft::WRL::ComPtr<ID3D11Device> m_pDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_pDeviceContext;
	VertexShader m_VertexShaderMinimal;
	VertexShader m_VertexShaderSkybox;
	VertexShader m_VertexShaderOrbit;
	VertexShader m_VertexShaderSun;
	VertexShader m_VertexShaderPostProcessing;
	VertexShader m_VertexShaderWaterSpheres;
	VertexShader m_VertexShaderShadow;
	PixelShader m_PixelShaderMinimal;
	PixelShader m_PixelShaderSkybox;
	PixelShader m_PixelShaderOrbit;
	PixelShader m_PixelShaderSun;
	PixelShader m_PixelShaderPostProcessing;
	PixelShader m_PixelShaderWaterSpheres;
	PixelShader m_PixelShaderShadow;
	PixelShader m_PixelShaderBloomLuma;
	ComputeShader m_ComputeShaderPlanet;
	InputLayout m_InputLayoutCosmicBody;
	InputLayout m_InputLayoutPlayerModel;
	InputLayout m_InputLayoutMinimal;
	InputLayout m_InputLayoutPostProcessing;
	InputLayout m_InputLayoutWaterSpheres;
	InputLayout m_InputLayoutPositionOnly;
	PrimitiveTopology m_TopologyTriList;
	PrimitiveTopology m_TopologyPatchList;
	PrimitiveTopology m_TopologyLineStrip;
	CubeTexture m_CubeTextureSkybox;
	SamplerState m_SamplerSkybox;
	SamplerState m_SamplerShadow;
	SamplerState m_SamplerPostProcessing;
	VertexBuffer m_VertexBufferCube;
	IndexBuffer m_IndexBufferCube;
	VertexBuffer m_VertexBufferQuad;
	IndexBuffer m_IndexBufferQuad;
	GBuffer m_GBuffer;
	std::vector<IBindable*> m_BindablesMinimalistic;
	std::vector<IBindable*> m_BindablesRenderQuad;
	std::vector<IBindable*> m_BindablesWater;
	std::vector<IBindable*> m_BindablesPlayer;
	std::vector<IBindable*> m_BindablesSkybox;
	std::vector<IBindable*> m_BindablesOrbit;
	std::vector<IBindable*> m_BindablesSun;
	std::vector<IBindable*> m_BindablesShadow;
	std::vector<IBindable*> m_BindablesWaterSpheres;
	std::vector<IBindable*> m_BindablesBloomLuna;
	std::vector<DirectX::XMFLOAT3> m_CubeVertices;
	std::vector<unsigned int> m_CubeIndices;
	std::vector<Vertex_PosTex> m_QuadVertices;
	std::vector<unsigned int> m_QuadIndices;
	unsigned int m_ClientWindowWidth;
	unsigned int m_ClientWindowHeight;
private:
	[[nodiscard]] const bool CreateAllBindables();
	void UnbindPipeline();
	void BindToPipeline(IEvent& event);
	void UpdateDXHandlers(IEvent& event) noexcept;
	void UpdateResolution(IEvent& event) noexcept;
	void OnEvent(IEvent& event) noexcept override;
	void CreateCubeData() noexcept;
	void CreateQuadData() noexcept;
public:
	ResourceManager() noexcept;
	virtual ~ResourceManager() noexcept = default;
	[[nodiscard]] const bool Initialize() noexcept;
};