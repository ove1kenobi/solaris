#pragma once
#include "../DXDebug.h"
#define NUM_PASSES 6
class ShadowMapping
{
private:
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pShaderResourceView;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_pDepthStencilViews[6];
	DirectX::XMFLOAT3 m_CameraDirections[6];
	DirectX::XMFLOAT3 m_CameraUpVectors[6];
	DirectX::XMFLOAT4X4 m_OrthographicProjection;
	unsigned int m_TextureWidth;
	unsigned int m_TextureHeight;
public:
	ShadowMapping() noexcept;
	virtual ~ShadowMapping() noexcept = default;
	[[nodiscard]] const bool Initialize(const Microsoft::WRL::ComPtr<ID3D11Device>& pDevice);
	void PreparePasses(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext) noexcept;
	void DoPasses(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext) noexcept;
	void CleanUp() noexcept;
};