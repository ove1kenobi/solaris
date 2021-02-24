#include "..\pch.h"
#include "WaterPostProcessing.h"

WaterPostProcessing::WaterPostProcessing() noexcept
	: m_pCameraCBuffer{ nullptr },
	m_pCamera{ nullptr },
	m_pSunLight{ nullptr },
	m_screenWidth{ 0 },
	m_screenHeight{ 0 }
{
	EventBuss::Get().AddListener(this, EventType::DelegateCameraEvent, EventType::DelegatePlanetsEvent, EventType::DelegateSunLightEvent);
	EventBuss::Get().AddListener(this, EventType::DelegateSunEvent);
}

WaterPostProcessing::~WaterPostProcessing() {
}

void WaterPostProcessing::AssignCamera(IEvent& event) noexcept
{
	DelegateCameraEvent& derivedEvent = static_cast<DelegateCameraEvent&>(event);
	m_pCamera = derivedEvent.GetCamera();
}

void WaterPostProcessing::AssignPlanets(IEvent& event) noexcept
{
	DelegatePlanetsEvent& derivedEvent = static_cast<DelegatePlanetsEvent&>(event);
	m_pPlanets = *derivedEvent.GetPlanets();
}

void WaterPostProcessing::AssignSunLight(IEvent& event) noexcept
{
	DelegateSunLightEvent& derivedEvent = static_cast<DelegateSunLightEvent&>(event);
	m_pSunLight = derivedEvent.GetSunLight();
}

void WaterPostProcessing::AssignSun(IEvent& event) noexcept
{
	DelegateSunEvent& derivedEvent = static_cast<DelegateSunEvent&>(event);
	m_pSunCenter = derivedEvent.GetCenter();
	m_pSunRadius = derivedEvent.GetRadius();
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
		break;
	case EventType::DelegateSunLightEvent:
	{
		AssignSunLight(event);
		break;
	}
	case EventType::DelegateSunEvent:
		AssignSun(event);
		break;
	}
}

const bool WaterPostProcessing::Initialize(const Microsoft::WRL::ComPtr<ID3D11Device>& pDevice, UINT screenWidth, UINT screenHeight) noexcept
{
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

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

	//Constant buffer for screen:
	D3D11_BUFFER_DESC screenBufferDesc = {};
	screenBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	screenBufferDesc.ByteWidth = sizeof(ScreenData);
	screenBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	screenBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	screenBufferDesc.MiscFlags = 0;
	screenBufferDesc.StructureByteStride = 0;
	HR(pDevice->CreateBuffer(&screenBufferDesc,
		nullptr,
		&m_pScreenCBuffer), "CreateBuffer");

	//Constant buffer for light:
	D3D11_BUFFER_DESC lightBufferDesc = {};
	lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	lightBufferDesc.ByteWidth = sizeof(BlinnPhongLightCB);
	lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightBufferDesc.MiscFlags = 0;
	lightBufferDesc.StructureByteStride = 0;
	HR(pDevice->CreateBuffer(&lightBufferDesc,
		nullptr,
		&m_pLightCBuffer), "CreateBuffer");

	//Get the camera for its matrices
	RequestCameraEvent requestCameraEvent;
	EventBuss::Get().Delegate(requestCameraEvent);

	//Get the planets for their radius' and centerpoints.
	RequestPlanetsEvent requestPlanetsEvent;
	EventBuss::Get().Delegate(requestPlanetsEvent);

	//Get the sunlight
	RequestSunLightEvent sunLightEvent;
	EventBuss::Get().Delegate(sunLightEvent);

	RequestSunEvent sunEvent;
	EventBuss::Get().Delegate(sunEvent);

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

	for (size_t i = 0; i < m_pPlanets.size(); i++) {
		DirectX::XMFLOAT3 center = m_pPlanets[i]->GetCenter();
		DirectX::XMFLOAT4 centerRadius = { center.x, center.y, center.z, m_pPlanets[i]->GetRadius() };
		data->center[i] = DirectX::XMLoadFloat4(&centerRadius);
	}
	//Fill the rest with zero's
	DirectX::XMFLOAT4 nullXMF4 = { 0.0f, 0.0f, 0.0f, 0.0f };
	for (size_t i = m_pPlanets.size(); i < 50; i++) {
		data->center[i] = DirectX::XMLoadFloat4(&nullXMF4);
	}

	DirectX::XMFLOAT4 sunData = { m_pSunCenter->x, m_pSunCenter->y, m_pSunCenter->z, *m_pSunRadius };
	data->sun = DirectX::XMLoadFloat4(&sunData);

	pDeviceContext->Unmap(m_pPlanetCBuffer.Get(), 0);

	//Camera Constant Buffer
	D3D11_MAPPED_SUBRESOURCE mappedSubresourceCamera = {};
	HR_X(pDeviceContext->Map(m_pCameraCBuffer.Get(),
		0,
		D3D11_MAP_WRITE_DISCARD,
		0,
		&mappedSubresourceCamera), "Map");

	CameraData* dataCamera = (CameraData*)mappedSubresourceCamera.pData;

	DirectX::XMFLOAT3 cameraPos3 = m_pCamera->getPos();
	DirectX::XMFLOAT4 cameraPos = { cameraPos3.x, cameraPos3.y, cameraPos3.z, 1.0f };
	dataCamera->cameraPos = DirectX::XMLoadFloat4(&cameraPos);
	dataCamera->inverseVMatrix = DirectX::XMMatrixTranspose(DirectX::XMMatrixInverse(nullptr, m_pCamera->getVMatrix()));
	dataCamera->PMatrix = m_pCamera->getPMatrix();

	pDeviceContext->Unmap(m_pCameraCBuffer.Get(), 0);

	//Screen Constant Buffer
	D3D11_MAPPED_SUBRESOURCE mappedSubresourceScreen = {};
	HR_X(pDeviceContext->Map(m_pScreenCBuffer.Get(),
		0,
		D3D11_MAP_WRITE_DISCARD,
		0,
		&mappedSubresourceScreen), "Map");

	ScreenData* dataScreen = (ScreenData*)mappedSubresourceScreen.pData;

	dataScreen->screenWidth = (float)m_screenWidth;
	dataScreen->screenHeight = (float)m_screenHeight;

	pDeviceContext->Unmap(m_pScreenCBuffer.Get(), 0);

	//Light constant buffer
	D3D11_MAPPED_SUBRESOURCE mappedSubresourceLight = {};
	HR_X(pDeviceContext->Map(m_pLightCBuffer.Get(),
		0,
		D3D11_MAP_WRITE_DISCARD,
		0,
		&mappedSubresourceLight),
		"Map");
	BlinnPhongLightCB* dataLight = (BlinnPhongLightCB*)mappedSubresourceLight.pData;
#if defined(DEBUG) | defined(_DEBUG)
	static float sAmbientColor[3] = { 0.5f, 0.5f, 0.5f };
	static float sDiffuseColor[3] = { 1.0f, 0.95f, 0.93f };
	static float sDiffuseLightIntensity = m_pSunLight->GetDiffuseLightIntensity();
	static float sLightWorldPosition[3] = { 0.0f, 0.0f, 0.0f };
	static float sAmbientLightIntensity = 0.5f;
	static float sSpecularIntensity = 1.0f;
	static float sSpecularPower = 16.0f;

	dataLight->ambientColor = DirectX::XMFLOAT3(sAmbientColor[0], sAmbientColor[1], sAmbientColor[2]);
	dataLight->diffuseColor = DirectX::XMFLOAT3(sDiffuseColor[0], sDiffuseColor[1], sDiffuseColor[2]);
	dataLight->diffuseLightIntensity = sDiffuseLightIntensity;
	dataLight->lightWorldPosition = DirectX::XMFLOAT3(sLightWorldPosition[0], sLightWorldPosition[1], sLightWorldPosition[2]);
	dataLight->ambientLightIntensity = sAmbientLightIntensity;
	dataLight->specularIntensity = sSpecularIntensity;
	dataLight->specularPower = sSpecularPower;

	ImGui::Begin("Lighting");
	ImGui::DragFloat3("Light World Position", sLightWorldPosition, 20.0f);
	ImGui::DragFloat3("Ambient Color", sAmbientColor, 0.05f, 0.0f, 1.0f);
	ImGui::DragFloat("Ambient Light Intensity", &sAmbientLightIntensity, 0.05f, 0.0f, 1.0f);
	ImGui::DragFloat3("Diffuse Color", sDiffuseColor, 0.05f, 0.0f, 1.0f);
	ImGui::DragFloat("Diffuse Light Intensity", &sDiffuseLightIntensity, 0.05f, 0.0f, 1.0f);
	ImGui::DragFloat("Specular intensity", &sSpecularIntensity, 0.05f, 0.0f, 1.0f);
	ImGui::DragFloat("Specular Power", &sSpecularPower, 0.05f, 1.0f, 80.0f);
	bool isPressed = ImGui::Button("Reset all");
	if (isPressed)
	{
		sAmbientColor[0] = 0.5f;
		sAmbientColor[1] = 0.5f;
		sAmbientColor[2] = 0.5f;
		sDiffuseColor[0] = 1.0f;
		sDiffuseColor[1] = 0.95f;
		sDiffuseColor[2] = 0.93f;
		sDiffuseLightIntensity = m_pSunLight->GetDiffuseLightIntensity();
		sLightWorldPosition[0] = sLightWorldPosition[1] = sLightWorldPosition[2] = 0.0f;
		sAmbientLightIntensity = 0.5f;
		sSpecularIntensity = 1.0f;
		sSpecularPower = 16.0f;
	}
	ImGui::End();
#else
	dataLight->ambientColor = DirectX::XMFLOAT3(0.5f, 0.5f, 0.5f);
	dataLight->diffuseColor = m_pSunLight->GetDiffuseColor();
	dataLight->diffuseLightIntensity = m_pSunLight->GetDiffuseLightIntensity();
	dataLight->lightWorldPosition = m_pSunLight->GetWorldPosition();
	dataLight->ambientLightIntensity = 0.5f;
	dataLight->specularIntensity = 1.0f;  //Should be material property later (Emil F)
	dataLight->specularPower = 16.0f; //Should be material property later (Emil F)
#endif
	pDeviceContext->Unmap(m_pLightCBuffer.Get(), 0);

	
	pDeviceContext->PSSetConstantBuffers(0u, 1u, m_pPlanetCBuffer.GetAddressOf());
	pDeviceContext->PSSetConstantBuffers(1u, 1u, m_pCameraCBuffer.GetAddressOf());
	pDeviceContext->PSSetConstantBuffers(2u, 1u, m_pScreenCBuffer.GetAddressOf());
	pDeviceContext->PSSetConstantBuffers(3u, 1u, m_pLightCBuffer.GetAddressOf());

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