#include "ResourceManager.h"

ResourceManager::ResourceManager() noexcept
	: m_pDevice{ nullptr }, m_pDeviceContext{ nullptr }
{

}

const bool ResourceManager::Initialize(
	Microsoft::WRL::ComPtr<ID3D11Device> pDevice, 
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pDeviceContext,
	std::vector<float> vertexBuffer,
	std::vector<int> indexBuffer,
	DirectX::XMMATRIX WMatrix,
	DirectX::XMMATRIX VMatrix,
	DirectX::XMMATRIX PMatrix
	) noexcept
{
	m_pDevice = pDevice;
	m_pDeviceContext = pDeviceContext;
	return Demo(vertexBuffer, indexBuffer, WMatrix, VMatrix, PMatrix);
}

const bool ResourceManager::Demo(std::vector<float> vertexArray, std::vector<int> indexBuffer, DirectX::XMMATRIX WMatrix, DirectX::XMMATRIX VMatrix, DirectX::XMMATRIX PMatrix)
{
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
	/*
	float vertexArray[] =
	{
		0.0f, 0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f
	};*/
	UINT stride = 3u * sizeof(float);
	UINT offset = 0u;
	UINT nrOfVertices = 3u;

	Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;

	D3D11_BUFFER_DESC vertexBufferDescriptor = {};
	vertexBufferDescriptor.ByteWidth = sizeof(float) * vertexArray.size();
	vertexBufferDescriptor.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDescriptor.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	D3D11_SUBRESOURCE_DATA sr_data = { 0 };
	sr_data.pSysMem = vertexArray.data();
	HR(m_pDevice->CreateBuffer(&vertexBufferDescriptor,
							   &sr_data,
							   &pVertexBuffer),
						       "CreateBuffer");
	m_pDeviceContext->IASetVertexBuffers(0u,
										 1u,
										 pVertexBuffer.GetAddressOf(),
										 &stride,
										 &offset);

	Microsoft::WRL::ComPtr<ID3D11Buffer> pIndexBuffer = NULL;

	D3D11_BUFFER_DESC indexBufferDescriptor = {};
	indexBufferDescriptor.ByteWidth = sizeof(unsigned int) * indexBuffer.size();
	indexBufferDescriptor.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDescriptor.BindFlags = D3D11_BIND_INDEX_BUFFER;

	// Define the resource data.
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = indexBuffer.data();
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;

	// Create the buffer with the device.
	HR(m_pDevice->CreateBuffer(	&indexBufferDescriptor,
								&InitData,
								&pIndexBuffer),
								"CreateBuffer");
	

	// Set the buffer.
	m_pDeviceContext->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	//Matrixbuffer for shader
	D3D11_BUFFER_DESC matrixBufferDesc;
	Microsoft::WRL::ComPtr<ID3D11Buffer> matrixBuffer = NULL;

	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBuffer);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	m_pDevice->CreateBuffer(&matrixBufferDesc, NULL, &matrixBuffer);

	D3D11_MAPPED_SUBRESOURCE mappedSubresource;
	MatrixBuffer* data;

	WMatrix = DirectX::XMMatrixTranspose(WMatrix);
	VMatrix = DirectX::XMMatrixTranspose(VMatrix);
	PMatrix = DirectX::XMMatrixTranspose(PMatrix);

	m_pDeviceContext->Map(
		matrixBuffer.Get(),
		0,
		D3D11_MAP_WRITE_DISCARD,
		0,
		&mappedSubresource
	);

	data = (MatrixBuffer*)mappedSubresource.pData;

	data->WMatrix = WMatrix;
	data->VMatrix = VMatrix;
	data->PMatrix = PMatrix;

	m_pDeviceContext->Unmap(matrixBuffer.Get(), 0);
	m_pDeviceContext->VSSetConstantBuffers(0, 1, matrixBuffer.GetAddressOf());
	return true;
}
