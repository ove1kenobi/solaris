#pragma once
#include "IBindable.h"
#include "../EventSystem/EventPublisher.h"
#include "../EventSystem/IEventListener.h"
#include "../EventSystem/UtilityEvents.h"

class Texture{
private:
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_pRenderTargetView;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pShaderResourceView;
	unsigned int m_Slot;
public:
	Texture() noexcept;
	virtual ~Texture() noexcept = default;
	[[nodiscard]] const bool Create(const Microsoft::WRL::ComPtr<ID3D11Device>& pDevice, const unsigned int& slot, int width, int height, D3D11_USAGE usage, UINT cpuaccessflags);

	const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& getSRV();
	const Microsoft::WRL::ComPtr<ID3D11RenderTargetView>& getRTV();
};