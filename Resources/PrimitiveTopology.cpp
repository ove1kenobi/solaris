#include "..\pch.h"
#include "PrimitiveTopology.h"

PrimitiveTopology::PrimitiveTopology() noexcept
	: m_Topology{ D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED }
{

}

void PrimitiveTopology::Bind(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext)
{
#if defined(DEBUG) | defined (_DEBUG)
	assert(pDeviceContext);
#endif
	pDeviceContext->IASetPrimitiveTopology(m_Topology);
}

void PrimitiveTopology::Unbind(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext)
{
#if defined(DEBUG) | defined (_DEBUG)
	assert(pDeviceContext);
#endif
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED);
}

const bool PrimitiveTopology::Create(D3D11_PRIMITIVE_TOPOLOGY topology)
{
	m_Topology = topology;
	return true; //Redundant, really. (Emil F)
}
