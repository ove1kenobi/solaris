#pragma once
#include "../DXDebug.h"
#include "../EventSystem/IEventListener.h"
#include "../EventSystem/EventPublisher.h"
#include "../EventSystem/RenderEvents.h"
#include "../GameObject.h"
struct MatrixBufferShadow
{
	DirectX::XMMATRIX worldMatrix;
	DirectX::XMMATRIX viewMatrix;
	DirectX::XMMATRIX projectionMatrix;
};

#define NUM_PASSES 6
class ShadowMapping : public EventPublisher
{
private:
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pShaderResourceView;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_pDepthStencilViews[6];
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pMatrixCBuffer;
	DirectX::XMFLOAT3 m_CameraDirections[6];
	DirectX::XMFLOAT3 m_CameraUpVectors[6];
	DirectX::XMFLOAT3 m_SunPosition;
	DirectX::XMFLOAT4X4 m_OrthographicProjection;
	float m_TextureWidth;
	float m_TextureHeight;
public:
	ShadowMapping() noexcept;
	virtual ~ShadowMapping() noexcept = default;
	[[nodiscard]] const bool Initialize(const Microsoft::WRL::ComPtr<ID3D11Device>& pDevice);
	void PreparePasses(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext) noexcept;
	void DoPasses(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext, const std::vector<GameObject*>* gameObjects) noexcept;
	void CleanUp() noexcept;
};