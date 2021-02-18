#include "..\pch.h"
#include "WaterPostProcessing.h"

WaterPostProcessing::WaterPostProcessing() noexcept
	: m_pCameraCBuffer{ nullptr },
	m_pCamera{ nullptr }
{

}

void WaterPostProcessing::AssignCamera(IEvent& event) noexcept
{
	DelegateCameraEvent& derivedEvent = static_cast<DelegateCameraEvent&>(event);
	m_pCamera = derivedEvent.GetCamera();
}

void WaterPostProcessing::AssignPlanets(IEvent& event) noexcept
{
	DelegatePlanetsEvent& derivedEvent = static_cast<DelegatePlanetsEvent&>(event);
	m_planets = *derivedEvent.GetPlanets();
}

void WaterPostProcessing::OnEvent(IEvent& event) noexcept
{
	switch (event.GetEventType())
	{
	case EventType::DelegateCameraEvent:
		AssignCamera(event);
		break;

	case EventType::DelegatePlanetsEvent:
		AssignPlanets(event);
	}
	
}

const bool WaterPostProcessing::Initialize(const Microsoft::WRL::ComPtr<ID3D11Device>& pDevice) noexcept
{
	EventBuss::Get().AddListener(this, EventType::DelegateCameraEvent, EventType::DelegatePlanetsEvent);

	//Constant buffer for the planet data:
	D3D11_BUFFER_DESC planetBufferDesc = {};
	planetBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	planetBufferDesc.ByteWidth = sizeof(PlanetData);
	planetBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	planetBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	planetBufferDesc.MiscFlags = 0;
	planetBufferDesc.StructureByteStride = 0;
	HR(pDevice->CreateBuffer(&planetBufferDesc,
		nullptr,
		&m_pPlanetCBuffer), "CreateBuffer");

	//Constant buffer for camera:
	D3D11_BUFFER_DESC cameraBufferDesc = {};
	cameraBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	cameraBufferDesc.ByteWidth = sizeof(CameraData);
	cameraBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cameraBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cameraBufferDesc.MiscFlags = 0;
	cameraBufferDesc.StructureByteStride = 0;
	HR(pDevice->CreateBuffer(&cameraBufferDesc,
		nullptr,
		&m_pCameraCBuffer), "CreateBuffer");
	return true;
}

void WaterPostProcessing::PreparePass(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext) noexcept
{
	//Unbind pipeline:
	UnbindPipelineEvent ubEvent;
	EventBuss::Get().Delegate(ubEvent);

	//prepare for post processing the water.
	BindIDEvent bindWaterEvent(BindID::ID_Water);
	EventBuss::Get().Delegate(bindWaterEvent);

	//Get the camera for its matrices
	RequestCameraEvent requestCameraEvent;
	EventBuss::Get().Delegate(requestCameraEvent);

	//Get the planets for their radius' and centerpoints.
	RequestPlanetsEvent requestPlanetsEvent;
	EventBuss::Get().Delegate(requestPlanetsEvent);

	//Bind backbuffer
	BindBackBufferEvent bindBackBuffer;
	EventBuss::Get().Delegate(bindBackBuffer);

	//Planet Constant Buffer
	D3D11_MAPPED_SUBRESOURCE mappedSubresource = {};
	HR_X(pDeviceContext->Map(m_pPlanetCBuffer.Get(),
		0,
		D3D11_MAP_WRITE_DISCARD,
		0,
		&mappedSubresource), "Map");

	PlanetData* data = (PlanetData*)mappedSubresource.pData;

	for (size_t i = 0; i < m_planets.size(); i++) {
		DirectX::XMFLOAT3 center = m_planets[i]->GetCenter();
		DirectX::XMFLOAT4 centerRadius = { center.x, center.y, center.z, m_planets[i]->GetRadius() };
		data->center[i] = DirectX::XMLoadFloat4(&centerRadius);
	}
	//Fill the rest with zero's
	DirectX::XMFLOAT4 nullXMF4 = { 0.0f, 0.0f, 0.0f, 0.0f };
	for (size_t i = m_planets.size(); i < 50; i++) {
		data->center[i] = DirectX::XMLoadFloat4(&nullXMF4);
	}

	pDeviceContext->Unmap(m_pPlanetCBuffer.Get(), 0);

	//Camera Constant Buffer
	D3D11_MAPPED_SUBRESOURCE mappedSubresourceCamera = {};
	HR_X(pDeviceContext->Map(m_pCameraCBuffer.Get(),
		0,
		D3D11_MAP_WRITE_DISCARD,
		0,
		&mappedSubresourceCamera), "Map");

	CameraData* dataCamera = (CameraData*)mappedSubresourceCamera.pData;

	dataCamera->cameraDir = m_pCamera->GetForward();
	DirectX::XMFLOAT3 cameraPos3 = m_pCamera->getPos();
	DirectX::XMFLOAT4 cameraPos = { cameraPos3.x, cameraPos3.y, cameraPos3.z, 1.0f };
	dataCamera->cameraPos = DirectX::XMLoadFloat4(&cameraPos);
	dataCamera->inverseVMatrix = DirectX::XMMatrixInverse(nullptr, DirectX::XMMatrixTranspose(m_pCamera->getVMatrix()));
	dataCamera->inversePMatrix = DirectX::XMMatrixInverse(nullptr, DirectX::XMMatrixTranspose(m_pCamera->getPMatrix()));

	pDeviceContext->Unmap(m_pCameraCBuffer.Get(), 0);

	pDeviceContext->PSSetConstantBuffers(0u, 1u, m_pPlanetCBuffer.GetAddressOf());
	pDeviceContext->PSSetConstantBuffers(1u, 1u, m_pCameraCBuffer.GetAddressOf());

	
	ToggleDepthStencilStateEvent dsEvent;
	EventBuss::Get().Delegate(dsEvent);
}

void WaterPostProcessing::DoPass(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext) noexcept
{
	//Hard coded, this will ALWAYS be 6 in this pass.
	pDeviceContext->DrawIndexed(6u, 0u, 0u);
}

void WaterPostProcessing::CleanUp() noexcept
{
	ToggleDepthStencilStateEvent dsEvent;
	EventBuss::Get().Delegate(dsEvent);
}