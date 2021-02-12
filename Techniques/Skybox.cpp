#include "..\pch.h"
#include "Skybox.h"

Skybox::Skybox() noexcept
	: m_pCameraCBuffer{ nullptr },
	  m_pCamera{ nullptr }
{

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
	EventBuss::Get().AddListener(this, EventType::DelegateCameraEvent);

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
	//Unbind pipeline
	UnbindPipelineEvent ubEvent;
	EventBuss::Get().Delegate(ubEvent);

	//Bind Skybox bindables
	BindIDEvent bindEvent(BindID::ID_Skybox);
	EventBuss::Get().Delegate(bindEvent);

	//Get the camera for its matrices
	RequestCameraEvent requestCameraEvent;
	EventBuss::Get().Delegate(requestCameraEvent);

	//Toggle depth stencil and rasterizer
	ToggleDepthStencilStateEvent dsEvent;
	EventBuss::Get().Delegate(dsEvent);

	//There should be as serious thought whether this should really be done here (Emil F)
	MatrixBuffer buffer = {};
	buffer.VMatrix = DirectX::XMMatrixTranspose(m_pCamera->getVMatrix());
	buffer.PMatrix = DirectX::XMMatrixTranspose(m_pCamera->getPMatrix());

	D3D11_MAPPED_SUBRESOURCE mappedSubresource = {};
	HR_X(pDeviceContext->Map(m_pCameraCBuffer.Get(),
							 0,
							 D3D11_MAP_WRITE_DISCARD,
							 0,
							 &mappedSubresource), "Map");
	MatrixBuffer* data = (MatrixBuffer*)mappedSubresource.pData;
	data->VMatrix = buffer.VMatrix;
	data->PMatrix = buffer.PMatrix;
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