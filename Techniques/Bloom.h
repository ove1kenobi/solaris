#pragma once
#include "GaussianBlur.h"
class Bloom : public EventPublisher
{
private:
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_pRenderTargetViewBright;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pShaderResourceViewBright;
	FLOAT m_ClearColor[4];
	GaussianBlur m_GaussianBlur;
public:
	Bloom() noexcept;
	virtual ~Bloom() noexcept = default;
	[[nodiscard]] const bool Initialize(const Microsoft::WRL::ComPtr<ID3D11Device>& pDevice) noexcept;
	void PrepareLumaExtractionPass(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext);
	void DoLumaExtractionPass(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext);
	void CleanUpLumaExtractionPass(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext);
	void DoBlurPass(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext) noexcept;
};