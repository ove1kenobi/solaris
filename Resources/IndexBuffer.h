/*The IndexBuffer class is for those objects that do not change during app runtime.
  If your IndexBuffer must be updated, or is highly specific per model, consider using BindUnique instead. (Emil F)*/
#pragma once
#include "IBindable.h"
class IndexBuffer : public IBindable
{
private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pIndexBuffer;
public:
	IndexBuffer() noexcept;
	virtual ~IndexBuffer() noexcept = default;
	void Bind(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext);
	void Unbind(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext);
	[[nodiscard]] const bool Create(const Microsoft::WRL::ComPtr<ID3D11Device>& pDevice,
									const D3D11_USAGE& usage,
									const UINT& cpuAccessFlags,
									const UINT& byteWidth,
									const UINT& structureByteStride,
									const void* pData);
};