#pragma once
#include "../DXDebug.h"
#include "../EventSystem/IEventListener.h"
#include "../EventSystem/EventPublisher.h"
#include "../EventSystem/RenderEvents.h"
#include "../GameObject.h"
struct MatrixBufferShadow
{
	DirectX::XMMATRIX worldMatrix;
	DirectX::XMMATRIX worldViewProjectionMatrix;
};
struct ShadowData
{
	float bias;
	DirectX::XMFLOAT3 padding;
};

struct LightData
{
	DirectX::XMFLOAT3 position;
	float padding2;
};

#define NUM_PASSES 6
class ShadowMapping : public EventPublisher
{
private:
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pShaderResourceView;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_pDepthStencilViews[6];
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_pRenderTargetViews[6];
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pMatrixCBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pShadowDataBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pLightDataBuffer;
	Microsoft::WRL::ComPtr<ID3D11BlendState> m_pShadowBlendState;
	Microsoft::WRL::ComPtr<ID3D11BlendState> m_pDefaultBlendState;
	DirectX::XMFLOAT3 m_CameraDirections[6];
	DirectX::XMFLOAT3 m_CameraUpVectors[6];
	DirectX::XMFLOAT3 m_SunPosition;
	DirectX::XMFLOAT4X4 m_OrthographicProjection;
	float m_TextureWidth;
	float m_TextureHeight;
	const FLOAT m_ClearColor[4];
	float m_ShadowBias;
public:
	ShadowMapping() noexcept;
	virtual ~ShadowMapping() noexcept = default;
	[[nodiscard]] const bool Initialize(const Microsoft::WRL::ComPtr<ID3D11Device>& pDevice);
	void PreparePasses(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext, const DirectX::XMFLOAT3& lightPosition) noexcept;
	void DoPasses(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext, const std::vector<GameObject*>* gameObjects, const size_t& numPlanets) noexcept;
	void CleanUp(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext) noexcept;
	void BindSRV(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext) noexcept;
	void UpdateBias(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext) noexcept;
};