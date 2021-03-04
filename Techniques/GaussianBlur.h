#pragma once
#include "../DXDebug.h"
#include "../EventSystem/EventPublisher.h"
#include "../EventSystem/RenderEvents.h"
class GaussianBlur : public EventPublisher
{
private:
	Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> m_pHorizontalBlurUAV;
	Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> m_pFinalBlurredUAV;	//The result after the vertical blur.
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pHorizontalBlurSRV;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pFinalBlurredSRV;
	FLOAT m_ClearColor[4];
public:
	GaussianBlur() noexcept;
	virtual ~GaussianBlur() noexcept = default;
	[[nodiscard]] const bool Initialize(const Microsoft::WRL::ComPtr<ID3D11Device>& pDevice, const unsigned int& textureWidth, const unsigned int& textureHeight);
	void PrepareHorizontalBlurPass(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext);
	void DoHorizontalPass(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext);
	void CleanUpHorizontalPass(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext);
	void PrepareVerticalBlurPass(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext);
	void DoVerticalBlurPass(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext);
	void CleanUpVerticalBlurPass(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext);
};