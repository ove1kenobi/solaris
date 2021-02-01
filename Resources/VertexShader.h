#pragma once
#include "IShader.h"
class VertexShader : public IShader
{
private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_pVertexShader;
public:
	VertexShader() = default;
	virtual ~VertexShader() = default;
	void Bind(Microsoft::WRL::ComPtr<ID3D11DeviceContext> pDeviceContext) override;
	void Unbind(Microsoft::WRL::ComPtr<ID3D11DeviceContext> pDeviceContext) override;
	[[nodiscard]] const bool Create(Microsoft::WRL::ComPtr<ID3D11Device> pDevice, const LPCWSTR& fileName) override;
	[[nodiscard]] const Microsoft::WRL::ComPtr<ID3DBlob>& GetVertexShaderBlob() const noexcept;
};