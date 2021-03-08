#pragma once
#include "..\EventSystem\EventPublisher.h"
#include "..\EventSystem\IEventListener.h"
#include "..\EventSystem\RenderEvents.h"
#include "..\EventSystem\UtilityEvents.h"
#include "..\DXDebug.h"
#include "..\PlayerCamera.h"
#include "..\Planet.h"
#include "..\PointLight.h"

struct PlanetData
{
	DirectX::XMFLOAT4 center[50];
	DirectX::XMFLOAT4 waterColor[50];
	DirectX::XMFLOAT4 sun;
};

struct CameraData
{
	DirectX::XMFLOAT4 cameraPos;
	DirectX::XMFLOAT4X4 inverseVMatrix;
	DirectX::XMFLOAT4X4 PMatrix;
	float element00;
	float element11;
	DirectX::XMFLOAT2 padding;
};

struct ScreenData
{
	float screenWidth;
	float screenHeight;
	DirectX::XMFLOAT2 padding;
};

class WaterPostProcessing : public IEventListener, public EventPublisher
{
private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pPlanetCBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pCameraCBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pScreenCBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pLightCBuffer;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_pRenderTargetView;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pShaderResourceView;

	PlayerCamera* m_pCamera;
	PointLight* m_pSunLight;
	DirectX::XMFLOAT3* m_pSunCenter;
	float* m_pSunRadius;

	UINT m_screenWidth;
	UINT m_screenHeight;
	FLOAT m_ClearColor[4];
private:
	void AssignCamera(IEvent& event) noexcept;
	void AssignSunLight(IEvent& event) noexcept;
	void AssignSun(IEvent& event) noexcept;
	void OnEvent(IEvent& event) noexcept override;
public:
	WaterPostProcessing() noexcept;
	virtual ~WaterPostProcessing() noexcept = default;
	[[nodiscard]] const bool Initialize(const Microsoft::WRL::ComPtr<ID3D11Device>& pDevice, UINT screenWidth, UINT screenHeight) noexcept;
	void PreparePass(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext, std::vector<GameObject*> planets) noexcept;
	void DoPass(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext) noexcept;
	void CleanUp(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext) noexcept;
	void BindSRV(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext);
};