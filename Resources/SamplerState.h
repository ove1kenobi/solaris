#pragma once
#include "IBindable.h"

enum class TechFlag {SKYBOX = 0, SHADOW, POSTPROCESSING};
enum class BindFlag {S_None = 0, S_PS, S_DS};
class SamplerState : public IBindable
{
private:
	Microsoft::WRL::ComPtr<ID3D11SamplerState> m_pSamplerState;
	BindFlag m_BindFlag;
	unsigned int m_Slot;
public:
	SamplerState() noexcept;
	virtual ~SamplerState() noexcept = default;
	void Bind(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext) override;
	void Unbind(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext) override;
	[[nodiscard]] const bool Create(const Microsoft::WRL::ComPtr<ID3D11Device>& pDevice,const BindFlag& bindFlag, const TechFlag& techFlag, const unsigned int& slot);
};