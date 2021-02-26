#include "pch.h"
#include "ForwardRenderer.h"

ForwardRenderer::ForwardRenderer() noexcept
	: m_Background{ 0.0f, 0.0f, 0.0f, 1.0f },
	  m_pCulledData{ nullptr },
	  m_pSunLight{ nullptr },
	  m_pCamera{ nullptr },
	  m_pDevice{ nullptr },
	  m_pDeviceContext{ nullptr },
	  m_pBackBuffer{ nullptr },
	  m_pDepthStencilView{ nullptr },
	  m_pLightCBuffer{ nullptr },
	  m_pCameraCBuffer{ nullptr }
{
	EventBuss::Get().AddListener(this, EventType::SendRenderObjectsEvent, EventType::DelegateDXEvent, EventType::DelegateSunLightEvent);
}

//Sets everything up for forward rendering, takes information from the event handler as input
void ForwardRenderer::BeginFrame()
{
	m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
	m_pDeviceContext->ClearRenderTargetView(m_pBackBuffer.Get(), m_Background);

	//Start by unbinding pipeline:
	UnbindPipelineEvent ubEvent;
	EventBuss::Get().Delegate(ubEvent);

	//Bind minimalistic:
	BindIDEvent bindEventMinimal(BindID::ID_Minimal);
	EventBuss::Get().Delegate(bindEventMinimal);

	//Request-event for game objects 
	AskForRenderObjectsEvent event;
	EventBuss::Get().Delegate(event);
	
	//Bind light:
	BindLightData();
	//Bind Camera:
	BindCameraData();

	//Planets:
	size_t i = 0;
	for (i; i < m_pCulledData->nrOfPlanetsInView; ++i) {
		(m_pCulledData->culledObjects)[i]->bindUniques(m_pDeviceContext);
		m_pDeviceContext->DrawIndexed((m_pCulledData->culledObjects)[i]->getIndexBufferSize(), 0u, 0u);
	}

	//Bind orbit:
	BindIDEvent bindEventOrbit(BindID::ID_Orbit);
	EventBuss::Get().Delegate(bindEventOrbit);

	//Orbits:
	for (i; i < m_pCulledData->totalNrOfOrbits + m_pCulledData->nrOfPlanetsInView; ++i) {
		(m_pCulledData->culledObjects)[i]->bindUniques(m_pDeviceContext);
		m_pDeviceContext->DrawIndexed((m_pCulledData->culledObjects)[i]->getIndexBufferSize(), 0u, 0u);
	}
	if (m_pCulledData->sunCulled == false)
	{
		//Bind Sun:
		BindIDEvent bindEventSun(BindID::ID_SUN);
		EventBuss::Get().Delegate(bindEventSun);
		//Sun:
		(m_pCulledData->culledObjects)[i]->bindUniques(m_pDeviceContext);
		m_pDeviceContext->DrawIndexed((m_pCulledData->culledObjects)[i]->getIndexBufferSize(), 0u, 0u);
		i++;
	}

	//Bind minimalistic:
	EventBuss::Get().Delegate(bindEventMinimal);
	//Player:
	(m_pCulledData->culledObjects)[i]->bindUniques(m_pDeviceContext);
	m_pDeviceContext->DrawIndexed((m_pCulledData->culledObjects)[i]->getIndexBufferSize(), 0u, 0u);

	//Skybox time:
	m_Skybox.PreparePass(m_pDeviceContext);
	m_Skybox.DoPass(m_pDeviceContext);
	m_Skybox.CleanUp();
}

//Cleans up for the next frame and applies post processing effects
void ForwardRenderer::EndFrame() 
{
	/*information that is needed from the event handler and resource manager:
	* - what renderTarget should we write to? (Will be the one that gets returned as well)
	*/

	//Apply the post processing effects to the renderTarget here
}

void ForwardRenderer::UpdateDXHandlers(IEvent& event) noexcept
{
	DelegateDXEvent& derivedEvent = static_cast<DelegateDXEvent&>(event);
	m_pDeviceContext = derivedEvent.GetDeviceContext();
	m_pBackBuffer = derivedEvent.GetBackBuffer();
	m_pDepthStencilView = derivedEvent.GetDepthStencilView();
	m_pDevice = derivedEvent.GetDevice();
#if defined(DEBUG) | defined(_DEBUG)
	assert(m_pDeviceContext && m_pBackBuffer && m_pDepthStencilView && m_pDevice);
#endif
}

const bool ForwardRenderer::Initialize() noexcept
{
	if (!m_Skybox.Initialize(m_pDevice))
		return false;
	if (!InitializeSunLight())
		return false;
	EventBuss::Get().AddListener(this, EventType::DelegateCameraEvent);
	if (!InitializeCameraBuffer())
		return false;
	return true;
}

//Calls private functions to render the frame
ID3D11RenderTargetView* ForwardRenderer::RenderFrame()
{
	BeginFrame();
	return nullptr;
}

void ForwardRenderer::OnEvent(IEvent& event) noexcept 
{
	switch (event.GetEventType())
	{
	case EventType::SendRenderObjectsEvent:
	{
		SendRenderObjectsEvent& derivedEvent = static_cast<SendRenderObjectsEvent&>(event);
		m_pCulledData = derivedEvent.GetCulledData();
		break;
	}
	case EventType::DelegateDXEvent:
	{
		UpdateDXHandlers(event);
		break;
	}
	case EventType::DelegateSunLightEvent:
	{
		DelegateSunLightEvent& derivedEvent = static_cast<DelegateSunLightEvent&>(event);
		m_pSunLight = derivedEvent.GetSunLight();
		break;
	}
	case EventType::DelegateCameraEvent:
	{
		DelegateCameraEvent& derivedEvent = static_cast<DelegateCameraEvent&>(event);
		m_pCamera = derivedEvent.GetCamera();
		EventBuss::Get().RemoveListener(this, EventType::DelegateCameraEvent);
		break;
	}
	}
}

const bool ForwardRenderer::InitializeSunLight()
{
	//Constant buffer for camera:
	D3D11_BUFFER_DESC lightBufferDesc = {};
	lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	lightBufferDesc.ByteWidth = sizeof(BlinnPhongLightCB);
	lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightBufferDesc.MiscFlags = 0;
	lightBufferDesc.StructureByteStride = 0;
	HR(m_pDevice->CreateBuffer(&lightBufferDesc,
							   nullptr,
							   &m_pLightCBuffer), 
							   "CreateBuffer");
	RequestSunLightEvent sunEvent;
	EventBuss::Get().Delegate(sunEvent);
	return true;
}

const bool ForwardRenderer::InitializeCameraBuffer()
{
	D3D11_BUFFER_DESC cameraBufferDesc = {};
	cameraBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	cameraBufferDesc.ByteWidth = sizeof(CameraCB);
	cameraBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cameraBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cameraBufferDesc.MiscFlags = 0;
	cameraBufferDesc.StructureByteStride = 0;
	HR(m_pDevice->CreateBuffer(&cameraBufferDesc,
							   nullptr,
							   &m_pCameraCBuffer),
							   "CreateBuffer");
	RequestCameraEvent camEvent;
	EventBuss::Get().Delegate(camEvent);
	return true;
}

void ForwardRenderer::BindLightData()
{
	D3D11_MAPPED_SUBRESOURCE mappedSubresource = {};
	HR_X(m_pDeviceContext->Map(m_pLightCBuffer.Get(),
							 0,
							 D3D11_MAP_WRITE_DISCARD,
							 0,
							 &mappedSubresource), 
							 "Map");
	BlinnPhongLightCB* data = (BlinnPhongLightCB*)mappedSubresource.pData;
#if defined(DEBUG) | defined(_DEBUG)
	static float sAmbientColor[3] = { 0.5f, 0.5f, 0.5f };
	static float sDiffuseColor[3] = { 1.0f, 0.95f, 0.93f };
	static float sDiffuseLightIntensity = m_pSunLight->GetDiffuseLightIntensity();
	static float sLightWorldPosition[3] = { 0.0f, 0.0f, 0.0f };
	static float sAmbientLightIntensity = 0.5f;
	static float sSpecularIntensity = 1.0f;
	static float sSpecularPower = 16.0f;

	data->ambientColor = DirectX::XMFLOAT3(sAmbientColor[0], sAmbientColor[1], sAmbientColor[2]);
	data->diffuseColor = DirectX::XMFLOAT3(sDiffuseColor[0], sDiffuseColor[1], sDiffuseColor[2]);
	data->diffuseLightIntensity = sDiffuseLightIntensity;
	data->lightWorldPosition = DirectX::XMFLOAT3(sLightWorldPosition[0], sLightWorldPosition[1], sLightWorldPosition[2]);
	data->ambientLightIntensity = sAmbientLightIntensity;
	data->specularIntensity = sSpecularIntensity;
	data->specularPower = sSpecularPower;

	ImGui::Begin("Lighting");
	ImGui::DragFloat3("Light World Position", sLightWorldPosition, 20.0f);
	ImGui::DragFloat3("Ambient Color", sAmbientColor, 0.05f, 0.0f, 1.0f);
	ImGui::DragFloat("Ambient Light Intensity", &sAmbientLightIntensity, 0.05f, 0.0f, 1.0f);
	ImGui::DragFloat3("Diffuse Color", sDiffuseColor, 0.05f, 0.0f, 1.0f);
	ImGui::DragFloat("Diffuse Light Intensity", &sDiffuseLightIntensity, 0.05f, 0.0f, 1.0f);
	ImGui::DragFloat("Specular intensity", &sSpecularIntensity, 0.05f, 0.0f, 1.0f);
	ImGui::DragFloat("Specular Power", &sSpecularPower, 0.05f, 1.0f, 30.0f);
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
	data->ambientColor = DirectX::XMFLOAT3(0.5f, 0.5f, 0.5f);
	data->diffuseColor = m_pSunLight->GetDiffuseColor();
	data->diffuseLightIntensity = m_pSunLight->GetDiffuseLightIntensity();
	data->lightWorldPosition = m_pSunLight->GetWorldPosition();
	data->ambientLightIntensity = 0.5f;
	data->specularIntensity = 1.0f;  //Should be material property later (Emil F)
	data->specularPower = 16.0f; //Should be material property later (Emil F)
#endif
	m_pDeviceContext->Unmap(m_pLightCBuffer.Get(), 0);
	m_pDeviceContext->PSSetConstantBuffers(0u, 1u, m_pLightCBuffer.GetAddressOf());
}

void ForwardRenderer::BindCameraData()
{
	D3D11_MAPPED_SUBRESOURCE mappedSubresource = {};
	HR_X(m_pDeviceContext->Map(m_pCameraCBuffer.Get(),
							   0,
							   D3D11_MAP_WRITE_DISCARD,
							   0,
							   &mappedSubresource),
							   "Map");
	CameraCB* data = (CameraCB*)mappedSubresource.pData;

	data->cameraWorldPosition = m_pCamera->getPos();

	m_pDeviceContext->Unmap(m_pCameraCBuffer.Get(), 0);
	m_pDeviceContext->PSSetConstantBuffers(1u, 1u, m_pCameraCBuffer.GetAddressOf());
}
