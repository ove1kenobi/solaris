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
	EventBuss::Get().AddListener(this, EventType::UnbindPipelineEvent, EventType::BindIDEvent);

	if (!CreateAllBindables())
		return false;

	return Demo(vertexBuffer, indexBuffer, WMatrix, VMatrix, PMatrix);
}

const bool ResourceManager::CreateAllBindables()
{
	//Vertex Shaders:
	if (!m_VertexShaderMinimal.Create(m_pDevice, L"VertexShader_Minimalistic.hlsl"))
		return false;
	//Pixel Shaders:
	if (!m_PixelShaderMinimal.Create(m_pDevice, L"PixelShader_Minimalistic.hlsl"))
		return false;
	//Geometry Shaders:

	//Hull Shaders:

	//Domain Shaders:

	//InputLayouts:
	if (!m_InputLayoutDefault.Create(m_pDevice, m_VertexShaderMinimal))
		return false;
	//Primitive topologies:
	if (!m_TopologyTriList.Create(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST))
		return false;
	//Samplers:

	//Arrange:
	//Minimal:
	m_BindablesMinimalistic.insert(m_BindablesMinimalistic.end(), { &m_VertexShaderMinimal, &m_PixelShaderMinimal, &m_InputLayoutDefault, &m_TopologyTriList });

	return true;
}

void ResourceManager::UnbindPipeline()
{
	m_pDeviceContext->VSSetShader(nullptr, nullptr, 0u);
	m_pDeviceContext->VSSetConstantBuffers(0u, 0u, nullptr);
	m_pDeviceContext->PSSetShader(nullptr, nullptr, 0u);
	m_pDeviceContext->PSSetSamplers(0u, 0u, nullptr);
	m_pDeviceContext->PSSetShaderResources(0u, 0u, nullptr);
	m_pDeviceContext->DSSetShader(nullptr, nullptr, 0u);
	m_pDeviceContext->DSSetShaderResources(0u, 0u, nullptr);
	m_pDeviceContext->DSSetConstantBuffers(0u, 0u, nullptr);
	m_pDeviceContext->HSSetShader(nullptr, nullptr, 0u);
	m_pDeviceContext->HSSetShaderResources(0u, 0u, nullptr);
	m_pDeviceContext->HSSetConstantBuffers(0u, 0u, nullptr);
	m_pDeviceContext->GSSetShader(nullptr, nullptr, 0u);
	m_pDeviceContext->GSSetShaderResources(0u, 0u, nullptr);
	m_pDeviceContext->HSSetConstantBuffers(0u, 0u, nullptr);
}

void ResourceManager::BindToPipeline(IEvent& event)
{
	BindIDEvent& derivedEvent = static_cast<BindIDEvent&>(event);
	switch (derivedEvent.GetBindID())
	{
	case BindID::ID_Minimal :
		for (auto bindables : m_BindablesMinimalistic)
		{
			if (!bindables->IsBound())
			{
				bindables->Bind(m_pDeviceContext);
			}
		}
		break;
	}
}

const bool ResourceManager::Demo(std::vector<float> vertexArray, std::vector<int> indexBuffer, DirectX::XMMATRIX WMatrix, DirectX::XMMATRIX VMatrix, DirectX::XMMATRIX PMatrix)
{
	UINT stride = 3u * sizeof(float);
	UINT offset = 0u;
	UINT nrOfVertices = 3u;

	Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;

	D3D11_BUFFER_DESC vertexBufferDescriptor = {};
	vertexBufferDescriptor.ByteWidth = sizeof(float) * static_cast<UINT>(vertexArray.size());
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
	indexBufferDescriptor.ByteWidth = sizeof(unsigned int) * static_cast<UINT>(indexBuffer.size());
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

void ResourceManager::OnEvent(IEvent& event) noexcept
{
	switch (event.GetEventType())
	{
	case EventType::UnbindPipelineEvent :
		UnbindPipeline();
			break;

	case EventType::BindIDEvent :
		BindToPipeline(event);
		break;
	}
}
