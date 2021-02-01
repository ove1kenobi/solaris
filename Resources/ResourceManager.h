#pragma once
/*Resource managern skall kunna ta emot beställningar på vad för bindables
den skall binda och därefter utföra detta. (Ex: För varje beställd bindable -> bind.)
Den kommer kunna ta emot en sådan beställning via ett event som antingen innehåller en övergripande
beställning (Ge mig alla bindables som motsvarar mot detta ENDA ID), eller kanske istället
kan man tänka beställningen mer som en kundvagn: "Här är alla de varor jag vill ha, samla ihop och bind dessa",
Vi får då kanske en bit-masked beställning istället. Jag vet EJ vilken strategi som är lämpligast.
Den senare är mer granulär.*/
#include "IBindable.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "PrimitiveTopology.h"
#include "InputLayout.h"
#include <vector>
#include <DirectXMath.h>
class ResourceManager
{
private:
	Microsoft::WRL::ComPtr<ID3D11Device> m_pDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_pDeviceContext;
	VertexShader m_VertexShaderDemo;
	PixelShader m_PixelShaderDemo;
	InputLayout m_InputLayoutDemo;
	PrimitiveTopology m_TopologyDemo;
public:
	ResourceManager() noexcept;
	virtual ~ResourceManager() noexcept = default;
	[[nodiscard]] const bool Initialize(Microsoft::WRL::ComPtr<ID3D11Device> device, Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext, std::vector<float> vertexBuffer, std::vector<int> indexBuffer, DirectX::XMMATRIX WMatrix, DirectX::XMMATRIX VMatrix, DirectX::XMMATRIX PMatrix) noexcept;
	[[nodiscard]] const bool Demo(std::vector<float> vertexArray, std::vector<int> indexBuffer, DirectX::XMMATRIX WMatrix, DirectX::XMMATRIX VMatrix, DirectX::XMMATRIX PMatrix); //TODO: Remove once event system is implemented.

	struct MatrixBuffer {
		DirectX::XMMATRIX WMatrix;
		DirectX::XMMATRIX VMatrix;
		DirectX::XMMATRIX PMatrix;
	};
};