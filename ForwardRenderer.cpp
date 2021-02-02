#include "ForwardRenderer.h"

ForwardRenderer::ForwardRenderer() noexcept
	: m_Background{ 0.0f, 0.0f, 0.0f, 1.0f },
	  m_gameObjects{ nullptr }
{
}

//Sets everything up for forward rendering, takes information from the event handler as input
void ForwardRenderer::BeginFrame()
{
	m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView.Get(), 0u, 1.0f, 0u);
	m_pDeviceContext->ClearRenderTargetView(m_pBackBuffer.Get(), m_Background);

	//Request-event for game objects 
	AskForRenderObjectsEvent event;
	EventBuss::Get().Delegate(event);

	(*m_gameObjects)[0]->bindUniques(m_pDeviceContext);
	m_pDeviceContext->DrawIndexed((*m_gameObjects)[0]->getIndexBufferSize(), 0u, 0u);
}

//Cleans up for the next frame and applies post processing effects
void ForwardRenderer::EndFrame() 
{
	/*information that is needed from the event handler and resource manager:
	* - what renderTarget should we write to? (Will be the one that gets returned as well)
	*/

	//Apply the post processing effects to the renderTarget here
}

const bool ForwardRenderer::Initialize(Microsoft::WRL::ComPtr<ID3D11DeviceContext> pDeviceContext,
									   Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pBackBuffer,
									   Microsoft::WRL::ComPtr<ID3D11DepthStencilView> pDepthStencilView) noexcept
{
	EventBuss::Get().AddListener(this, EventType::SendRenderObjectsEvent);
	m_pDeviceContext = pDeviceContext;
	m_pBackBuffer = pBackBuffer;
	m_pDepthStencilView = pDepthStencilView;
#if defined(DEBUG) | defined(_DEBUG)
	assert(m_pDeviceContext && m_pBackBuffer && m_pDepthStencilView);
#endif
	return true;
}

//Calls private functions to render the frame
ID3D11RenderTargetView* ForwardRenderer::RenderFrame()
{
	//return the finished render to the Engine after applying BeginFrame, SubmitObjects, and then Endframe to it.
	BeginFrame();
	//return this->EndFrame(target);

	//return this->EndFrame(this->SubmitObject(this->BeginFrame(renderTarget), gameObjects));
	return nullptr;
}

void ForwardRenderer::OnEvent(IEvent& event) noexcept 
{
	switch (event.GetEventType())
	{
	case EventType::SendRenderObjectsEvent:
		//Do something with objects.
		SendRenderObjectsEvent& derivedEvent = static_cast<SendRenderObjectsEvent&>(event);
		this->m_gameObjects = derivedEvent.getGameObjectVector();
		break;
	}
}