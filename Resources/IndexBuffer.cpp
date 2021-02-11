#include "..\pch.h"
#include "IndexBuffer.h"

IndexBuffer::IndexBuffer() noexcept
	: m_pIndexBuffer{ nullptr }
{

}

void IndexBuffer::Bind(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext)
{
#if defined(DEBUG) | defined (_DEBUG)
	assert(pDeviceContext && m_pIndexBuffer);
#endif
	pDeviceContext->IASetIndexBuffer(m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0u);
}

void IndexBuffer::Unbind(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext)
{
#if defined(DEBUG) | defined (_DEBUG)
	assert(pDeviceContext && m_pIndexBuffer);
#endif
	ID3D11Buffer* nullBuffer = { nullptr };
	pDeviceContext->IASetIndexBuffer(nullBuffer, DXGI_FORMAT_R32_UINT, 0u);
}

const bool IndexBuffer::Create(const Microsoft::WRL::ComPtr<ID3D11Device>& pDevice, 
							   const D3D11_USAGE& usage, 
							   const UINT& cpuAccessFlags, 
							   const UINT& byteWidth, 
							   const UINT& structureByteStride, 
							   const void* pData)
{
	D3D11_BUFFER_DESC bufferDescriptor = {};
	bufferDescriptor.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDescriptor.Usage = usage;
	bufferDescriptor.CPUAccessFlags = cpuAccessFlags;
	bufferDescriptor.MiscFlags = 0u;
	bufferDescriptor.ByteWidth = byteWidth;
	bufferDescriptor.StructureByteStride = structureByteStride;
	D3D11_SUBRESOURCE_DATA subResourceData = {};
	subResourceData.pSysMem = pData;
	HR(pDevice->CreateBuffer(&bufferDescriptor, &subResourceData, &m_pIndexBuffer), "CreateBuffer");
	return true;
}
