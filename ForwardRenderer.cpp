#include "pch.h"
#include "ForwardRenderer.h"

ForwardRenderer::ForwardRenderer() noexcept
	: m_Background{ 0.0f, 0.0f, 0.0f, 1.0f },
	  m_pRenderData{ nullptr },
	  m_pDevice{ nullptr },
	  m_pDeviceContext{ nullptr },
	  m_pBackBuffer{ nullptr },
	  m_pDepthStencilView{ nullptr },
	  m_LightPosition{ DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f)}
{
	EventBuss::Get().AddListener(this, EventType::SendRenderObjectsEvent, EventType::DelegateDXEvent, EventType::DelegateSunLightEvent);
}

//Sets everything up for forward rendering, takes information from the event handler as input
void ForwardRenderer::BeginFrame()
{
	//Request-event for game objects 
	AskForRenderObjectsEvent event;
	EventBuss::Get().Delegate(event);

	//Shadow map pass(es):
	m_ShadowMapping.PreparePasses(m_pDeviceContext, m_LightPosition);
	m_ShadowMapping.DoPasses(m_pDeviceContext, m_pRenderData);
	m_ShadowMapping.CleanUp(m_pDeviceContext);

	m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
	m_pDeviceContext->ClearRenderTargetView(m_pBackBuffer.Get(), m_Background);

	//Start by unbinding pipeline:
	UnbindPipelineEvent ubEvent;
	EventBuss::Get().Delegate(ubEvent);

	//Bind RenderQuad:
	BindIDEvent bindEvent(BindID::ID_RenderQuad);
	EventBuss::Get().Delegate(bindEvent);

	//Planets:
	size_t i = 0;
	for (i; i < m_pRenderData->nrOfPlanetsInView; ++i) {
		(m_pRenderData->culledObjects)[i]->bindUniques(m_pDeviceContext);
		m_pDeviceContext->DrawIndexed((m_pRenderData->culledObjects)[i]->getIndexBufferSize(), 0u, 0u);
	}

	//Bind orbit:
	BindIDEvent bindEventOrbit(BindID::ID_Orbit);
	EventBuss::Get().Delegate(bindEventOrbit);

	//Orbits:
	for (i; i < m_pRenderData->totalNrOfOrbits + m_pRenderData->nrOfPlanetsInView; ++i) {
		(m_pRenderData->culledObjects)[i]->bindUniques(m_pDeviceContext);
		m_pDeviceContext->DrawIndexed((m_pRenderData->culledObjects)[i]->getIndexBufferSize(), 0u, 0u);
	}
	if (m_pRenderData->sunCulled == false)
	{
		//Bind Sun:
		BindIDEvent bindEventSun(BindID::ID_SUN);
		EventBuss::Get().Delegate(bindEventSun);
		//Sun:
		(m_pRenderData->culledObjects)[i]->bindUniques(m_pDeviceContext);
		m_pDeviceContext->DrawIndexed((m_pRenderData->culledObjects)[i]->getIndexBufferSize(), 0u, 0u);
		i++;
	}

	//Bind Textured models:
	BindIDEvent bindEventTextured(BindID::ID_Textured);
	EventBuss::Get().Delegate(bindEventTextured);
	// Textured models:
	for (i; i < m_pRenderData->totalNrOfObjects; ++i) {
		(m_pRenderData->culledObjects)[i]->bindUniques(m_pDeviceContext);
		m_pDeviceContext->DrawIndexed((m_pRenderData->culledObjects)[i]->getIndexBufferSize(), 0u, 0u);
	}


	//Skybox time:
	m_Skybox.PreparePass(m_pDeviceContext);
	m_Skybox.DoPass(m_pDeviceContext);
	m_Skybox.CleanUp();

	//Bind waterSpheres:
	BindIDEvent bindEventWaterSpheres(BindID::ID_WaterSphere);
	EventBuss::Get().Delegate(bindEventWaterSpheres);

	for (size_t i = 0; i < m_pRenderData->totalNrOfPlanets; i++) {
		(*m_pRenderData->waterSpheres)[i]->bindUniques(m_pDeviceContext);
		m_pDeviceContext->DrawIndexed((*m_pRenderData->waterSpheres)[i]->getIndexBufferSize(), 0u, 0u);
	}

	//size is number of textures in the gbuffer.
	ID3D11RenderTargetView* nullRTV[4] = { nullptr };
	ID3D11DepthStencilView* nullDSV = { nullptr };
	m_pDeviceContext->OMSetRenderTargets(ARRAYSIZE(nullRTV), nullRTV, nullDSV);

	m_WaterPP.PreparePass(m_pDeviceContext, m_pRenderData->culledPlanetsDepthSorted);
	//Bind shadow resource:
	m_ShadowMapping.BindSRV(m_pDeviceContext);
	m_ShadowMapping.UpdateBias(m_pDeviceContext);
	m_WaterPP.DoPass(m_pDeviceContext);
	m_WaterPP.CleanUp(m_pDeviceContext);

	//Bloom Luma Pass:
	//This pass extracts the colors from the texture of the previous pass that pass a certain threshold:
	m_Bloom.PrepareLumaExtractionPass(m_pDeviceContext);
	m_Bloom.DoLumaExtractionPass(m_pDeviceContext);
	m_Bloom.CleanUpLumaExtractionPass(m_pDeviceContext);

	//Bloom Blur Pass:
	//This pass does, 3 times, a Gaussian blur over the texture with the highlight-only-colors: 
	m_Bloom.DoBlurPass(m_pDeviceContext);

	//Anew bind the normal color texture from the water pass:
	m_WaterPP.BindSRV(m_pDeviceContext);

	//Rebind back buffer:
	BindBackBufferEvent bbEvent;
	EventBuss::Get().Delegate(bbEvent);

	//Prepare for the last Bloom phase:
	//This pass combines the original texture color with the colors from the 
	//brightened and Gaussian filtered texture:
	m_Bloom.PrepareCombinePass(m_pDeviceContext);
	m_Bloom.DoCombinePass(m_pDeviceContext);
	m_Bloom.CleanUpCombinePass(m_pDeviceContext);
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
	m_pDepthShaderResourceView = derivedEvent.GetShaderResourceView();
#if defined(DEBUG) | defined(_DEBUG)
	assert(m_pDeviceContext && m_pBackBuffer && m_pDepthStencilView && m_pDevice);
#endif
}

const bool ForwardRenderer::Initialize(UINT screenWidth, UINT screenHeight) noexcept
{
	if (!m_Skybox.Initialize(m_pDevice))
		return false;
	if (!m_WaterPP.Initialize(m_pDevice, screenWidth, screenHeight))
		return false;
	if (!m_ShadowMapping.Initialize(m_pDevice))
		return false;
	if (!m_Bloom.Initialize(m_pDevice))
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
		m_pRenderData = derivedEvent.GetRenderData();
		break;
	}
	case EventType::DelegateDXEvent:
	{
		UpdateDXHandlers(event);
		break;
	}
	}
}