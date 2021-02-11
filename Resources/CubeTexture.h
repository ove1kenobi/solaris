#pragma once
#include "IBindable.h"
class CubeTexture : public IBindable
{
private:
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pShaderResourceView;
	unsigned int m_Slot;
public:
	CubeTexture() noexcept;;
	virtual ~CubeTexture() noexcept = default;
	void Bind(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext) override;
	void Unbind(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext) override;
	[[nodiscard]] const bool Create(const Microsoft::WRL::ComPtr<ID3D11Device>& pDevice, const LPCWSTR& fileName, const unsigned int& slot = 0u);
};