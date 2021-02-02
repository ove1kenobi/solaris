#pragma once
#include <d3d11.h>
#include <vector>
#include <DirectXMath.h>

#include "DXDebug.h"
#include "Bloom.h"
#include "EventSystem/IEventListener.h"
#include "EventSystem/EventPublisher.h"
#include "EventSystem/RenderEvents.h"

class ForwardRenderer : public EventPublisher, IEventListener 
{
private:
	//Color of the screen
	FLOAT m_Background[4];

	//Techniques that will be applied
	Bloom m_Bloom;

	std::vector<GameObject*>* m_gameObjects;

	//Emil F:s edits:
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_pDeviceContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_pBackBuffer;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_pDepthStencilView;

	//Render functions
	ID3D11RenderTargetView* BeginFrame();
	ID3D11RenderTargetView* EndFrame(ID3D11RenderTargetView* renderTarget);
public:
	ForwardRenderer() noexcept;
	virtual ~ForwardRenderer() = default;
	[[nodiscard]] const bool Initialize(Microsoft::WRL::ComPtr<ID3D11DeviceContext> pDeviceContext, 
										Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pBackBuffer, 
										Microsoft::WRL::ComPtr<ID3D11DepthStencilView> pDepthStencilView) noexcept;
	ID3D11RenderTargetView* RenderFrame();

	void OnEvent(IEvent& event) noexcept;
};