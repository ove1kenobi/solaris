#include "pch.h"
#include "ForwardRenderer.h"

ForwardRenderer::ForwardRenderer() noexcept
	: m_Background{ 0.0f, 0.0f, 0.0f, 1.0f },
	  m_pGameObjects{ nullptr },
	  m_pDevice{ nullptr },
	  m_pDeviceContext{ nullptr },
	  m_pBackBuffer{ nullptr },
	  m_pDepthStencilView{ nullptr },
	m_numPlanets{ 0 }
{
	EventBuss::Get().AddListener(this, EventType::SendRenderObjectsEvent, EventType::DelegateDXEvent);
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
	
	//Loop that renders and draws every GameObject.
	for (size_t i = 0; i < m_numPlanets; ++i) {
		(*m_pGameObjects)[i]->bindUniques(m_pDeviceContext);
		m_pDeviceContext->DrawIndexed((*m_pGameObjects)[i]->getIndexBufferSize(), 0u, 0u);
	}

	//Bind orbit:
	BindIDEvent bindEventOrbit(BindID::ID_Orbit);
	EventBuss::Get().Delegate(bindEventOrbit);

	for (size_t i = m_numPlanets; i < m_numPlanets * 2; ++i) {
		(*m_pGameObjects)[i]->bindUniques(m_pDeviceContext);
		m_pDeviceContext->DrawIndexed((*m_pGameObjects)[i]->getIndexBufferSize(), 0u, 0u);
	}

	//Bind minimalistic:
	EventBuss::Get().Delegate(bindEventMinimal);

	for (size_t i = m_numPlanets * 2; i < (*m_pGameObjects).size(); ++i) {
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
	return true;
}

//Calls private functions to render the frame
void ForwardRenderer::RenderFrame()
{
	BeginFrame();
}

void ForwardRenderer::OnEvent(IEvent& event) noexcept 
{
	switch (event.GetEventType())
	{
	case EventType::SendRenderObjectsEvent:
	{
		SendRenderObjectsEvent& derivedEvent = static_cast<SendRenderObjectsEvent&>(event);
		m_pGameObjects = derivedEvent.getGameObjectVector();
		m_numPlanets = derivedEvent.GetNumPlanets();
		break;
	}
	case EventType::DelegateDXEvent:
	{
		UpdateDXHandlers(event);
	}
	}
}