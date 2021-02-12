#pragma once
#include "IShader.h"

class ComputeShader : public IShader
{
private:
	Microsoft::WRL::ComPtr<ID3D11ComputeShader> m_pComputeShader;
public:
	ComputeShader() = default;
	virtual ~ComputeShader() = default;
	void Bind(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext) override;
	void Unbind(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext) override;
	[[nodiscard]] const bool Create(const Microsoft::WRL::ComPtr<ID3D11Device>& pDevice, const LPCWSTR& fileName) override;
};


