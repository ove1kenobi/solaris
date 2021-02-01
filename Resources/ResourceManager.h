#pragma once
/*Resource managern skall kunna ta emot best�llningar p� vad f�r bindables
den skall binda och d�refter utf�ra detta. (Ex: F�r varje best�lld bindable -> bind.)
Den kommer kunna ta emot en s�dan best�llning via ett event som antingen inneh�ller en �vergripande
best�llning (Ge mig alla bindables som motsvarar mot detta ENDA ID), eller kanske ist�llet
kan man t�nka best�llningen mer som en kundvagn: "H�r �r alla de varor jag vill ha, samla ihop och bind dessa",
Vi f�r d� kanske en bit-masked best�llning ist�llet. Jag vet EJ vilken strategi som �r l�mpligast.
Den senare �r mer granul�r.*/
#include "IBindable.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "PrimitiveTopology.h"
#include "InputLayout.h"
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
	[[nodiscard]] const bool Initialize(Microsoft::WRL::ComPtr<ID3D11Device> device, Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext) noexcept;
	[[nodiscard]] const bool Demo(); //TODO: Remove once event system is implemented.
};