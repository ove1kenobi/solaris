#pragma once
#include "Bloom.h"
#include "EventSystem/IEventListener.h"
#include "EventSystem/EventPublisher.h"
#include "EventSystem/RenderEvents.h"
#include "EventSystem\UtilityEvents.h"
#include "GameObject.h"
class ForwardRenderer : public EventPublisher, IEventListener 
{
private:
	//Color of the screen
	FLOAT m_Background[4];
	std::vector<GameObject*>* m_gameObjects;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_pDeviceContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_pBackBuffer;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_pDepthStencilView;
private:
	void BeginFrame();
	void EndFrame();
	void UpdateDXHandlers(IEvent& event) noexcept;
public:
	ForwardRenderer() noexcept;
	virtual ~ForwardRenderer() = default;
	[[nodiscard]] const bool Initialize() noexcept;
	ID3D11RenderTargetView* RenderFrame();
	void OnEvent(IEvent& event) noexcept;
};