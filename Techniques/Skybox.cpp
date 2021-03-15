#include "..\pch.h"
#include "Skybox.h"

Skybox::Skybox() noexcept
	: m_pCameraCBuffer{ nullptr },
	  m_pCamera{ nullptr }
{
	EventBuss::Get().AddListener(this, EventType::DelegateCameraEvent);
}

Skybox::~Skybox() {
	EventBuss::Get().RemoveListener(this, EventType::DelegateCameraEvent);
}

void Skybox::AssignCamera(IEvent& event) noexcept
{
	DelegateCameraEvent& derivedEvent = static_cast<DelegateCameraEvent&>(event);
	m_pCamera = derivedEvent.GetCamera();
}

void Skybox::OnEvent(IEvent& event) noexcept
{
	switch (event.GetEventType())
	{
	case EventType::DelegateCameraEvent:
		AssignCamera(event);
		break;
	}
}

const bool Skybox::Initialize(const Microsoft::WRL::ComPtr<ID3D11Device>& pDevice) noexcept
{
	//Constant buffer for camera:
	D3D11_BUFFER_DESC matrixBufferDesc = {};
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBuffer);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;
	HR(pDevice->CreateBuffer(&matrixBufferDesc,
							 nullptr,
							 &m_pCameraCBuffer), "CreateBuffer");
	
	return true;
}

void Skybox::PreparePass(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext) noexcept
{
	//Get the camera for its matrices
	RequestCameraEvent requestCameraEvent;
	EventBuss::Get().Delegate(requestCameraEvent);

	//Unbind pipeline
	UnbindPipelineEvent ubEvent;
	EventBuss::Get().Delegate(ubEvent);

	//Bind Skybox bindables
	BindIDEvent bindEvent(BindID::ID_Skybox);
	EventBuss::Get().Delegate(bindEvent);

	//Toggle depth stencil and rasterizer
	ToggleDepthStencilStateEvent dsEvent;
	EventBuss::Get().Delegate(dsEvent);

	//There should be as serious thought whether this should really be done here (Emil F)
	DirectX::XMFLOAT4X4 VMatrix = m_pCamera->getVMatrix();
	DirectX::XMFLOAT4X4 PMatrix = m_pCamera->getPMatrix();

	DirectX::XMMATRIX vMatrix = DirectX::XMMatrixTranspose(DirectX::XMLoadFloat4x4(&VMatrix));
	DirectX::XMMATRIX pMatrix = DirectX::XMMatrixTranspose(DirectX::XMLoadFloat4x4(&PMatrix));

	DirectX::XMStoreFloat4x4(&VMatrix, vMatrix);
	DirectX::XMStoreFloat4x4(&PMatrix, pMatrix);

	D3D11_MAPPED_SUBRESOURCE mappedSubresource = {};
	HR_X(pDeviceContext->Map(m_pCameraCBuffer.Get(),
							 0,
							 D3D11_MAP_WRITE_DISCARD,
							 0,
							 &mappedSubresource), 
							 "Map");

	memcpy(mappedSubresource.pData, &vMatrix, sizeof(DirectX::XMFLOAT4X4));
	memcpy(((char*)mappedSubresource.pData) + sizeof(DirectX::XMFLOAT4X4), &pMatrix, sizeof(DirectX::XMFLOAT4X4));

	pDeviceContext->Unmap(m_pCameraCBuffer.Get(), 0);

	pDeviceContext->VSSetConstantBuffers(0u, 1u, m_pCameraCBuffer.GetAddressOf());
}

void Skybox::DoPass(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext) noexcept
{
	//Hard coded, this will ALWAYS be 36 in this pass. (Emil F)
	pDeviceContext->DrawIndexed(36u, 0u, 0u);
}

void Skybox::CleanUp() noexcept
{
	ToggleDepthStencilStateEvent dsEvent;
	EventBuss::Get().Delegate(dsEvent);
}