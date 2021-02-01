#pragma once
#include "IShader.h"
class PixelShader : public IShader
{
private:
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pPixelShader;
public:
	PixelShader() = default;
	virtual ~PixelShader() = default;
	void Bind(Microsoft::WRL::ComPtr<ID3D11DeviceContext> pDeviceContext) override;
	void Unbind(Microsoft::WRL::ComPtr<ID3D11DeviceContext> pDeviceContext) override;
	[[nodiscard]] const bool Create(Microsoft::WRL::ComPtr<ID3D11Device> pDevice, const LPCWSTR& fileName) override;
};