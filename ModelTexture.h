#pragma once
#include <DirectXTK/WICTextureLoader.h>
#include "Resources/IBindable.h"

class ModelTexture : public IBindable
{
private:
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_shaderResourceView;
	UINT m_Slot;
public:
	ModelTexture() = default;
	~ModelTexture() = default;
	void Bind(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext) override;
	void Unbind(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext) override;
	void LoadTexture(const Microsoft::WRL::ComPtr<ID3D11Device>& pDevice, const LPCWSTR& file, UINT type) noexcept;
};