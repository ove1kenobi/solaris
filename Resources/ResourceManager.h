#pragma once
#include "IBindable.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "PrimitiveTopology.h"
#include "InputLayout.h"
#include "../EventSystem/RenderEvents.h"
#include "..\EventSystem\EventBuss.h"
#include "..\EventSystem\IEventListener.h"
#include "..\Model.h"
#include <vector>
#include <DirectXMath.h>
class ResourceManager : public IEventListener
{
private:
	Microsoft::WRL::ComPtr<ID3D11Device> m_pDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_pDeviceContext;
	VertexShader m_VertexShaderMinimal;
	PixelShader m_PixelShaderMinimal;
	InputLayout m_InputLayoutDefault;
	PrimitiveTopology m_TopologyTriList;
	std::vector<IBindable*> m_BindablesMinimalistic;
public:
	ResourceManager() noexcept;
	virtual ~ResourceManager() noexcept = default;
	[[nodiscard]] const bool Initialize(Microsoft::WRL::ComPtr<ID3D11Device> device, Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext) noexcept;
	[[nodiscard]] const bool CreateAllBindables();
	void UnbindPipeline();
	void BindToPipeline(IEvent& event);
	void OnEvent(IEvent& event) noexcept override;
};