#pragma once
#include "IBindable.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "PrimitiveTopology.h"
#include "InputLayout.h"
#include "../EventSystem/RenderEvents.h"
#include "..\EventSystem\EventBuss.h"
#include "..\EventSystem\IEventListener.h"
#include <vector>
#include <DirectXMath.h>
class ResourceManager : public IEventListener
{
private:
	Microsoft::WRL::ComPtr<ID3D11Device> m_pDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_pDeviceContext;
	VertexShader m_VertexShaderMinimal;
	PixelShader m_PixelShaderMinimal;
	InputLayout m_InputLayoutCosmicBody;
	InputLayout m_InputLayoutPlayerModel;
	InputLayout m_InputLayoutMinimal;
	PrimitiveTopology m_TopologyTriList;
	std::vector<IBindable*> m_BindablesMinimalistic;
	std::vector<IBindable*> m_BindablesCosmic;
	std::vector<IBindable*> m_BindablesPlayer;
public:
	ResourceManager() noexcept;
	virtual ~ResourceManager() noexcept = default;
	[[nodiscard]] const bool Initialize(Microsoft::WRL::ComPtr<ID3D11Device> device, Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext, std::vector<float> vertexBuffer, std::vector<int> indexBuffer, DirectX::XMMATRIX WMatrix, DirectX::XMMATRIX VMatrix, DirectX::XMMATRIX PMatrix) noexcept;
	[[nodiscard]] const bool CreateAllBindables();
	void UnbindPipeline();
	void BindToPipeline(IEvent& event);
	[[nodiscard]] const bool Demo(std::vector<float> vertexArray, std::vector<int> indexBuffer, DirectX::XMMATRIX WMatrix, DirectX::XMMATRIX VMatrix, DirectX::XMMATRIX PMatrix); //TODO: Remove once event system is implemented.
	void OnEvent(IEvent& event) noexcept override;
	struct MatrixBuffer {
		DirectX::XMMATRIX WMatrix;
		DirectX::XMMATRIX VMatrix;
		DirectX::XMMATRIX PMatrix;
	};
};