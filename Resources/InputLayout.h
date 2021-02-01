#pragma once
#include "IBindable.h"
#include "VertexShader.h"
class InputLayout : public IBindable
{
private:
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayout;
public:
	InputLayout() = default;
	virtual ~InputLayout() = default;
	void Bind(Microsoft::WRL::ComPtr<ID3D11DeviceContext> pDeviceContext) override;
	void Unbind(Microsoft::WRL::ComPtr<ID3D11DeviceContext> pDeviceContext) override;
	[[nodiscard]] const bool Create(Microsoft::WRL::ComPtr<ID3D11Device> pDevice, const VertexShader& vertexShader);
};
