#pragma once
#include "IBindable.h"
#include "../EventSystem/EventPublisher.h"
#include "../EventSystem/IEventListener.h"
#include "../EventSystem/UtilityEvents.h"

class Texture : public IBindable, public EventPublisher, public IEventListener{
private:
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_pRenderTargetView;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pShaderResourceView;
	Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> m_pUnorderedAccessView;
	unsigned int m_Slot;
	bool m_isRenderTarget;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_pDepthStencilView;
	float m_background[4];
public:
	Texture() noexcept;
	virtual ~Texture() noexcept = default;
	void Bind(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext) override;
	void Unbind(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext) override;
	[[nodiscard]] const bool Create(const Microsoft::WRL::ComPtr<ID3D11Device>& pDevice, const unsigned int& slot, int width, int height, D3D11_USAGE usage, UINT bindflags, UINT cpuaccessflags);
	void OnEvent(IEvent& event) noexcept;
};