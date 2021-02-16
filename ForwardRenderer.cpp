#include "pch.h"
#include "ForwardRenderer.h"

ForwardRenderer::ForwardRenderer() noexcept
	: m_Background{ 0.0f, 0.0f, 0.0f, 1.0f },
	  m_pGameObjects{ nullptr },
	  m_pSunLight{ nullptr },
	  m_pDevice{ nullptr },
	  m_pDeviceContext{ nullptr },
	  m_pBackBuffer{ nullptr },
	  m_pDepthStencilView{ nullptr },
	  m_pLightCBuffer{ nullptr }
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
	BindIDEvent bindEvent(BindID::ID_Minimal);
	EventBuss::Get().Delegate(bindEvent);

	//Request-event for game objects 
	AskForRenderObjectsEvent event;
	EventBuss::Get().Delegate(event);
	
	//Loop that renders and draws every GameObject.
	for (size_t i = 0; i < (*m_pGameObjects).size(); ++i) {
		(*m_pGameObjects)[i]->bindUniques(m_pDeviceContext);
		m_pDeviceContext->DrawIndexed((*m_pGameObjects)[i]->getIndexBufferSize(), 0u, 0u);
	}

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
		this->m_pGameObjects = derivedEvent.getGameObjectVector();
		break;
	}
	case EventType::DelegateDXEvent:
	{
		UpdateDXHandlers(event);
	}
	}
}

const bool ForwardRenderer::InitializeSunLight()
{
	//Constant buffer for camera:
	D3D11_BUFFER_DESC lightBufferDesc = {};
	lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	lightBufferDesc.ByteWidth = sizeof(PhongLightCB);
	lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightBufferDesc.MiscFlags = 0;
	lightBufferDesc.StructureByteStride = 0;
	HR(m_pDevice->CreateBuffer(&lightBufferDesc,
							   nullptr,
							   &m_pLightCBuffer), 
							   "CreateBuffer");
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
	PhongLightCB* data = (PhongLightCB*)mappedSubresource.pData;
	data->ambientColor = DirectX::XMFLOAT3(0.5f, 0.5f, 0.5f);
	data->diffuseColor = m_pSunLight->GetDiffuseColor();
	data->diffuseLightIntensity = m_pSunLight->GetDiffuseLightIntensity();
	data->lightWorldPosition = m_pSunLight->GetWorldPosition();
	m_pDeviceContext->Unmap(m_pLightCBuffer.Get(), 0);
	m_pDeviceContext->PSSetConstantBuffers(0u, 1u, m_pLightCBuffer.GetAddressOf());
}
