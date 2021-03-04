#pragma once
#include "../DXDebug.h"
#include "../EventSystem/EventPublisher.h"
#include "../EventSystem/RenderEvents.h"
class GaussianBlur : public EventPublisher
{
private:
	Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> m_pBlurredUAV;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pBlurredSRV;
	FLOAT m_ClearColor[4];
public:
	GaussianBlur() noexcept;
	virtual ~GaussianBlur() noexcept = default;
	[[nodiscard]] const bool Initialize(const Microsoft::WRL::ComPtr<ID3D11Device>& pDevice, const unsigned int& textureWidth, const unsigned int& textureHeight);
	void PreparePass(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext);
	void DoPass(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext);
	void CleanUpPass(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext);
};