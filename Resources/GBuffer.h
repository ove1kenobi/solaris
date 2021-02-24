#pragma once
#include "../EventSystem/EventPublisher.h"
#include "../EventSystem/IEventListener.h"
#include "../EventSystem/UtilityEvents.h"
#include "Texture.h"

//Holds all the textures that are sent to the last shader pass.
class GBuffer : public IBindable, public EventPublisher, public IEventListener {
private:
	static const UINT nr = 5;
	std::vector<Texture*> m_Textures;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_RTVArray[nr];
	bool m_isRenderTarget;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_pDepthStencilView;
	float m_background[4];
public:
	GBuffer() noexcept;
	virtual ~GBuffer();
	void Bind(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext) override;
	void Unbind(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext) override;
	[[nodiscard]] const bool Create(const Microsoft::WRL::ComPtr<ID3D11Device>& pDevice, int width, int height);
	void OnEvent(IEvent& event) noexcept;
};