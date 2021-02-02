#include "ResourceManager.h"

ResourceManager::ResourceManager() noexcept
	: m_pDevice{ nullptr }, m_pDeviceContext{ nullptr }
{

}

const bool ResourceManager::Initialize(Microsoft::WRL::ComPtr<ID3D11Device> pDevice, 
									   Microsoft::WRL::ComPtr<ID3D11DeviceContext> pDeviceContext) noexcept
{
	m_pDevice = pDevice;
	m_pDeviceContext = pDeviceContext;
	return Demo();
}

const bool ResourceManager::Demo()
{
	/* DEMO MODEL (0ve) */
	Model* model = ModelFactory::Get().GetModel(std::string("models/cubes.obj"));

	/*DEMO INITIALIZATION. THIS IS ONLY TEMPORARY (Emil F) */
	if (!m_VertexShaderDemo.Create(m_pDevice, L"VertexShader_Minimalistic.hlsl"))
		return false;
	m_VertexShaderDemo.Bind(m_pDeviceContext);
	
	if (!m_PixelShaderDemo.Create(m_pDevice, L"PixelShader_Minimalistic.hlsl"))
		return false;
	m_PixelShaderDemo.Bind(m_pDeviceContext);

	if (!m_InputLayoutDemo.Create(m_pDevice, m_VertexShaderDemo))
		return false;
	m_InputLayoutDemo.Bind(m_pDeviceContext);
	
	if (!m_TopologyDemo.Create(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST))
		return false;
	m_TopologyDemo.Bind(m_pDeviceContext);

	//float vertexArray[] =
	//{
	//	0.0f, 0.5f, 0.0f,
	//	0.5f, -0.5f, 0.0f,
	//	-0.5f, -0.5f, 0.0f
	//};
	UINT stride = 3u * sizeof(float);
	UINT offset = 0u;
	UINT nrOfVertices = model->NumVertices(); // 3u;

	Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;

	D3D11_BUFFER_DESC vertexBufferDescriptor = {};
	vertexBufferDescriptor.ByteWidth = model->DataLength(); // sizeof(vertexArray);
	vertexBufferDescriptor.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDescriptor.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	D3D11_SUBRESOURCE_DATA sr_data = { 0 };
	sr_data.pSysMem = model->PosData(); // vertexArray;
	HR(m_pDevice->CreateBuffer(&vertexBufferDescriptor,
							   &sr_data,
							   &pVertexBuffer),
						       "CreateBuffer");
	m_pDeviceContext->IASetVertexBuffers(0u,
										 1u,
										 pVertexBuffer.GetAddressOf(),
										 &stride,
										 &offset);

	return true;
}
