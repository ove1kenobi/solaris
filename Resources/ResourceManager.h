#pragma once
/*Resource managern skall kunna ta emot best?llningar p? vad f?r bindables
den skall binda och d?refter utf?ra detta. (Ex: F?r varje best?lld bindable -> bind.)
Den kommer kunna ta emot en s?dan best?llning via ett event som antingen inneh?ller en ?vergripande
best?llning (Ge mig alla bindables som motsvarar mot detta ENDA ID), eller kanske ist?llet
kan man t?nka best?llningen mer som en kundvagn: "H?r ?r alla de varor jag vill ha, samla ihop och bind dessa",
Vi f?r d? kanske en bit-masked best?llning ist?llet. Jag vet EJ vilken strategi som ?r l?mpligast.
Den senare ?r mer granul?r.*/
#include "IBindable.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "PrimitiveTopology.h"
#include "InputLayout.h"
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
public:
	ResourceManager() noexcept;
	virtual ~ResourceManager() noexcept = default;
	[[nodiscard]] const bool Initialize(Microsoft::WRL::ComPtr<ID3D11Device> device, Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext, std::vector<vertex> vertexBuffer, std::vector<UINT> indexBuffer, DirectX::XMMATRIX WMatrix, DirectX::XMMATRIX VMatrix, DirectX::XMMATRIX PMatrix) noexcept;
	[[nodiscard]] const bool CreateAllBindables();
	void UnbindPipeline();
	[[nodiscard]] const bool Demo(std::vector<vertex> vertexArray, std::vector<UINT> indexBuffer, DirectX::XMMATRIX WMatrix, DirectX::XMMATRIX VMatrix, DirectX::XMMATRIX PMatrix); //TODO: Remove once event system is implemented.
	void OnEvent(IEvent& event) noexcept override;
	struct MatrixBuffer {
		DirectX::XMMATRIX WMatrix;
		DirectX::XMMATRIX VMatrix;
		DirectX::XMMATRIX PMatrix;
	};
};