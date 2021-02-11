#pragma once
#include "IBindable.h"
class IShader : public IBindable
{
protected:
	Microsoft::WRL::ComPtr<ID3DBlob> m_pShaderBlob;
public:
	IShader() = default;
	virtual ~IShader() = default;
	virtual void Bind(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext) = 0;
	virtual void Unbind(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext) = 0;
	virtual [[nodiscard]] const bool Create(const Microsoft::WRL::ComPtr<ID3D11Device>& pDevice, const LPCWSTR& fileName) = 0;
};