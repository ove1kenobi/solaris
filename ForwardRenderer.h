#pragma once
#include "Bloom.h"
#include "Techniques\Skybox.h"
#include "EventSystem/IEventListener.h"
#include "EventSystem/EventPublisher.h"
#include "EventSystem/RenderEvents.h"
#include "EventSystem\UtilityEvents.h"
#include "Scene.h"
#include "Techniques\WaterPostProcessing.h"

class ForwardRenderer : public EventPublisher, public IEventListener 
{
private:
	FLOAT m_Background[4];
	RenderData* m_pRenderData;
	Microsoft::WRL::ComPtr<ID3D11Device> m_pDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_pDeviceContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_pBackBuffer;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_pDepthStencilView;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pDepthShaderResourceView; 
	Skybox m_Skybox;
	WaterPostProcessing m_WaterPP;
private:
	void BeginFrame();
	void EndFrame();
	void UpdateDXHandlers(IEvent& event) noexcept;
	void OnEvent(IEvent& event) noexcept;
public:
	ForwardRenderer() noexcept;
	virtual ~ForwardRenderer() noexcept = default;
	[[nodiscard]] const bool Initialize(UINT screenWidth, UINT screenHeight) noexcept;
	void RenderFrame();
};