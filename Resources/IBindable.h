#pragma once
#include "..\DXDebug.h"
class IBindable
{
private:
	bool m_IsBound;
public:
	IBindable();
	virtual ~IBindable() = default;
	virtual void Bind(Microsoft::WRL::ComPtr<ID3D11DeviceContext> pDeviceContext) = 0;
	virtual void Unbind(Microsoft::WRL::ComPtr<ID3D11DeviceContext> pDeviceContext) = 0;
	[[nodiscard]] const bool& IsBound() const noexcept;
};