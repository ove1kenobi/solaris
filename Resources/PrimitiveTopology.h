#pragma once
#include "IBindable.h"
class PrimitiveTopology : public IBindable
{
private:
	D3D11_PRIMITIVE_TOPOLOGY m_Topology;
public:
	PrimitiveTopology() noexcept;
	virtual ~PrimitiveTopology() noexcept = default;
	void Bind(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext) override;
	void Unbind(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext) override;
	[[nodiscard]] const bool Create(D3D11_PRIMITIVE_TOPOLOGY topology);
};