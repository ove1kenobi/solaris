#include "..\pch.h"
#include "VertexBuffer.h"

VertexBuffer::VertexBuffer() noexcept
	: m_pVertexBuffer{ nullptr },
	  m_Stride{ 0u },
	  m_Offset{ 0u },
	  m_Slot{ 0u }
{

}

void VertexBuffer::Bind(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext)
{
#if defined(DEBUG) | defined (_DEBUG)
	assert(pDeviceContext && m_pVertexBuffer);
#endif
	pDeviceContext->IASetVertexBuffers(m_Slot, 1u, m_pVertexBuffer.GetAddressOf(), &m_Stride, &m_Offset);
}

void VertexBuffer::Unbind(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext)
{
#if defined(DEBUG) | defined (_DEBUG)
	assert(pDeviceContext && m_pVertexBuffer);
#endif
	ID3D11Buffer* nullBuffer[1] = { nullptr };
	UINT stride = 0u;
	UINT offset = 0u;
	pDeviceContext->IASetVertexBuffers(m_Slot, 1u, nullBuffer, &stride, &offset);
}

const bool VertexBuffer::Create(const Microsoft::WRL::ComPtr<ID3D11Device>& pDevice, 
								const D3D11_USAGE& usage, 
								const UINT& cpuAccessFlags, 
								const UINT& byteWidth,
								const UINT& structureByteStride,
								const void* pData,
								const UINT& slot)
{
	D3D11_BUFFER_DESC bufferDescriptor = {};
	bufferDescriptor.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDescriptor.Usage = usage;
	bufferDescriptor.CPUAccessFlags = cpuAccessFlags;
	bufferDescriptor.MiscFlags = 0u;
	bufferDescriptor.ByteWidth = byteWidth;
	bufferDescriptor.StructureByteStride = structureByteStride;
	D3D11_SUBRESOURCE_DATA subResourceData = {};
	subResourceData.pSysMem = pData;
	HR(pDevice->CreateBuffer(&bufferDescriptor, &subResourceData, &m_pVertexBuffer), "CreateBuffer");
	m_Stride = structureByteStride;
	m_Slot = slot;
	return true;
}
