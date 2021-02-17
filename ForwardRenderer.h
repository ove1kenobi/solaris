#pragma once
#include "Bloom.h"
#include "Techniques\Skybox.h"
#include "EventSystem/IEventListener.h"
#include "EventSystem/EventPublisher.h"
#include "EventSystem/RenderEvents.h"
#include "EventSystem\UtilityEvents.h"
#include "GameObject.h"
#include "PointLight.h"

class ForwardRenderer : public EventPublisher, public IEventListener 
{
private:
	FLOAT m_Background[4];
	std::vector<GameObject*>* m_pGameObjects;
	PointLight* m_pSunLight;
	PlayerCamera* m_pCamera;
	Microsoft::WRL::ComPtr<ID3D11Device> m_pDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_pDeviceContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_pBackBuffer;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_pDepthStencilView;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pLightCBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pCameraCBuffer;
	Skybox m_Skybox;
private:
	void BeginFrame();
	void EndFrame();
	void UpdateDXHandlers(IEvent& event) noexcept;
	void OnEvent(IEvent& event) noexcept;
	[[nodiscard]] const bool InitializeSunLight();
	[[nodiscard]] const bool InitializeCameraBuffer();
	void BindLightData();
	void BindCameraData();
public:
	ForwardRenderer() noexcept;
	virtual ~ForwardRenderer() noexcept = default;
	[[nodiscard]] const bool Initialize() noexcept;
	ID3D11RenderTargetView* RenderFrame();
};