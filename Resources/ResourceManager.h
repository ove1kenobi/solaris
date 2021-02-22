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
#include "..\EventSystem\EventBuss.h"
#include "..\EventSystem\IEventListener.h"
class ResourceManager : public IEventListener
{
private:
	UINT m_screenWidth;
	UINT m_screenHeight;
	Microsoft::WRL::ComPtr<ID3D11Device> m_pDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_pDeviceContext;
	VertexShader m_VertexShaderMinimal;
	VertexShader m_VertexShaderSkybox;
	VertexShader m_VertexShaderPostProcessing;
	PixelShader m_PixelShaderMinimal;
	PixelShader m_PixelShaderSkybox;
	PixelShader m_PixelShaderPostProcessing;
	ComputeShader m_ComputeShaderPlanet;
	InputLayout m_InputLayoutCosmicBody;
	InputLayout m_InputLayoutPlayerModel;
	InputLayout m_InputLayoutMinimal;
	InputLayout m_InputLayoutSkybox;
	InputLayout m_InputLayoutPostProcessing;
	PrimitiveTopology m_TopologyTriList;
	PrimitiveTopology m_TopologyPatchList;
	CubeTexture m_CubeTextureSkybox;
	SamplerState m_SamplerSkybox;
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
	std::vector<DirectX::XMFLOAT3> m_CubeVertices;
	std::vector<unsigned int> m_CubeIndices;
	std::vector<Vertex_PosTex> m_QuadVertices;
	std::vector<unsigned int> m_QuadIndices;
private:
	[[nodiscard]] const bool CreateAllBindables();
	void UnbindPipeline();
	void BindToPipeline(IEvent& event);
	void UpdateDXHandlers(IEvent& event) noexcept;
	void OnEvent(IEvent& event) noexcept override;
	void CreateCubeData() noexcept;
	void CreateQuadData() noexcept;
public:
	ResourceManager() noexcept;
	virtual ~ResourceManager() noexcept = default;
	[[nodiscard]] const bool Initialize(UINT screenWidth, UINT screenHeight) noexcept;
};